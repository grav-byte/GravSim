//
// Created by Lorenz Saalmann on 06.12.25.
//

#pragma once
#include "Scene.h"


class SceneLoader {
public:
    static std::string sceneFolder;

    static void EnsureSceneFolderExists();

    static bool SaveScene(Scene &scene);
    static std::unique_ptr<Scene> LoadScene(const std::string& filepath);
};
