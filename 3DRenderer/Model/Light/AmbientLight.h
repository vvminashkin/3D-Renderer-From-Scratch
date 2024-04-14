#pragma once
#include "../Primitives.h"

namespace renderer {
class AmbientLightSource {
public:
    double GetIntencity() const {
        return intencity_;
    }

private:
    static constexpr double kDefaultIntenstity = 0.3;
    static const RGB kDefaultColor;
    double intencity_ = kDefaultIntenstity;
    RGB color_ = kDefaultColor;
};
}  // namespace renderer
