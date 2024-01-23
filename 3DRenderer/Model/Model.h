#pragma once
#include "Renderer.h"
namespace model {
class Model {
public:
    Model() = default;
    

private:
    renderer::Renderer renderer_;
    renderer::World world_;
};

};  // namespace model
