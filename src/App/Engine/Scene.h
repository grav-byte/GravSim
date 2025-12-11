//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include <vector>

#include "Camera.h"
#include "SceneObject.h"


class Scene {
public:
    Scene();

    Camera* GetCamera() const;

    void CreateObject();

    void AddObject(std::unique_ptr<SceneObject> obj);

    std::vector<SceneObject*> GetAllObjects() const;

    void DeleteObject(uint32_t id);

    std::string* GetName();

    glm::vec2 globalGravity;
private:
    std::string name_;
    std::unique_ptr<Camera> camera_; // scene owns the camera
    std::vector<std::unique_ptr<SceneObject>> sceneObjects_; // scene owns objects
    uint32_t nextID_;

public:
    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(name_, camera_, globalGravity, sceneObjects_, nextID_);
    }
};
