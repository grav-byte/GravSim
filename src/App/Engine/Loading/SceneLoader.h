#pragma once
#include <filesystem>

#include "App/Engine/Scene.h"


class SceneLoader {
public:
    static std::string sceneFolder;

    static void EnsureSceneFolderExists();

    static bool SaveScene(Scene &scene);
    static bool SaveTempScene(Scene &scene);

    static std::unique_ptr<Scene> LoadTempScene();

    static std::unique_ptr<Scene> LoadScene(const std::string& filepath);
private:
    static bool SaveSceneInternal(Scene &scene, std::filesystem::path &filePath);
};
