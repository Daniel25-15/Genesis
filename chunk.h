#pragma once
#include "raylib.h"
#include <vector>
#include "block.h"


class chunk {
public:
    Vector3 originPosition;
    float blockSize;
    int row;
    int column;
    std::vector<Block> blocks;
    chunk(Vector3 pos, float size, int r, int c);
    void Draw(Model& cubeModel, Model& metalModel);
};
