//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "EngineLayer.h"

EngineLayer::EngineLayer() : AppLayer() {
    audioSystem = std::make_shared<AudioSystem>();
    audioSystem->StartMusic();
}

EngineLayer::~EngineLayer() {
}

void EngineLayer::OnInit() {
}

void EngineLayer::OnUpdate(float deltaTime) {
}

void EngineLayer::OnEvent(Core::Event &event) {
}

void EngineLayer::OnRender() {
}
