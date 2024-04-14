#pragma once
#include "../Utils.h"

namespace renderer {

class PointLightSource {
public:
    // TODO calculate intencity
    double GetIntencity(double distance) const;
    const RGB &GetColor() const;

private:
    static constexpr double kDefaultIntencity = 0.9;
    static constexpr double kDefaultConstantAttenuation = 1;
    static constexpr double kDefaultLinearAttenuation = 0.5;
    static constexpr double kDefaultQuadraticAttenuation = 0.3;
    static const RGB kDefaultColor;
    double initial_intencity_ = kDefaultIntencity;
    double constant_attenuation_ = kDefaultConstantAttenuation;
    double linear_attenuation_ = kDefaultLinearAttenuation;
    double quadratic_attenuation_ = kDefaultQuadraticAttenuation;
    RGB color_ = kDefaultColor;
};
}  // namespace renderer
