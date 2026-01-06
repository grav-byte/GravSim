#pragma once

class PIDData {
public:
    PIDData() = default; // Default constructor for deserialization

    PIDData(const float p, const float i, const float d): pGain(p), iGain(i), dGain(d) {}

    float pGain;
    float iGain;
    float dGain;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(pGain, iGain, dGain);
    }
};
