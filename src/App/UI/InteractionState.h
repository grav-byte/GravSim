//
// Created by kw on 20.12.2025.
//

#pragma once

struct InteractionState {
    bool clickToPlaceEnabled = true;

    glm::vec2 mouseScreen = {0,0};
    ImVec2 sceneMinScreen = ImVec2(0,0);
    ImVec2 sceneSize      = ImVec2(0,0);
    bool   sceneHovered   = false;

    // bool deleteMode = false;
    // bool selectMode = true;
};
