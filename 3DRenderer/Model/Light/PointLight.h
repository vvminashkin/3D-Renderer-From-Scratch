#pragma once
#include "../Utils.h"

namespace renderer {

class PointLightSource {
public:
    double GetIntencity(double distance) const;
    RGB GetColor() const;
    void SetParameters(double c_attenuation, double l_attenuation, double q_attenuation);
    void Toggle();

private:
    static constexpr double kDefaultIntencity = 1;
    static constexpr double kDefaultConstantAttenuation = 1;
    static constexpr double kDefaultLinearAttenuation = 0.4;
    static constexpr double kDefaultQuadraticAttenuation = 0.02;
    static const RGB kDefaultColor;
    double initial_intencity_ = kDefaultIntencity;
    double constant_attenuation_ = kDefaultConstantAttenuation;
    double linear_attenuation_ = kDefaultLinearAttenuation;
    double quadratic_attenuation_ = kDefaultQuadraticAttenuation;
    RGB color_ = kDefaultColor;
    bool is_on_ = true;
};
}  // namespace renderer
