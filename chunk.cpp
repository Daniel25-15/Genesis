#include "chunk.h"
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distrib(1, 1);

chunk::chunk(Vector3 pos, float size, int r, int c) {
    originPosition = pos;
    blockSize = size;
    row = r;
    column = c;
    for (int rIndex = 0; rIndex < row; rIndex++) {
        for (int cIndex = 0; cIndex < column; cIndex++) {
            Vector3 blockPos = {
                originPosition.x + (cIndex * blockSize),
                originPosition.y + distrib(gen),
                originPosition.z + (rIndex * blockSize)
            };
            blocks.push_back(Block(blockPos, blockSize, 0, GREEN));
        }
    }
}
void chunk::Draw(Model& cubeModel, Model& metalModel) {
    for (Block& b : blocks) {
        b.Draw(cubeModel, metalModel);
    }
}
