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

void Renderer::SwitchLightModel() {
    use_blinn_phong_ = !use_blinn_phong_;
}
void Renderer::ShiftLightToAlignCamera(const World &world, LightSourcesDescription *desc) {
    Matrix34d transformation_matrix = Renderer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(),
        world.GetCameraRotation().inverse() * -world.GetCameraPosition());
    for (int i = 0; i < world.GetPointLightSources().size(); ++i) {
        Vector4d temp = Vector4d::Ones();
        temp.topLeftCorner<3, 1>() = world.GetPointLightSources()[i].GetCoordinates();
        desc->point_light_coordinates_.push_back(transformation_matrix * temp);
        // std::cout<<(transformation_matrix*temp).transpose()<<'b'<<i<<std::endl;
    }
}
void Renderer::ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix34d transformation_matrix =
        Renderer::MakeHomogeneousTransformationMatrix(owner.GetAngle(), owner.GetCoordinates());
    Renderer::ApplyMatrix(transformation_matrix, &(*vertices));
}

void Renderer::ShiftTriangleToAlignCamera(const World &world, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix34d transformation_matrix = Renderer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(),
        world.GetCameraRotation().inverse() * -world.GetCameraPosition());
    // std::cout<<world.GetCameraRotation()<<std::endl;
    // std::cout<<vertices->GetVerticiesCoordinates()<<std::endl;
    Renderer::ApplyMatrix(transformation_matrix, vertices);
    // std::cout<<vertices->GetVerticiesCoordinates().row(0)<<std::endl;
    //  std::cout<<vertices->GetVerticiesCoordinates()<<std::endl;
}

std::unique_ptr<Screen> Renderer::Draw(const World &world, size_t width, size_t height) {
    std::unique_ptr<Screen> screen(new Screen(width, height));
    LightSourcesDescription light_desc;
    ShiftLightToAlignCamera(world, &light_desc);
    for (const auto &object : world.GetObjectsIterable()) {
        for (const auto &triangle : object->GetMesh().GetTriangles()) {
            DrawTriangle(triangle, object, world, light_desc, screen.get());
        }
    }
    return screen;
}
void Renderer::DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                            const World &world, const LightSourcesDescription &light_desc,
                            Screen *screen) {
    Triangle vertices = owner_object->GetMesh().MakeTriangleVertices(current);
    ShiftTriangleCoordinates(owner_object, &vertices);

    ShiftTriangleToAlignCamera(world, &vertices);
    Eigen::Matrix<double, 3, 4> transformed_vertices =
        world.GetCamera().ApplyPerspectiveTransformation(
            vertices.GetVerticesHomogeniousCoordinates());
    vertices.CalculateNorm();
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
        RasterizeTriangle(system, homogeneous_coords.topLeftCorner<3, 3>(), world, light_desc,
                          screen);
    }
    std::cout.flush();
    // RasterizeTriangle(system, system.GetTriangle(), &screen);
}
void Renderer::RasterizeTriangle(const BarycentricCoordinateSystem &system,
                                 const Matrix3d &coordinates, const World &world,
                                 const LightSourcesDescription &desc, Screen *screen) {
    size_t height = screen->GetHeight();
    size_t width = screen->GetWidth();
    Matrix3d screen_triangle = TransformToScreenSpace(coordinates, width, height);
    Matrix2d inverse_matrix =
        BarycentricCoordinateSystem::MakeBarycentricTransformationMatrix(coordinates);

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

    Vector3d inv_z_coords = system.GetOriginalCoordinatesMatrix().col(2).cwiseInverse();
    for (size_t x = min_x; x <= max_x; ++x) {
        for (size_t y = min_y; y <= max_y; ++y) {
            Vector2d point_screen_space = {static_cast<double>(x) + 0.5,
                                           static_cast<double>(y) + 0.5};
            Vector2d point_camera_space =
                TransformVectorToCameraSpace(point_screen_space, width, height);
            Vector3d b_local_coordinates = BarycentricCoordinateSystem::TransformToBarycentric(
                inverse_matrix, coordinates, point_camera_space);
            if (!CheckIfInside(b_local_coordinates)) {
                continue;
            }
            Vector3d b_coordinate = system.ConvertToBarycentricCoordinates(point_camera_space);
            double z = system.GetTransformedCoordinates(b_coordinate).z();
            // double z = system.GetOriginalCoordinates(b_coordinate).norm();
            if (screen->GetZ(y, x) != 0 && z >= screen->GetZ(y, x)) {
                continue;
            }
            screen->SetZ(y, x, z);
            Vector3d new_b_coordinate = b_coordinate;
            double real_z_inv = (b_coordinate.dot(inv_z_coords));
            new_b_coordinate = (1 / real_z_inv) * new_b_coordinate.cwiseProduct(inv_z_coords);
            if (use_blinn_phong_) {

                screen->SetPixel(
                    y, x,
                    CalculateBlinnPhong(
                        system.GetOriginalTriangle().GetDiffuseColor(new_b_coordinate),
                        system.GetOriginalTriangle().GetSpecularColor(new_b_coordinate), desc,
                        world, new_b_coordinate, system.GetOriginalTriangle()));
            } else {
                RGB color = system.GetColor(new_b_coordinate);
                screen->SetPixel(y, x, color);
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
RGB Renderer::CalculateBlinnPhong(const RGB &diffuse_color, const RGB &specular_color,
                                  const LightSourcesDescription &desc, const World &world,
                                  const Vector3d &b_coordinates, const Triangle &triangle) {
    RGB ans{0, 0, 0};
    Vector3d position = b_coordinates.transpose() * triangle.GetVerticiesCoordinates();
    Vector3d normal = triangle.GetNormal(b_coordinates);
    RGB diffusion = CalculateBlinnPhongDiffusion(diffuse_color, desc, world, position, normal);
    RGB specular = CalculateBlinnPhongSpecular(specular_color, desc, world, position, normal);
    return diffusion + specular;
}
RGB Renderer::CalculateBlinnPhongDiffusion(const RGB &diffuse_color,
                                           const LightSourcesDescription &desc, const World &world,
                                           const Vector3d &coordinates, const Vector3d &normal) {
    RGB ans = {0, 0, 0};
    for (const auto &light : world.GetAmbientLightSources()) {
        ans += diffuse_color * light.GetIntencity();
    }
    for (int i = 0; i < world.GetPointLightSources().size(); ++i) {
        Vector3d direction = (desc.point_light_coordinates_[i] - coordinates).eval().normalized();
        if ((-1 + 2 * std::signbit(normal.dot(-coordinates))) *
                (-1 + 2 * std::signbit(normal.dot(direction))) <
            0) {
            continue;
        }
        const auto &light = world.GetPointLightSources()[i];
        ans += light.GetColor() *
               (light.GetIntencity((coordinates - desc.point_light_coordinates_[i]).norm()) *
                std::abs(direction.dot(normal)));
    }
    ans.normalize();
    ans *= diffuse_color;

    return ans;
}
RGB Renderer::CalculateBlinnPhongSpecular(const RGB &initial_color,
                                          const LightSourcesDescription &desc, const World &world,
                                          const Vector3d &coordinates, const Vector3d &normal) {
    RGB ans = {0, 0, 0};
    constexpr int kM = 25;
    for (int i = 0; i < world.GetPointLightSources().size(); ++i) {
        Vector3d direction = (desc.point_light_coordinates_[i] - coordinates).eval().normalized();
        if ((-1 + 2 * std::signbit(normal.dot(-coordinates))) *
                (-1 + 2 * std::signbit(normal.dot(direction))) <
            0) {
            continue;
        }
        Vector3d viewer = (-coordinates).normalized();
        Vector3d halfway = (direction + viewer) / (direction + viewer).norm();
        const auto &light = world.GetPointLightSources()[i];
        ans += light.GetColor() *
               (light.GetIntencity((coordinates - desc.point_light_coordinates_[i]).norm()) *
                std::pow(std::abs(halfway.dot(normal)), kM));
    }
    ans.normalize();
    ans *= initial_color;

    return ans;
}
Renderer::Matrix34d Renderer::MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                                  const Vector3d &offset) {

    Matrix34d transformation_matrix = Matrix34d::Zero();
    transformation_matrix.topLeftCorner<3, 3>() = rotation.toRotationMatrix();
    // transformation_matrix(3, 3) = 1;
    transformation_matrix.col(3).topLeftCorner<3, 1>() = offset;
    return transformation_matrix;
}
void Renderer::ApplyMatrix(const Matrix34d &transformation_matrix, Triangle *vertices) {
    assert(vertices != nullptr);

    // Page 76 "Mathematics for 3D game..."
    for (auto &ver : vertices->GetVerticies()) {
        ver.coordinates = (transformation_matrix * ver.coordinates.GetHomogeneousCoordinates());
        ver.normal = transformation_matrix * ver.normal.GetHomogeneousCoordinates();
    }
}

}  // namespace renderer
