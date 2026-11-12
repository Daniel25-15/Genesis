#include "follower.h"

Follower::Follower(Vector3 pos, Vector3 s, int h) {
    position = pos;
    size = s;
    healthAmount = h;
}
BoundingBox Follower::GetBoundingBox() const {
    return {
        { position.x - (size.x / 2), position.y - (size.y / 2), position.z - (size.z / 2) },
        { position.x + (size.x / 2), position.y + (size.y / 2), position.z + (size.z / 2) }
    };
}
void Follower::Move(Vector3 deltaHorizontal, const std::vector<BoundingBox>& blocks) {
    position.x += deltaHorizontal.x * speed * GetFrameTime();
    BoundingBox followerBox = GetBoundingBox();
    for (const auto& block : blocks) {
        if (CheckCollisionBoxes(followerBox, block)) {
            if (deltaHorizontal.x > 0.0f) {
                position.x = block.min.x - (size.x / 2.0f) - 0.001f;
            } else {
                position.x = block.max.x + (size.x / 2.0f) + 0.001f;
            }
            break;
        }
    }
    position.z += deltaHorizontal.z * speed * GetFrameTime();
    followerBox = GetBoundingBox();
    for (const auto& block : blocks) {
        if(CheckCollisionBoxes(followerBox, block)) {
            if (deltaHorizontal.z > 0.0f) {
                position.z = block.min.z - (size.z / 2.0f) - 0.001f;
            } else {
                position.z = block.max.z + (size.z / 2.0f) + 0.001f;
            }
            break;
        }
    }
    float gravity = 25.0f;
    velocityY -= gravity * GetFrameTime();
    position.y += velocityY * GetFrameTime();
    isOnGround = false;
    for (const auto& block : blocks) {
        BoundingBox followerBox = GetBoundingBox();
        if (CheckCollisionBoxes(followerBox, block)) {
            float followerBottom = followerBox.min.y;
            if (followerBottom <= block.max.y && (followerBottom - velocityY * GetFrameTime()) >= block.min.y - 0.5f) {
                    position.y = block.max.y + (size.y / 2) + 0.001f;
                    velocityY = 0.0f;
                    isOnGround = true;
                    break;
            } else if (velocityY > 0.0f) {
                position.y = block.min.y - (size.y / 2.0f) - 0.001f;
                velocityY = 0.0f;
                break;
            }
        }
        // followerBox = GetBoundingBox();
    }
}
void Follower::Draw(Model& model) {
    // DrawCube(position, size.x, size.y, size.z, BLACK);
    DrawModel(model, position, 0.3f, WHITE);
    DrawCubeWires(position, size.x, size.y, size.z, RED);
}