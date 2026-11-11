#include "chunk.h"

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <cstring>

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, 2);

    void addFace(
        std::vector<float>& vertices,
        std::vector<float>& texcoords,
        std::vector<float>& normals,
        Vector3 p0,
        Vector3 p1,
        Vector3 p2,
        Vector3 p3,
        Vector3 normal
    ) {
        Vector3 positions[6] = {
            p0, p1, p2,
            p0, p2, p3
        };
        Vector2 uvs[6] = {
            { 0.0f, 1.0f },
            { 1.0f, 1.0f },
            { 1.0f, 0.0f },

            { 0.0f, 1.0f },
            { 1.0f, 0.0f },
            { 0.0f, 0.0f },
        };
        for (int i = 0; i < 6; i++) {
            vertices.push_back(positions[i].x);
            vertices.push_back(positions[i].y);
            vertices.push_back(positions[i].z);

            texcoords.push_back(uvs[i].x);
            texcoords.push_back(uvs[i].y);

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
        }
    }
}

chunk::chunk(Vector3 pos, float size, int r, int c, int ra, const char* heightmapPath) {
    originPosition = pos;
    blockSize = size;
    row = r;
    column = c;
    rotationAngle = ra;
    this->heightmapPath = (heightmapPath != nullptr) ? heightmapPath : "";
    Generate();
}

void chunk::Generate() {
    Unload();
    blocks.clear();
    if (row <= 0 || column <= 0 || blockSize <= 0.0f) {
        return;
    }
    size_t totalBlocks = static_cast<size_t>(row) * static_cast<size_t>(column);
    blocks.reserve(totalBlocks);
    Image heightmap = { 0 };
    bool useHeightMap = !heightmapPath.empty();
    if (useHeightMap) {
        heightmap = LoadImage(heightmapPath.c_str());
        if (heightmap.data == nullptr) {
            std::cerr << "Failed to load heightmap: " << heightmapPath << std::endl;
            useHeightMap = false;
        } else {
            ImageRotate(&heightmap, rotationAngle);
        }
    }
    for (int rIndex = 0; rIndex < row; rIndex++) {
        for (int cIndex = 0; cIndex < column; cIndex++) {
            float blockHeight = 0.0f;
            if (useHeightMap) {
                int imgX = (column > 1) ? static_cast<int>((static_cast<float>(cIndex) / (column - 1)) * (heightmap.width - 1)) : 0;
                int imgY = (row > 1) ? static_cast<int>((static_cast<float>(rIndex) / (row - 1)) * (heightmap.height - 1)) : 0;
                Color pixelColor = GetImageColor(heightmap, imgX, imgY);
                float normalizedHeight = pixelColor.r / 255.0f;
                int maxBlockSteps = 5;
                int heightSteps = static_cast<int>(normalizedHeight * maxBlockSteps);
                blockHeight = heightSteps * blockSize;
            } else {
                blockHeight = static_cast<float>(distrib(gen));
            }
            Vector3 blockPos = {
                originPosition.x + (cIndex * blockSize),
                originPosition.y + blockHeight,
                originPosition.z + (rIndex * blockSize)
            };
            blocks.emplace_back(blockPos,blockSize, 0, GREEN);
        }
    }
    if (heightmap.data != nullptr) {
        UnloadImage(heightmap);
    }
    GenerateMesh();
}
void chunk::GenerateMesh() {
    if (blocks.empty()) {
        return;
    }
    std::vector<float> vertices;
    std::vector<float> texcoords;
    std::vector<float> normals;

    vertices.reserve(blocks.size() * 6 * 6 * 3);
    texcoords.reserve(blocks.size() * 6 * 6 * 2);
    normals.reserve(blocks.size() * 6 * 6 * 3);

    for (int rIndex = 0; rIndex < row; rIndex++) {
        for (int cIndex = 0; cIndex < column; cIndex++) {
            size_t index = static_cast<size_t>(rIndex) * column + cIndex;
            if (index >= blocks.size()) {
                continue;
            }
            const Block& block = blocks[index];
            if (block.id != 0) {
                continue;
            }
            float half = block.size / 2.0f;
            Vector3 center = {
                block.position.x - originPosition.x,
                block.position.y - originPosition.y,
                block.position.z - originPosition.z
            };
            float x0 = center.x - half;
            float x1 = center.x + half;
            float y0 = center.y - half;
            float y1 = center.y + half;
            float z0 = center.z - half;
            float z1 = center.z + half;

            auto sameHeight = [&](const Block& neighbor) {
                return neighbor.id == 0 &&
                std::fabs(neighbor.position.y - block.position.y) < 0.001f;
            };
            bool left = false;
            bool right = false;
            bool front = false;
            bool back = false;

            if (cIndex > 0) {
                left = sameHeight(blocks[index - 1]);
            }
            if (cIndex < column - 1) {
                right = sameHeight(blocks[index + 1]);
            }
            if (rIndex > 0) {
                back = sameHeight(blocks[index - column]);
            }
            if (rIndex < row - 1) {
                front = sameHeight(blocks[index + column]);
            }

            addFace(
                vertices, texcoords, normals,
                { x0, y1, z0 },
                { x0, y1, z1 },
                { x1, y1, z1 },
                { x1, y1, z0 },
                { 0.0f, 1.0f, 0.0f}
            );
            addFace(
                vertices, texcoords, normals,
                { x0, y0, z1 },
                { x0, y0, z0 },
                { x1, y0, z0 },
                { x1, y0, z1 },
                { 0.0f, -1.0f, 0.0f }
            );
            if (!left) {
                addFace(
                    vertices, texcoords, normals,
                    { x0, y0, z0 },
                    { x0, y0, z1 },
                    { x0, y1, z1 },
                    { x0, y1, z0 },
                    { -1.0f, 0.0f, 0.0f }
                );
            }
            if (!right) {
                addFace(
                    vertices, texcoords, normals,
                    { x1, y0, z1 },
                    { x1, y0, z0 },
                    { x1, y1, z0 },
                    { x1, y1, z1 },
                    { 1.0f, 0.0f, 0.0f}
                );
            }
            if (!front) {
                addFace(
                    vertices, texcoords, normals,
                    { x0, y0, z1 },
                    { x1, y0, z1 },
                    { x1, y1, z1 },
                    { x0, y1, z1 },
                    { 0.0f, 0.0f, 1.0f }
                );
            }
            if (!back) {
                addFace(
                    vertices, texcoords, normals,
                    { x1, y0, z0 },
                    { x0, y0, z0 },
                    { x0, y1, z0 },
                    { x1, y1, z0 },
                    { 0.0f, 0.0f, -1.0f }
                );
            }
        }
    }
    if (vertices.empty()) {
        return;
    }
    terrainMesh = { 0 };
    terrainMesh.vertexCount = static_cast<int>(vertices.size() / 3);
    terrainMesh.triangleCount = terrainMesh.vertexCount / 3;
    terrainMesh.vertices = static_cast<float*>(MemAlloc(vertices.size() * sizeof(float)));
    terrainMesh.texcoords = static_cast<float*>(MemAlloc(texcoords.size() * sizeof(float)));
    terrainMesh.normals = static_cast<float*>(MemAlloc(normals.size() * sizeof(float)));

    if (terrainMesh.vertices == nullptr || terrainMesh.texcoords == nullptr || terrainMesh.normals == nullptr) {
        MemFree(terrainMesh.vertices);
        MemFree(terrainMesh.texcoords);
        MemFree(terrainMesh.normals);
        terrainMesh = { 0 };
        std::cerr << "Failed to allocate chunk memory." << std::endl;
        return;
    }
    std::memcpy(terrainMesh.vertices, vertices.data(), vertices.size() * sizeof(float));
    std::memcpy(terrainMesh.texcoords, texcoords.data(), texcoords.size() * sizeof(float));
    std::memcpy(terrainMesh.normals, normals.data(), normals.size() * sizeof(float));
    UploadMesh(&terrainMesh, false);
    terrainModel = LoadModelFromMesh(terrainMesh);
}
void chunk::Draw(Model& cubeModel, Model& metalModel, Model& metalGrate, Model& wallModel, Model& windowModel) {
    if (terrainModel.meshCount > 0 && terrainModel.materialCount > 0 && cubeModel.materialCount > 0){
        terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = blockTextures[0];
        DrawModel(terrainModel, originPosition, 1.0f, WHITE);
    }
    for (Block& block : blocks) {
        if (block.id != 0) {
            block.Draw(cubeModel, metalModel, metalGrate, wallModel, windowModel);
        }
    }
}
void chunk::Unload() {
    if (terrainModel.meshes != nullptr) {
        UnloadModel(terrainModel);
    }
    terrainModel = { 0 };
    terrainMesh = { 0 };
}