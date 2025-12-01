//
// Created by kw on 01.12.2025.
//

#pragma once

class RenderingInterface {
public:
    virtual ~RenderingInterface() = default;

    virtual void Render() = 0;
};
