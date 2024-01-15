#pragma once

#include <Eigen/Dense>

#include "Utils.h"
#include "Vertex.h"
namespace renderer {
class Triangle {
    using Vector3i = Eigen::Vector3i;

public:
    Triangle(const Vector3i& points, const std::vector<Vertex>& parent);
    const Vector3i GetPoints();
    std::array<Vertex,3> GetVertexArray()const;
private:
    Vector3i points_;// array of indices of parent_vertex_array_
    const std::vector<Vertex>& parent_vertex_array_; // not sure about this
};
}  // namespace renderer
