//
// Created by Lorenz Saalmann on 06.12.25.
//

#include "CerealHelpers.h"
#include "SceneLoader.h"

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
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>

#include "App/RocketControl/RocketObject.h"

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

std::string SceneLoader::sceneFolder = "scenes";

void SceneLoader::EnsureSceneFolderExists() {
    const std::filesystem::path fullPath = Core::Application::GetAppDataFolder() / sceneFolder;
    std::filesystem::create_directories(fullPath);
}

bool SceneLoader::SaveSceneInternal(Scene &scene, std::filesystem::path &filePath) {
    try {
        filePath += ".json";
        std::ofstream os(filePath);

        if (!os.is_open())
            return false;

        cereal::JSONOutputArchive archive(os);
        archive (scene);
        std::cout << "Saved scene to " << filePath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to save scene: " << e.what() << std::endl;
        return false;
    }
}

bool SceneLoader::SaveScene(Scene &scene) {
    std::filesystem::path base = "../assets/";
    auto path = base / sceneFolder / *scene.GetName();
    return SaveSceneInternal(scene, path);
}

bool SceneLoader::SaveTempScene(Scene &scene) {
    auto path = Core::Application::GetAppDataFolder() / "tempScene";
    return SaveSceneInternal(scene, path);
}

std::unique_ptr<Scene> SceneLoader::LoadTempScene() {
    auto path = Core::Application::GetAppDataFolder() / "tempScene.json";
    return LoadScene(path.string());
}

std::unique_ptr<Scene> SceneLoader::LoadScene(const std::string &filepath) {
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