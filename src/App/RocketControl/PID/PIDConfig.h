#pragma once

#include <array>
#include "PIDData.h"

// config is used for saving/loading PID parameters for all 3 controllers
class PIDConfig {
public:
    PIDConfig() = default;

    explicit PIDConfig(const std::array<PIDData,3>& data, const float steeringAngle)
            : pidData_(data), steeringAngle_(steeringAngle) {}

    const PIDData& GetPIDData(const int index) const { return pidData_.at(index); }
    float GetSteeringAngle() const { return steeringAngle_; }

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(pidData_, steeringAngle_);
    }


private:
    std::array<PIDData, 3> pidData_;
    float steeringAngle_ = 0.0f;
};
