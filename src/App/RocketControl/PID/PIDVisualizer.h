#pragma once
#include <vector>

#include "App/Engine/SceneObject.h"

class PIDVisualizer {
public:
    static void PushArrow(std::vector<std::unique_ptr<SceneObject::DebugArrow>>& arrows,
                          const glm::vec2& origin,
                          const glm::vec2& dir,
                          const glm::vec4& color,
                          bool dashed = false,
                          bool hasArrow = true);

    static void DrawPIDArrows(std::vector<std::unique_ptr<SceneObject::DebugArrow>>& arrows,
                              const glm::vec2& origin,
                              const glm::vec2& dir,
                              const glm::vec3& terms,
                              float scale = 1.0f);

    static void DrawVerticalArrows(SceneObject *rocketObject, const glm::vec3 & terms);
    static void DrawHorizontalArrows(SceneObject *rocketObject, const glm::vec3 & terms,
                                     float targetAngle);
    static void DrawAttitudeArrows(SceneObject *rocketObject, const glm::vec3 & terms);
};
