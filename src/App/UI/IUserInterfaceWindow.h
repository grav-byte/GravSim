//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once

class IUserInterfaceWindow {
public:
    virtual ~IUserInterfaceWindow() = default;
    virtual void Render() = 0;
};