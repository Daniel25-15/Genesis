#include "block.h"

Block::Block(Vector3 pos, float s, int identifier, Color c) {
    position = pos;
    size = s;
    id = identifier;
    color = c;
}
BoundingBox Block::GetBoundingBox() const {
    if (id == 2) {
        return {
            { position.x - (size / 2), position.y - (size / 1), position.z - (size / 2) },
            { position.x + (size / 2), position.y + (size / 1), position.z + (size / 2) }
        };
    } else {
        return {
            { position.x - (size / 2), position.y - (size / 2), position.z - (size / 2) },
            { position.x + (size / 2), position.y + (size / 2), position.z + (size / 2) }
        };
    }
    
}
void Block::Draw(Model& model, Model& metalModel) {
        // Texture2D blockTexture = LoadTexture("assets/grass.png");
        // Texture2D stoneTexture = LoadTexture("assets/blocks.png");
        // model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = stoneTexture;

        Vector3 cubePos = { position.x, position.y, position.z};
        // DrawCubeTexture(texture, cubePos, size, size, size, color);
        // if (texture.id > 0) {
        //     model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        //     DrawModel(model, cubePos, size, WHITE);
        // } 
        // else {
        //     DrawCube(cubePos, size, size, size, color);
        // }
        // DrawCube(cubePos, size, size, size, color);
        if (id == 2) {
            DrawModel(metalModel, position, 0.175f, WHITE);
            // DrawCubeWires(cubePos, size, size, size, BLACK);
        } 
        else {
            if (blockTextures[id].id > 0) {
                model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = blockTextures[id];
            }
            DrawModel(model, cubePos, size, WHITE);
            DrawCubeWires(cubePos, size, size, size, BLACK);
        }
        // if (id != 2) {DrawCubeWires(cubePos, size, size, size, BLACK);}
    
}