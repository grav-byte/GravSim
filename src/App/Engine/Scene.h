//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include <vector>

#include "SceneObject.h"


class Scene {
    std::vector<std::unique_ptr<SceneObject>> sceneObjects;
};
