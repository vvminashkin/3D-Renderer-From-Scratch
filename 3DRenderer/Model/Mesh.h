#pragma once
#include <Eigen/Dense>
#include <vector>
#include "Primitives.h"
#include "Vertex.h"
namespace renderer {
    class Mesh {
    public:
       const std::vector<Vertex>& GetVertices()const;
        const std::vector<Triangle>& GetTriangles()const;
    private:
        std::vector<Vertex> vertices_;
        std::vector<Triangle> triangles_;
    };
}