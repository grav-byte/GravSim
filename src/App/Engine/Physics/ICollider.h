//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once


class ICollider {
public:
    virtual ~ICollider() = default;
    virtual bool CheckCollision(const ICollider& other);
};