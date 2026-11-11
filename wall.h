#pragma once
#include "raylib.h"
#include <vector>
#include "block.h"


class Wall {
public:
    Vector3 originPosition;
    float blockSize;
    int width;
    int length;
    int height;
    int id;
    BoundingBox bounds;
    std::vector<Block> blocks;
    Wall(Vector3 pos, float size, int w, int l, int h, int identifier);
    void Draw(Model& cubeModel, Model& metalModel, Model& metalGrate, Model& wallModel, Model& windowModel);
    BoundingBox GetBoundingBox() const { return bounds; }
};
