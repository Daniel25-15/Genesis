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
    float speed = 2.0;
    inline static Vector3 playerSpawnPosition = { 0.0f, 7.5f, 0.0f };
    int healthBarAmount = 10;
    Player(Vector3 pos, Vector3 s, Color c);
    BoundingBox GetBoundingBox() const;
    void Move (Vector3 deltaHorizontal, const std::vector<BoundingBox>& blocks);
    void Draw();
    void Wireframe();
    void Death();
};