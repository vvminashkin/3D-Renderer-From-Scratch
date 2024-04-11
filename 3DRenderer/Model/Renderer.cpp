#include "Renderer.h"
#include "Primitives.h"
#include <cassert>
#include <iostream>

namespace renderer {
namespace {
using Vector3d = Eigen::Vector3d;
using Vector2d = Eigen::Vector2d;
using Matrix2d = Eigen::Matrix2d;
Renderer::Matrix3d TransformToScreenSpace(Renderer::Matrix3d triangle, size_t width,
                                          size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);

    triangle.col(0) += Vector3d::Ones();
    triangle.col(0) *= dwidth / 2;

    triangle.col(1) = -triangle.col(1);
    triangle.col(1) += Vector3d::Ones();
    triangle.col(1) *= dheight / 2;
    return triangle;
}
Renderer::Matrix3d TransformToCameraSpace(Renderer::Matrix3d triangle, size_t width,
                                          size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);

    triangle.col(0) /= dwidth / 2;
    triangle.col(0) -= Vector3d::Ones();

    triangle.col(1) /= dheight / 2;
    triangle.col(1) -= Vector3d::Ones();
    triangle.col(1) = -triangle.col(1);
    return triangle;
}
Vector2d TransformVectorToCameraSpace(Vector2d vec, size_t width, size_t height) {
    assert(width != 0);
    assert(height != 0);

    double dwidth = width;
    double dheight = height;

    vec.x() /= dwidth / 2;
    vec.x() -= 1;

    vec.y() /= dheight / 2;
    vec.y() -= 1;
    vec.y() = -vec.y();
    return vec;
}

Vector3d ConvertToBarycentric(Vector2d coordinates, const Renderer::Matrix3d &triangle) {
    Vector3d ans;
    coordinates -= triangle.row(2).topLeftCorner<1, 2>();
    Matrix2d temp;
    temp = triangle.topLeftCorner<2, 2>().transpose();
    temp.col(0) -= triangle.row(2).topLeftCorner<1, 2>();
    temp.col(1) -= triangle.row(2).topLeftCorner<1, 2>();
    ans.topLeftCorner<2, 1>() = temp.inverse() * coordinates;
    ans.z() = 1 - ans.x() - ans.y();
    return ans;
}
bool CheckIfInside(const Vector3d &b_coordinate) {
    return b_coordinate.x() <= 1.0 && b_coordinate.x() >= 0.0 && b_coordinate.y() <= 1.0 &&
           b_coordinate.y() >= 0.0 && b_coordinate.z() <= 1.0 && b_coordinate.z() >= 0;
}
size_t RoundDown(double coordinate) {
    return static_cast<size_t>(std::max(0.0, std::floor(coordinate)));
}
size_t RoundUp(double coordinate) {
    return static_cast<size_t>(std::ceil(coordinate));
}
};  // namespace

std::unique_ptr<Screen> Renderer::Draw(const World &world, size_t width, size_t height) {
    std::unique_ptr<Screen> screen(new Screen(width, height));
    for (const auto &object : world.GetObjectsIterable()) {
        for (const auto &triangle : object->GetMesh().GetTriangles()) {
            DrawTriangle(triangle, object, world, screen.get());
        }
    }
    return screen;
}

void Renderer::ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix =
        Renderer::MakeHomogeneousTransformationMatrix(owner.GetAngle(), owner.GetCoordinates());
    Renderer::ApplyMatrix(transformation_matrix, &(*vertices));
}

void Renderer::ShiftTriangleToAlignCamera(const World &world, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix = Renderer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(), -world.GetCameraPosition());
    // std::cout<<world.GetCameraRotation()<<std::endl;
    // std::cout<<vertices->GetVerticiesCoordinates()<<std::endl;
    Renderer::ApplyMatrix(transformation_matrix, vertices);
    // std::cout<<vertices->GetVerticiesCoordinates()<<std::endl;
}

void Renderer::DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                            const World &world, Screen *screen) {
    Triangle vertices = owner_object->GetMesh().MakeTriangleVertices(current);

    ShiftTriangleCoordinates(owner_object, &vertices);

    ShiftTriangleToAlignCamera(world, &vertices);
    Eigen::Matrix<double, 3, 4> transformed_vertices =
        world.GetCamera().ApplyPerspectiveTransformation(
            vertices.GetVerticesHomogeniousCoordinates());
    BarycentricCoordinateSystem system(vertices, transformed_vertices);
    std::list<Matrix3d> triangles;
    triangles.push_back(vertices.GetVerticesHomogeniousCoordinates().topLeftCorner<3, 3>());
    // std::cout<<vertices.GetVerticesHomogeniousCoordinates()<<std::endl;

    for (int i = 0; i < world.GetCamera().GetClippingPlanes().rows(); ++i) {
        ClipAllTriangles(world.GetCamera().GetClippingPlanes().row(i), &triangles);
    }

    for (const auto &curr : triangles) {
        Eigen::Matrix<double, 3, 4> homogeneous_coords;
        homogeneous_coords.topLeftCorner<3, 3>() = curr;
        homogeneous_coords.col(3) = Vector3d::Ones();
        homogeneous_coords = world.GetCamera().ApplyPerspectiveTransformation(homogeneous_coords);
        RasterizeTriangle(system, homogeneous_coords.topLeftCorner<3, 3>(), screen);
    }
    // RasterizeTriangle(system, system.GetTriangle(), &screen);
}
void Renderer::RasterizeTriangle(const BarycentricCoordinateSystem &system,
                                 const Matrix3d &coordinates, Screen *screen) {
    size_t height = screen->GetHeight();
    size_t width = screen->GetWidth();
    Matrix3d screen_triangle = TransformToScreenSpace(coordinates, width, height);
    size_t min_x = -1;
    size_t min_y = -1;
    size_t max_x = 0;
    size_t max_y = 0;

    for (int i = 0; i < 3; ++i) {
        min_x = std::min(RoundDown(screen_triangle.row(i).x()), min_x);
        min_y = std::min(RoundDown(screen_triangle.row(i).y()), min_y);
        max_x = std::max(RoundUp(screen_triangle.row(i).x()), max_x);
        max_y = std::max(RoundUp(screen_triangle.row(i).y()), max_y);
    }
    assert("bounded dimensions are not OK" && max_x < 2 * screen->GetWidth() &&
           max_y < 2 * screen->GetHeight());
    max_x = std::min(static_cast<size_t>(screen->GetWidth() - 1), max_x);
    max_y = std::min(static_cast<size_t>(screen->GetHeight() - 1), max_y);
    for (size_t x = min_x; x <= max_x; ++x) {
        for (size_t y = min_y; y <= max_y; ++y) {
            Vector2d vec = {static_cast<double>(x) + 0.5, static_cast<double>(y) + 0.5};
            Vector3d b_coordinate = system.ConvertToBarycentricCoordinates(
                TransformVectorToCameraSpace(vec, width, height));
            if (CheckIfInside(b_coordinate)) {
                double z = system.GetOriginalCoordinates(b_coordinate).norm();
                if (z >= screen->GetZ(y, x)) {
                    screen->SetZ(y, x, z);
                    RGB color = system.GetColor(b_coordinate);
                    color.SetB(std::sin(z / 1.5));
                    screen->SetPixel(y, x, color);
                }
            }
        }
    }
}

bool Renderer::DetermineSide(const Eigen::Vector4d &plane, const Eigen::Vector3d &point) {
    assert("Plane is not zero" && (!(plane.x() == 0 && plane.y() == 0 && plane.z() == 0)));
    Eigen::Vector3d point_on_plane = plane.topLeftCorner<3, 1>() * (-plane.w());
    Eigen::Vector3d point_vector = point - point_on_plane;
    double scalar_prod = plane.topLeftCorner<3, 1>().dot(point_vector);
    return scalar_prod > 0;
}
Eigen::Vector3d Renderer::PlaneLineIntersection(const Eigen::Vector4d &plane,
                                                const Eigen::Vector3d &point1,
                                                const Eigen::Vector3d &point2) {
    // page 98
    Eigen::Vector3d ans;
    Eigen::Vector3d v = point2 - point1;
    double t = (-(plane.topLeftCorner<3, 1>().dot(point1) + plane.w())) /
               (plane.topLeftCorner<3, 1>().dot(v));
    ans = point1 + t * v;
    return ans;
}
bool Renderer::ClipOneTriangle(const Eigen::Vector4d &plane, const Eigen::Matrix3d &triangle,
                               std::list<Eigen::Matrix3d> *triangles) {
    int cnt = 0;

    Eigen::Matrix3d points_inside;
    Eigen::Matrix3d points_outside;
    for (int i = 0; i < 3; ++i) {
        bool is_inside = DetermineSide(plane, triangle.row(i));
        if (is_inside) {
            ++cnt;
            points_inside.row(cnt - 1) = triangle.row(i);
        } else {
            points_outside.row(i - cnt) = triangle.row(i);
        }
    }
    if (cnt == 3) {
        return false;
    }
    if (cnt == 0) {
        return true;
    }
    if (cnt == 1) {
        Eigen::Vector3d first_inter =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(0));
        Eigen::Vector3d second_inter =
            PlaneLineIntersection(plane, points_outside.row(1), points_inside.row(0));
        Eigen::Matrix3d ans;
        ans.row(0) = first_inter;
        ans.row(1) = second_inter;
        ans.row(2) = points_inside.row(0);
        triangles->push_back(ans);
    }
    if (cnt == 2) {

        Eigen::Vector3d first_inter =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(0));
        Eigen::Vector3d second_inter =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(1));
        Eigen::Matrix3d ans1;
        Eigen::Matrix3d ans2;
        ans1.row(0) = first_inter;
        ans1.row(1) = points_inside.row(0);
        ans1.row(2) = points_inside.row(1);

        ans2.row(0) = first_inter;
        ans2.row(1) = second_inter;
        ans2.row(2) = points_inside.row(1);

        triangles->push_back(ans1);
        triangles->push_back(ans2);
    }

    return true;
}
void Renderer::ClipAllTriangles(const Eigen::Vector4d &plane,
                                std::list<Eigen::Matrix3d> *triangles) {
    if (triangles->empty()) {
        return;
    }
    auto it = triangles->end();
    --it;
    while (true) {
        bool is_clipped = ClipOneTriangle(plane, *it, triangles);
        if (is_clipped) {
            bool is_begining = it == triangles->begin();
            auto new_it = triangles->erase(it);
            if (new_it == triangles->begin()) {
                break;
            }
            it = new_it;
            --it;
            continue;
        }
        if (it == triangles->begin()) {
            break;
        }
        --it;
    }
}
Renderer::Matrix4d Renderer::MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                                 const Vector3d &offset) {

    Matrix4d transformation_matrix = Matrix4d::Zero();
    transformation_matrix.topLeftCorner<3, 3>() = rotation.toRotationMatrix();
    transformation_matrix(3, 3) = 1;
    transformation_matrix.col(3).topLeftCorner<3, 1>() = offset;
    return transformation_matrix;
}
void Renderer::ApplyMatrix(const Matrix4d &transformation_matrix, Triangle *vertices) {
    assert(vertices != nullptr);

    // Page 76 "Mathematics for 3D game..."
    for (auto &ver : vertices->GetVerticies()) {
        ver.coordinates =
            (transformation_matrix * ver.coordinates.GetHomogeneousCoordinates()).eval();
        ver.normal = transformation_matrix * ver.normal.GetHomogeneousCoordinates();
    }
}

}  // namespace renderer
