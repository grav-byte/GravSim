//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "Scene.h"

Scene::Scene() {
    sceneObjects_ = std::vector<std::unique_ptr<SceneObject>>();
    camera_ = std::make_unique<Camera>();
    sceneObjects_.push_back(std::make_unique<SceneObject>());
}

const Camera & Scene::GetCamera() const { return *camera_; }

void Scene::AddObject(std::unique_ptr<SceneObject> obj) {
    sceneObjects_.push_back(std::move(obj));
}

std::vector<SceneObject *> Scene::GetAllObjects() const {
    // return non-owning pointers to scene objects
    auto sceneObjectsPtrs = std::vector<SceneObject*>();
    for (const auto& obj : sceneObjects_) {
        sceneObjectsPtrs.push_back(obj.get());
    }
    return sceneObjectsPtrs;
}
