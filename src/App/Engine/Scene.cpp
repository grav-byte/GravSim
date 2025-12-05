//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "Scene.h"

Scene::Scene() {
    nextID_ = 0;
    sceneObjects_ = std::vector<std::unique_ptr<SceneObject>>();
    camera_ = std::make_unique<Camera>();
    sceneObjects_.push_back(std::make_unique<SceneObject>(nextID_++, "Circle"));
}

Camera* Scene::GetCamera() const { return camera_.get(); }

void Scene::CreateObject() {
    sceneObjects_.push_back(std::make_unique<SceneObject>(nextID_++, "Circle"));
}

void Scene::AddObject(std::unique_ptr<SceneObject> obj) {
    obj->id = nextID_++;
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

void Scene::DeleteObject(uint32_t id) {
    sceneObjects_.erase(
                std::remove_if(
                    sceneObjects_.begin(),
                    sceneObjects_.end(),
                    [id](const std::unique_ptr<SceneObject>& o) { return o->id == id; }
                ),
                sceneObjects_.end()
            );
}
