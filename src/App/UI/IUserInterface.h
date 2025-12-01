//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once

class IUserInterface {
public:
    virtual ~IUserInterface() = default;
    virtual void Render() = 0;
};