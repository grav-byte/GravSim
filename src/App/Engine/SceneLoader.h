//
// Created by Lorenz Saalmann on 06.12.25.
//

#pragma once
#include "Scene.h"


class SceneLoader {
public:
    static bool SaveScene(const Scene& scene, const std::string& filepath);
    static std::unique_ptr<Scene> LoadScene(const std::string& filepath);
};
