#include "Renderer.h"

namespace renderer {
void Renderer::Draw() {
}

void Renderer::DrawTriangle(const Triangle &current, const World::ObjectHolder &owner_object) {

    std::array<Vertex, 3> vertices = current.GetVertexArray();

    ShiftTriangleCoordinates(owner_object, &vertices);

    ShiftTriangleToAlignCamera(&vertices);// maybe needs to be camera's method

    world_.GetCamera().ApplyPerspectiveTransformation(&vertices);

    // TODO: rasterize triangle
}

void Renderer::ShiftTriangleCoordinates(const World::ObjectHolder &owner,
                                        std::array<Vertex, 3> *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix =
        Renderer::MakeHomogeneousTransformationMatrix(owner.GetAngle(), owner.GetCoordinates());
    Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, &(*vertices));
}

void Renderer::ShiftTriangleToAlignCamera(std::array<Vertex, 3> *vertices) {
    assert(vertices != nullptr);
    Matrix4d transformation_matrix = Renderer::MakeHomogeneousTransformationMatrix(
        world_.GetCameraRotation().inverse(), -world_.GetCameraPosition());

    Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, &(*vertices));
}

void Renderer::ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &transformation_matrix,
                                                    std::array<Vertex, 3> *vertices) {
    assert(vertices != nullptr);

    // Page 76 "Mathematics for 3D game..."
    for (auto &ver : *vertices) {
        ver.coordinates = transformation_matrix * ver.coordinates.GetHomogeneousCoordinates();
        ver.normal = transformation_matrix * ver.normal.GetHomogeneousCoordinates();
    }
}
Renderer::Renderer(const Screen &screen) : screen_(screen) {
}
Renderer::Matrix4d Renderer::MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                                 const Vector3d &offset) {

    Matrix4d transformation_matrix = Matrix4d::Zero();
    transformation_matrix.topLeftCorner<3, 3>() = rotation.toRotationMatrix();
    transformation_matrix(3, 3) = 1;
    transformation_matrix.col(3).topLeftCorner<3, 1>() = offset;
    return transformation_matrix;
}
}  // namespace renderer
