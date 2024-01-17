#include "Primitives.h"
namespace renderer {
Triangle::Triangle(const renderer::Triangle::Vector3i& points, const std::vector<Vertex>& parent) {
}
const renderer::Triangle::Vector3i renderer::Triangle::GetPoints() const {
    return points_;
}
}  // namespace renderer
