#pragma once
#include "raylib.h"
#include <vector>
#include "block.h"

class Player {
public:
    Vector3 position;
    Vector3 size;
    Color color;
    float velocityY = 0.0f;
    bool isOnGround = false;
    int currentItemNumber = 0;
    Player(Vector3 pos, Vector3 s, Color c);
    BoundingBox GetBoundingBox() const;
    void Move (Vector3 deltaHorizontal, const std::vector<BoundingBox>& blocks);
    void Draw();
    void Wireframe();
};