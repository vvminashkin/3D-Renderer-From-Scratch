#pragma once

#include <Eigen/Dense>
#include "World.h"

namespace renderer {
    class Renderer {
    public:
        Renderer(size_t width, size_t height);

        void Draw();
        void DrawTriangle(const Triangle&,const World::ObjectContainer&);

    private:
        void ShiftTriangleCoordinates(const World::ObjectContainer & owner, std::array<Vertex,3>*);
        World world_;
        Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> screen_;
        size_t height_;
        size_t width_;


        void ShiftTriangleToAlignCamera(std::array<Vertex, 3> *);
        static void ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d&, std::array<Vertex,3>*);
    };
}