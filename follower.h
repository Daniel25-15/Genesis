#pragma once
#include "raylib.h"
#include <vector>

class Follower {
public: 
    Vector3 position;
    Vector3 size;
    float velocityY = 0.0f;
    bool isOnGround = false;
    int healthAmount;
    float speed = 1.85;
    float attackCooldown = 0.0f;
    const float attackCooldownDuration = 1.0f;
    Follower(Vector3 pos, Vector3 s, int h);
    BoundingBox GetBoundingBox() const;
    void Move(Vector3 deltaHorizontal, const std::vector<BoundingBox>& blocks);
    void Draw(Model& model);
    void attack();
};