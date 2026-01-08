#pragma once
#include <filesystem>

#include "App/Engine/Scene.h"
#include "App/RocketControl/PID/PIDConfig.h"
#include "App/RocketControl/PID/PIDData.h"


class Serialiser {
public:
    static std::string sceneFolder;

    static void EnsureSceneFolderExists();

    static bool SaveScene(Scene &scene);
    static std::unique_ptr<Scene> LoadScene(const std::string& filepath);

    static bool SaveTempScene(Scene &scene);
    static std::unique_ptr<Scene> LoadTempScene();

    static bool SavePIDConfig(PIDConfig &pid, const std::filesystem::path &filePath);
    static std::unique_ptr<PIDConfig> LoadPIDConfig(const std::filesystem::path &filePath);

private:
    template<typename T>
    static bool SaveObjInternal(T &obj, std::filesystem::path &filePath);
};
