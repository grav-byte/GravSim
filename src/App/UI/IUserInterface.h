#pragma once
#include "Core/Event.h"

class IUserInterface {
public:
    virtual ~IUserInterface() = default;
    virtual void OnEvent(Core::Event &event) = 0;
    virtual void Draw() = 0;
};
