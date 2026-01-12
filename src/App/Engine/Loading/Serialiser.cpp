//
// Created by Lorenz Saalmann on 06.12.25.
//

#include "CerealHelpers.h"
#include "Serialiser.h"

#include <fstream>
#include <filesystem>
#include "App/Rendering/Visuals/CircleVisual.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/Rendering/Visuals/SpriteVisual.h"
#include "App/Rendering/Visuals/IVisual.h"
#include "Core/Application.h"
#include "App/Engine//Physics/Colliders/CircleCollider.h"
#include "App/Engine/Physics/Colliders/ColliderBase.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>


#include "App/RocketControl/RocketObject.h"
#include "App/RocketControl/TargetObject.h"
#include "App/RocketControl/PID/PIDConfig.h"

CEREAL_REGISTER_TYPE(CircleVisual)

CEREAL_REGISTER_POLYMORPHIC_RELATION(IVisual, CircleVisual)

CEREAL_REGISTER_TYPE(SpriteVisual)
CEREAL_REGISTER_POLYMORPHIC_RELATION(IVisual, SpriteVisual)

CEREAL_REGISTER_TYPE(ShaderVisual)
CEREAL_REGISTER_POLYMORPHIC_RELATION(IVisual, ShaderVisual)

CEREAL_REGISTER_TYPE(CircleCollider)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ColliderBase, CircleCollider)

CEREAL_REGISTER_TYPE(RocketObject)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SceneObject, RocketObject)

CEREAL_REGISTER_TYPE(TargetObject)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SceneObject, TargetObject)

std::string Serialiser::sceneFolder = "scenes";

void Serialiser::EnsureSceneFolderExists() {
    const std::filesystem::path fullPath = Core::Application::GetAppDataFolder() / sceneFolder;
    std::filesystem::create_directories(fullPath);
}

template<typename T>
bool Serialiser::SaveObjInternal(T &obj, std::filesystem::path &filePath) {
    try {
        filePath += ".json";
        std::ofstream os(filePath);

        if (!os.is_open())
            return false;

        cereal::JSONOutputArchive archive(os);
        archive(obj);
        std::cout << "Saved object to " << filePath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to save object: " << e.what() << std::endl;
        return false;
    }
}

bool Serialiser::SaveScene(Scene &scene) {
    std::filesystem::path base = "../assets/";
    auto path = base / sceneFolder / *scene.GetName();
    return SaveObjInternal(scene, path);
}

bool Serialiser::SaveTempScene(Scene &scene) {
    auto path = Core::Application::GetAppDataFolder() / "tempScene";
    return SaveObjInternal(scene, path);
}

std::unique_ptr<Scene> Serialiser::LoadTempScene() {
    auto path = Core::Application::GetAppDataFolder() / "tempScene.json";
    return LoadScene(path.string());
}

std::unique_ptr<Scene> Serialiser::LoadScene(const std::string &filepath) {
    try {
        std::ifstream is(filepath);
        if (!is.is_open()) {
            std::cerr << "Failed to open scene file: " << filepath << std::endl;
            return nullptr;
        }

        cereal::JSONInputArchive archive(is);
        auto scene = std::make_unique<Scene>();
        archive (*scene);
        return scene;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load scene: " << e.what() << std::endl;
        return nullptr;
    }
}

bool Serialiser::SavePIDConfig(PIDConfig &pid, const std::filesystem::path &filePath) {
    std::filesystem::path path = filePath;
    return SaveObjInternal(pid, path);
}

std::unique_ptr<PIDConfig> Serialiser::LoadPIDConfig(const std::filesystem::path &filePath) {
    try {
        std::ifstream is(filePath);
        if (!is.is_open()) {
            std::cerr << "Failed to open PIDConfig file: " << filePath << std::endl;
            return nullptr;
        }

        cereal::JSONInputArchive archive(is);
        auto pid = std::make_unique<PIDConfig>();
        archive(*pid);
        return pid;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load PIDConfig: " << e.what() << std::endl;
        return nullptr;
    }
}