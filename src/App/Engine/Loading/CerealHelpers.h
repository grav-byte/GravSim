#pragma once
#include <glm/glm.hpp>

namespace cereal {
    template<class Archive>
    void serialize(Archive& ar, glm::vec2& v) { ar(v.x, v.y); }

    template<class Archive>
    void serialize(Archive& ar, glm::vec3& v) { ar(v.x, v.y, v.z); }

    template<class Archive>
    void serialize(Archive& ar, glm::vec4& v) { ar(v.x, v.y, v.z, v.w); }
}