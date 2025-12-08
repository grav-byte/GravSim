//
// Created by Lorenz Saalmann on 08.12.25.
//

#pragma once
#include "IUserInterface.h"


class SimulationUI : public IUserInterface {
public:
    void OnEvent(Core::Event &event) override;

    void Draw() override;
};
