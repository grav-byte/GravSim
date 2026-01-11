#include "PIDVisualizer.h"
#include <memory>
#include <glm/glm.hpp>


void PIDVisualizer::PushArrow(std::vector<std::unique_ptr<SceneObject::DebugArrow> > &arrows,
                              const glm::vec2 &origin,
                              const glm::vec2 &dir,
                              const glm::vec4 &color,
                              const bool dashed,
                              const bool hasArrow) {
    const float l = glm::length(dir);
    // normalize and clamp length
    auto dirClamped = l > 0.0f ? dir / l * glm::clamp(l, .1f, 4.0f) : glm::vec2(0.0f);
    auto arrow = std::make_unique<SceneObject::DebugArrow>(origin, dirClamped, color);
    arrow->dashed = dashed;
    arrow->hasArrow = hasArrow;
    arrows.push_back(std::move(arrow));
}

void PIDVisualizer::DrawPIDArrows(std::vector<std::unique_ptr<SceneObject::DebugArrow> > &arrows,
                   const glm::vec2 &origin,
                   const glm::vec2 &dir,
                   const glm::vec4 &terms,
                   const float scale) {
    PushArrow(arrows, origin, dir * terms.x * scale, glm::vec4(0, 0, 1, 1)); // P = Blue
    PushArrow(arrows, origin, dir * terms.y * scale, glm::vec4(1, 0, 0, 1)); // I = Red
    PushArrow(arrows, origin, dir * terms.z * scale, glm::vec4(0, 1, 0, 1)); // D = Green
    PushArrow(arrows, origin, dir * terms.w * scale, glm::vec4(0, .4, .5, 1)); // Bias = Cyan
}

void PIDVisualizer::DrawVerticalArrows(SceneObject *rocketObject, const glm::vec4 &terms) {
    const glm::vec2 yDir = rocketObject->transform.GetMatrix() * glm::vec4(0, 1, 0, 0);
    const glm::vec2 origin = rocketObject->transform.position;
    DrawPIDArrows(rocketObject->debugArrows, origin, yDir, terms, 3.0f);
}

void PIDVisualizer::DrawHorizontalArrows(SceneObject *rocketObject, const glm::vec4 &terms, float targetAngle) {
    const glm::vec2 xDir = rocketObject->transform.GetMatrix() * glm::vec4(1, 0, 0, 0);

    const glm::vec2 origin = rocketObject->transform.position;

    // P/I/D arrows3
    DrawPIDArrows(rocketObject->debugArrows, origin, xDir, terms, 3.0f);

    // Target direction line
    const float absoluteAngle = glm::radians(-targetAngle);
    const glm::vec2 targetDir = glm::vec2(sin(absoluteAngle), cos(absoluteAngle));
    PushArrow(rocketObject->debugArrows, origin, targetDir * 3.0f, glm::vec4(1, 1, 1, 1), true, false);
}

void PIDVisualizer::DrawAttitudeArrows(SceneObject *rocketObject, const glm::vec4 &terms) {
    const glm::vec2 xDir = rocketObject->transform.GetMatrix() * glm::vec4(1, 0, 0, 0);
    const glm::vec2 yDir = rocketObject->transform.GetMatrix() * glm::vec4(0, 1, 0, 0);
    const glm::vec2 origin = rocketObject->transform.position + yDir * -1.0f;

    // P/I/D arrows along x-direction
    DrawPIDArrows(rocketObject->debugArrows, origin, xDir, terms, 1.0f);
}
