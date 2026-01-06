#pragma once
#include <string>

class PIDData {
public:
    PIDData() = default; // Default constructor for deserialization

    PIDData(const float p, const float i, const float d) : pGain(p), iGain(i), dGain(d) {}

    float pGain = 0;
    float iGain = 0;
    float dGain = 0;
    std::string name;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(pGain, iGain, dGain, name);
    }
};
