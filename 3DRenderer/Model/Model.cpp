#include "Model.h"
#include "Primitives.h"
#include "Screen.h"
#include <algorithm>
#include <memory>
namespace model {
Model::Model(int width, int height)
    : width_(width), height_(height), update_port_([this]() { return GetCurrentScreen(); }) {
}

std::optional<std::shared_ptr<const renderer::Screen>> Model::GetCurrentScreen() {
    return current_screen_;
}

void Model::Subscribe(observer::CObserver<std::shared_ptr<const renderer::Screen>> *obs) {
    update_port_.subscribe(obs);
}
void Model::TestUpdate() {
    std::unique_ptr<renderer::Screen> screen = std::make_unique<renderer::Screen>(width_, height_);
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            screen->SetPixel(i, j, test_colors_[test_current_index_]);
        }
    }
    Eigen::Matrix3d tr;
    tr << -0.5, 0.5, 0, -0.5, 0, 0, 0.5, 0, 0;
    renderer::Triangle triangle(tr);
    renderer::BarycentricCoordinateSystem syst(triangle, triangle);
    renderer_.RasteriseTriangle(syst, tr, &(*screen));
    ++test_current_index_;
    test_current_index_ = test_current_index_ % 2;
    current_screen_.reset(screen.release());
    update_port_.notify();
}
}  // namespace model
