//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "Scene.h"

#include "EngineEvents.h"
#include "Core/Application.h"

Scene::Scene() {
    nextID_ = 0;
    sceneObjects_ = std::vector<std::unique_ptr<SceneObject>>();
    sceneConstraints_ = std::vector<std::unique_ptr<Constraint>>();
    globalGravity = glm::vec2(0.0f, 0.0f);
    name_ = std::string("New Scene");
    camera_ = std::make_unique<Camera>();

}

Camera* Scene::GetCamera() const { return camera_.get(); }

void Scene::CreateObject() {
    sceneObjects_.push_back(std::make_unique<SceneObject>(nextID_++, "Circle"));
}

void Scene::CreateObject(const glm::vec2& pos) {
    auto obj = std::make_unique<SceneObject>(nextID_++, "Circle");
    obj->transform.position = pos;
    obj->lastPosition = pos;
    sceneObjects_.push_back(std::move(obj));
}

void Scene::AddObject(std::unique_ptr<SceneObject> obj) {
    obj->id = nextID_++;
    sceneObjects_.push_back(std::move(obj));
}

void Scene::AddConstraint(std::unique_ptr<Constraint> constraint) {
    sceneConstraints_.push_back(std::move(constraint));
}

std::vector<SceneObject *> Scene::GetAllObjects() const {
    // return non-owning pointers to scene objects
    auto sceneObjectsPtrs = std::vector<SceneObject*>();
    for (const auto& obj : sceneObjects_) {
        sceneObjectsPtrs.push_back(obj.get());
    }
    return sceneObjectsPtrs;
}

std::vector<Constraint *> Scene::GetConstraints() const {
    // return non-owning pointers to constraints
    auto constraints = std::vector<Constraint*>();
    for (const auto& obj : sceneConstraints_) {
        constraints.push_back(obj.get());
    }
    return constraints;
}

void Scene::RemoveConstraint(Constraint::ConstraintDirection direction) {
    sceneConstraints_.erase(
               std::remove_if(
                   sceneConstraints_.begin(),
                   sceneConstraints_.end(),
                   [direction](const std::unique_ptr<Constraint>& c) {
                       const auto con = c.get();
                       return con->direction == direction;
                   }
               ),
               sceneConstraints_.end()
           );
}

void Scene::DeleteObject(uint32_t id) {
    auto event = ObjectDestroyedEvent(id);
    Core::Application::Get().RaiseEvent(event);

    sceneObjects_.erase(
                std::remove_if(
                    sceneObjects_.begin(),
                    sceneObjects_.end(),
                    [id](const std::unique_ptr<SceneObject>& o) { return o->id == id; }
                ),
                sceneObjects_.end()
            );
}

std::string* Scene::GetName() {
    return &name_;
}
