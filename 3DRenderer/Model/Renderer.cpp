#include "Renderer.h"

namespace renderer {
Screen Renderer::Draw(const World &world, size_t width, size_t height) {
    Screen screen(width, height);
    for (const auto &object : world.GetObjectsIterable()) {
        for (const auto &triangle : object->GetMesh().GetTriangles()) {
            DrawTriangle(triangle, object, world, screen);
        }
    }
    return screen;
}

void Renderer::ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix =
        Renderer::MakeHomogeneousTransformationMatrix(owner.GetAngle(), owner.GetCoordinates());
    Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, &(*vertices));
}

void Renderer::ShiftTriangleToAlignCamera(const World &world, Triangle *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix = Renderer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(), -world.GetCameraPosition());

    Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, vertices);
}

void Renderer::DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                            const World &world, Screen &screen) {

    Triangle vertices = owner_object->GetMesh().GetTriangleVertices(current);

    ShiftTriangleCoordinates(owner_object, &vertices);

    ShiftTriangleToAlignCamera(world, &vertices);

    Triangle camera_space_triangle = world.GetCamera().ApplyPerspectiveTransformation(vertices);

    // TODO: rasterize triangle
}
Renderer::Matrix4d Renderer::MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                                 const Vector3d &offset) {

    Matrix4d transformation_matrix = Matrix4d::Zero();
    transformation_matrix.topLeftCorner<3, 3>() = rotation.toRotationMatrix();
    transformation_matrix(3, 3) = 1;
    transformation_matrix.col(3).topLeftCorner<3, 1>() = offset;
    return transformation_matrix;
}
void Renderer::ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &transformation_matrix,
                                                    Triangle *vertices) {
    assert(vertices != nullptr);

    // Page 76 "Mathematics for 3D game..."
    for (auto &ver : vertices->verticies_) {
        ver.coordinates = transformation_matrix * ver.coordinates.GetHomogeneousCoordinates();
        ver.normal = transformation_matrix * ver.normal.GetHomogeneousCoordinates();
    }
}
}  // namespace renderer
