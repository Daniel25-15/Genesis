#include "wall.h"

Wall::Wall(Vector3 pos, float size, int w, int l, int h, int identifier) {
    originPosition = pos;
    blockSize = size;
    width = w;
    length = l;
    height = h;
    id = identifier;
    float totalWidth = width * blockSize;
    float totalLength = length * blockSize;
    float totalHeight = height * blockSize;
    bounds = {
        { originPosition.x, originPosition.y, originPosition.z},
        { originPosition.x + totalWidth, originPosition.y + totalHeight, originPosition.z + totalLength}
    };
    
    for (int wIndex = 0; wIndex < w; wIndex++) {
        for (int lIndex = 0; lIndex < length; lIndex++) {
            for (int hIndex = 0; hIndex < height; hIndex++) {
                Vector3 blockPos = {
                    originPosition.x + (lIndex * blockSize),
                    originPosition.y + (hIndex * blockSize),
                    originPosition.z + (wIndex * blockSize)
                };
                blocks.push_back(Block(blockPos, blockSize, id, GREEN));
            }
        }
    }
}
void Wall::Draw(Model& cubeModel, Model& metalModel, Model& metalGrate, Model& wallModel, Model& windowModel) {
    for (Block& b : blocks) {
        b.Draw(cubeModel, metalModel, metalGrate, wallModel, windowModel);
    }
}
