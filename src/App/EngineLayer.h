//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "AudioSystem.h"
#include "Core/AppLayer.h"


class EngineLayer : public Core::AppLayer{
public:
    EngineLayer();
    ~EngineLayer() override;

    std::shared_ptr<AudioSystem> audioSystem;

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

};
