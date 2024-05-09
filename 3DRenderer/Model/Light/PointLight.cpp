#include "PointLight.h"

namespace renderer {
const RGB PointLightSource::kDefaultColor{1, 1, 1};
double PointLightSource::GetIntencity(double distance) const {
    return initial_intencity_ * (1 / (constant_attenuation_ + linear_attenuation_ * distance +
                                      quadratic_attenuation_ * distance * distance));
}
RGB PointLightSource::GetColor() const {
    return color_ * is_on_;
}
void PointLightSource::SetParameters(double c_attenuation, double l_attenuation,
                                     double q_attenuation) {
    constant_attenuation_ = c_attenuation;
    linear_attenuation_ = l_attenuation;
    quadratic_attenuation_ = q_attenuation;
}
void PointLightSource::Toggle() {
    is_on_ = !is_on_;
}
}  // namespace renderer
