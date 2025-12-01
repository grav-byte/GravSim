//
// Created by kw on 01.12.2025.
//

#pragma once

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void Render() = 0;
};
