#include "PointLight.h"

namespace renderer {
const RGB PointLightSource::kDefaultColor{1, 1, 1};
double PointLightSource::GetIntencity(double distance) const {
    return initial_intencity_ * (1 / (constant_attenuation_ + linear_attenuation_ * distance +
                                      quadratic_attenuation_ * distance * distance));
}
const RGB &PointLightSource::GetColor() const {
    return color_;
}
}  // namespace renderer
