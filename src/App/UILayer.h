//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include "Core/AppLayer.h"


class UILayer: public Core::AppLayer {
public:
    ~UILayer();

    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnEvent(Core::Event &event) override;
    void OnRender() override;
private:
    bool m_ShowDemo = false;
};
