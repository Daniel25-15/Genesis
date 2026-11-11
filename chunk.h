#pragma once
#include "raylib.h"
#include <vector>
#include "block.h"
#include <string>


class chunk {
public:
    Vector3 originPosition;
    float blockSize;
    int row;
    int column;
    int rotationAngle;
    std::string heightmapPath;
    Mesh terrainMesh = { 0 };
    Model terrainModel = { 0 };
    std::vector<Block> blocks;
    chunk(Vector3 pos, float size, int r, int c, int ra, const char* heightmapPath = nullptr);
    void Generate();
    void GenerateMesh();
    void Draw(Model& cubeModel, Model& metalModel, Model& metalGrate, Model& wallModel, Model& windowModel);
    void Unload();
};
