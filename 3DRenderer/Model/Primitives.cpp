#include "Primitives.h"
namespace renderer {
Triangle::Triangle(const renderer::Triangle::Vector3i& points, const std::vector<Vertex>& parent)
    : points_(points), parent_vertex_array_(parent) {
}
const renderer::Triangle::Vector3i renderer::Triangle::GetPoints() {
    return points_;
}
std::array<Vertex, 3> renderer::Triangle::GetVertexArray() const {
    std::array<Vertex, 3> ans;
    for (size_t i = 0; i < 3; ++i) {
        ans[i] = parent_vertex_array_[points_[i]];  // TODO get triangle
    }
    return ans;
}
}  // namespace renderer
