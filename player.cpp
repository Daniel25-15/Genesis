#include "player.h"

Player::Player(Vector3 pos, Vector3 s, Color c) {
    position = pos;
    size = s;
    color = c;
}
BoundingBox Player::GetBoundingBox() const {
    return {
        { position.x - (size.x / 2), position.y - (size.y / 2), position.z - (size.z / 2) },
        { position.x + (size.x / 2), position.y + (size.y / 2), position.z + (size.z / 2) }
    };
}
void Player::Move (Vector3 deltaHorizontal, const std::vector<BoundingBox>& blocks) {
    position.x += deltaHorizontal.x;
    BoundingBox playerBox = GetBoundingBox();
    for (const auto& block : blocks) {
        if (CheckCollisionBoxes(playerBox, block)) {
            // position.x -= deltaHorizontal.x;
            // break;
            if (deltaHorizontal.x > 0.0f) {
                position.x = block.min.x - (size.x / 2.0f) - 0.001f;
            } else {
                position.x = block.max.x + (size.x / 2.0f) + 0.001f;
            }
            break;
        }
    }
    position.z += deltaHorizontal.z;
    playerBox = GetBoundingBox();
    for (const auto& block : blocks) {
        if (CheckCollisionBoxes(playerBox, block)) {
            // position.z -= deltaHorizontal.z;
            // break;
            if (deltaHorizontal.z > 0.0f) {
                position.z = block.min.z - (size.z / 2.0f) - 0.001f;
            } else {
                position.z = block.max.z + (size.z / 2.0f) + 0.001f;
            }
            break;
        }
    }
    velocityY -= 0.015f;
    position.y += velocityY;

    isOnGround = false;
    for (const auto& block : blocks) {
        BoundingBox playerBox = GetBoundingBox();
        if (CheckCollisionBoxes(playerBox, block)) {
            if (velocityY <= 0.0f) {
                float playerBottom = (position.y - velocityY) - (size.y / 2.0f);
                if (playerBottom <= block.max.y + 0.2f && playerBottom >= block.max.y - 0.5f) {
                    // bool xBounds = (position.x + (size.x / 2.0f) > block.min.x) && (position.x - (size.x / 2.0f) < block.max.x);
                    // bool zBounds = (position.z + (size.z / 2.0f) > block.min.z) && (position.z - (size.x / 2.0f) < block.max.z);

                    // if (xBounds && zBounds) {
                    position.y = block.max.y + (size.y / 2) + 0.001f;
                    velocityY = 0.0f;
                    isOnGround = true;
                    break;
                }
            }
            else if (velocityY > 0.0f) {
                position.y = block.min.y - (size.y / 2);
                velocityY = 0.0f;
                break;
            }
            playerBox = GetBoundingBox();
        }
        // break;
    }
}
void Player::Draw() {
    DrawCube(position, size.x, size.y, size.z, color);
}
void Player::Wireframe() {
    DrawCubeWires(position, size.x, size.y, size.z, BLACK);
}