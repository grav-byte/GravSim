#pragma once

#include <array>
#include "PIDData.h"

class PIDConfig {
public:
    PIDConfig() = default;

    explicit PIDConfig(const std::array<PIDData,3>& data) : pidData_(data) {}

    const PIDData& GetPIDData(const int index) const {
        return pidData_.at(index);
    }

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(pidData_);
    }

private:
    std::array<PIDData, 3> pidData_;
};
