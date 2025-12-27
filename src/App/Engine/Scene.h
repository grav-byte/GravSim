#pragma once
#include <vector>

#include "Camera.h"
#include "SceneObject.h"
#include "Physics/Constraint.h"


class Scene {
public:
    Scene();

    Camera* GetCamera() const;

    void CreateObject();
    void CreateObject(const glm::vec2& pos);

    void AddObject(std::unique_ptr<SceneObject> obj);

    void AddConstraint(std::unique_ptr<Constraint> constraint);

    std::vector<SceneObject*> GetAllObjects() const;
    std::vector<Constraint*> GetConstraints() const;
    void RemoveConstraint(Constraint::ConstraintDirection direction);

    void DeleteObject(uint32_t id);

    std::string* GetName();


    glm::vec2 globalGravity{};

private:
    std::string name_;
    std::unique_ptr<Camera> camera_; // scene owns the camera
    std::vector<std::unique_ptr<SceneObject>> sceneObjects_; // scene owns objects
    std::vector<std::unique_ptr<Constraint>> sceneConstraints_;
    uint32_t nextID_;

public:
    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(name_, camera_, globalGravity, sceneObjects_, sceneConstraints_, nextID_);
    }
};
