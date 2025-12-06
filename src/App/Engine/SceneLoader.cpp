//
// Created by Lorenz Saalmann on 06.12.25.
//

#include "SceneLoader.h"

#include <fstream>
#include "../Rendering/CircleRenderer.h"
#include "../Rendering/IRenderer.h"
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

CEREAL_REGISTER_TYPE(CircleRenderer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(IRenderer, CircleRenderer)

namespace cereal {
    template <class Archive>
    void serialize(Archive& ar, glm::vec2& v) { ar(v.x, v.y); }
    template<class Archive>
    void serialize(Archive& ar, glm::vec4& v) { ar(v.x, v.y, v.z, v.w); }
}

bool SceneLoader::SaveScene(const Scene &scene, const std::string &filepath) {
    try {
        std::ofstream os(filepath);
        if (!os.is_open()) {
            return false;
        }

        cereal::JSONOutputArchive archive(os);
        archive (scene);
        std::cout << "Saved scene to " << std::filesystem::current_path() << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to save scene: " << e.what() << std::endl;
        return false;
    }
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
