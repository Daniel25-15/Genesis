#include "chunk.h"
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distrib(1, 2);

chunk::chunk(Vector3 pos, float size, int r, int c, int ra, const char* heightmapPath) {
    originPosition = pos;
    blockSize = size;
    row = r;
    column = c;
    rotationAngle = ra;
    Image heightmap = {};
    bool useHeightMap = (heightmapPath != nullptr);
    if (useHeightMap) {
        heightmap = LoadImage(heightmapPath);
        ImageRotate(&heightmap, rotationAngle);
        // if (heightmap.id == 0) {TraceLog(LOG_ERROR, "Failed to load asset: blockTexture (grass.png)");std::cout<<"Failed to load asset:(grass.png)"<<std::endl;};
        if (heightmap.data == nullptr) {
            std::cerr << "Failed to load heightmap: " << heightmapPath << std::endl;
            useHeightMap = false;
            // return;
        }
    }
    
    for (int rIndex = 0; rIndex < row; rIndex++) {
        for (int cIndex = 0; cIndex < column; cIndex++) {
            float blockHeight = 0.0f;
            if (useHeightMap) {
                // int imgX = (int)(((float)cIndex / (column - 1)) * (heightmap.width - 1));
                // int imgY = (int)(((float)rIndex / (row -1)) * (heightmap.width -1));
                int imgX = (column > 1) ? (int)(((float)cIndex / (column - 1)) * (heightmap.width - 1)) : 0;
                int imgY = (row > 1) ? (int)(((float)rIndex / (row -1)) * (heightmap.width -1)) : 0; 
                Color pixelColor = GetImageColor(heightmap, imgX, imgY);
                float normalizedHeight = (float)pixelColor.r / 255.0f;
                float maxHeight = 1.5f;
                int maxBlockSteps = 5;
                int heightSteps = (int)(normalizedHeight * maxBlockSteps);
                blockHeight = heightSteps * blockSize;
                // blockHeight = normalizedHeight * maxHeight;
            } else {
                blockHeight = distrib(gen);
            }
            // std::cout << "Height: " << blockHeight << std::endl;
            Vector3 blockPos = {
                originPosition.x + (cIndex * blockSize),
                originPosition.y + blockHeight,
                originPosition.z + (rIndex * blockSize)
            };
            blocks.push_back(Block(blockPos, blockSize, 0, GREEN));
        }
    }
    if (useHeightMap) {
        UnloadImage(heightmap);
    }
}
void chunk::Draw(Model& cubeModel, Model& metalModel, Model& metalGrate, Model& wallModel, Model& windowModel) {
    for (Block& b : blocks) {
        b.Draw(cubeModel, metalModel, metalGrate, wallModel, windowModel);
    }
}
