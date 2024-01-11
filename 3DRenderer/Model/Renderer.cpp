#include "Renderer.h"

namespace renderer {
    void Renderer::Draw() {
    }

    Renderer::Renderer(size_t width, size_t height) {
        screen_.resize(height, width);
        width_ = width;
        height_ = height;

    }

    void Renderer::DrawTriangle(const Triangle &current, const World::ObjectContainer &owner_object) {

        std::array<Vertex, 3> vertices;
        for (size_t i = 0; i < 3; ++i) {
            vertices[i] = owner_object.object.GetMesh().GetVertices()[current.points[i]];
        }
        ShiftTriangleCoordinates(owner_object, &vertices);

        ShiftTriangleToAlignCamera(&vertices);


    }

    void Renderer::ShiftTriangleCoordinates(const World::ObjectContainer &owner, std::array<Vertex, 3> *vertices) {
        Eigen::Matrix4d transformation_matrix = Eigen::Matrix4d::Zero();
        transformation_matrix.topLeftCorner<3, 3>() = owner.global_rotation_.toRotationMatrix();
        transformation_matrix(3, 3) = 1;
        transformation_matrix.col(3).topLeftCorner<3, 1>() = owner.global_coordinates;

        Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, &(*vertices));


    }

    void Renderer::ShiftTriangleToAlignCamera(std::array<Vertex, 3> *vertices) {
        Eigen::Matrix4d transformation_matrix = Eigen::Matrix4d::Zero();
        transformation_matrix.topLeftCorner<3, 3>() = world_.GetCameraRotation().inverse().toRotationMatrix();
        transformation_matrix(3, 3) = 1;
        transformation_matrix.col(3).topLeftCorner<3, 1>() = world_.GetCameraPosition();

        Renderer::ApplyHomogeneousTransformationMatrix(transformation_matrix, &(*vertices));
    }

    void Renderer::ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &transformation_matrix,
                                                        std::array<Vertex, 3> *vertices) {

        //Page 76 "Mathematics for 3D game..."
        for (auto &ver: *vertices) {
            Eigen::Vector4d temp = Eigen::Vector4d::Zero();

            temp.topLeftCorner<3, 1>() = ver.coordinates;
            temp.w() = 1; //because it is a point
            temp = transformation_matrix * temp;
            ver.coordinates = temp.topLeftCorner<3, 1>();
            temp = Eigen::Vector4d::Zero();

            temp.topLeftCorner<3, 1>() = ver.normal;
            temp.w() = 0;//because it is a direction
            temp = transformation_matrix * temp;
            ver.normal = temp.topLeftCorner<3, 1>();
        }
    }
}