#pragma once
#include "raylib.h"

extern Texture2D blockTextures[10];

class Block {
public:
    Vector3 position;
    float size;
    Color color;
    int id;
    Block(Vector3 pos, float s, int identifier, Color c);
    BoundingBox GetBoundingBox() const;
    void Draw(Model& cubeModel, Model& metalModel, Model& metalGrate, Model& wallModel, Model& windowModel);
};