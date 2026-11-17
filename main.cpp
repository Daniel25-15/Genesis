#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include <unistd.h>
#include <libgen.h>
#include "json.hpp"
#include <iostream>
#include <fstream>

#include "block.h"
#include "chunk.h"
#include "player.h"
#include "wall.h"
#include "follower.h"

std::vector<Block> allBlocks;
std::vector<chunk> worldChunks;
std::vector<Wall> allWalls;
std::vector<BoundingBox> allColiders;
std::vector<Follower> followers;
// std::vector

using json = nlohmann::json;

class Item{
public:
    Vector3 position;
    float size;
    Color color;
    Model model;
    Item(Vector3 pos, float s, Color c, Model m) {
        position = pos;
        size = s;
        color = c;
        model = m;
    };
    void Draw(Vector3 rotationAxis, float rotationAngle) {
        DrawModelEx(model, position, rotationAxis, rotationAngle, (Vector3) {size, size, size}, color);
    };
};

class ad {
public:
    // float width;
    // float height;
    Vector2 position;
    float scale;
    float rotation;
    ad(Vector2 pos, float s, float r) {
        position = pos;
        scale = s;
        rotation = r;
    }
    void Draw(Texture2D texture) {
        DrawTextureEx(texture, position, rotation, scale, WHITE);
    }
};

class menuButton {
public:
    // bool isHovering = false;
    int yOffset;
    int xOffset;
    menuButton(int y, int x) : yOffset(y), xOffset(x) {}
    
    void Draw(const char* text, Vector2 mousePoint, std::function<void()> onClick, bool gamePaused, bool settingsMenu) {
        Rectangle button = {(float)(GetScreenWidth() / 2) -xOffset, (float)(GetScreenHeight() / 2) - yOffset, 110.0f, 40.0f };
        bool isHovering = CheckCollisionPointRec(mousePoint, button);
        Color menuButtonColor = isHovering ? LIGHTGRAY : DARKGRAY;
        // if (isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gamePaused && settingsMenu) {std::cout<<"Settings Exit"<<std::endl; settingsMenu = false; mainPauseMenu = true;}
        if (isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gamePaused && settingsMenu) {onClick();}
        DrawRectangleRec(button, menuButtonColor);
        DrawRectangleLines((int)button.x, (int)button.y, (int)button.width, (int)button.height, BLACK);
        DrawText(text, (int)button.x + 15, (int)button.y + 10, 20, WHITE);
    }

};

// 
// Color exitButtonColor = isHoveringExit ? LIGHTGRAY : DARKGRAY;

// DrawRectangleRec(exitButton, exitButtonColor);
// DrawRectangleLines((int)exitButton.x, (int)exitButton.y, (int)exitButton.width, (int)exitButton.height, BLACK);
// DrawText("exit", (int)exitButton.x + 15, (int)exitButton.y + 10, 20, WHITE);



int buildObject(int chunkSizeInt) {
    allBlocks.push_back(Block((Vector3){ -2.0f, 1.0f, 1.0f }, 1.0f, 1, BLUE));
    allBlocks.push_back(Block((Vector3){ -2.0f, 2.0f, 1.0f }, 1.0f, 1, BLUE));
    allBlocks.push_back(Block((Vector3){ -3.0f, 1.0f, 2.0f }, 1.0f, 1, BLUE));
    allBlocks.push_back(Block((Vector3){ -2.0f, 2.5f, 2.0f }, 1.0f, 1, BLUE));
    allBlocks.push_back(Block((Vector3){ -5.0f, 2.5f, 2.0f }, 1.0f, 2, BLUE));
    allBlocks.push_back(Block((Vector3){ -2.0f, 2.5f, 4.0f }, 1.0f, 2, BLUE));
    allBlocks.push_back(Block((Vector3){ 0.0f, 6.5f, 3.0f }, 1.0f, 4, BLUE));
    allBlocks.push_back(Block((Vector3){ -2.0f, 6.0f, 1.0f }, 1.0f, 5, BLUE));

    worldChunks.push_back(chunk((Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, chunkSizeInt, chunkSizeInt, 0, "assets/heightmap2.png"));
    worldChunks.push_back(chunk((Vector3) {-1.0f, -1.0f, -64.0f}, 1.0f, chunkSizeInt, chunkSizeInt, -90, "assets/heightmap2.png"));


    return 0;
}

int buildBuilding() {
    allWalls.push_back(Wall((Vector3) {5.0f, 1.0f, 5.0f}, 1.0f, 1, 8, 3, 1));
    allWalls.push_back(Wall((Vector3) {5.0f, 1.0f, 14.0f}, 1.0f, 1, 8, 3, 1));
    allWalls.push_back(Wall((Vector3) {5.0f, 1.0f, 6.0f}, 1.0f, 8, 1, 3, 1));
    allWalls.push_back(Wall((Vector3) {12.0f, 1.0f, 6.0f}, 1.0f, 8, 1, 3, 1));

    return 0;
};

int buildColliders() {
    allColiders.clear();
    size_t totalBlocks = allBlocks.size();

    for (const auto& wall : allWalls) {
        totalBlocks += wall.blocks.size();
    }
    for (const auto& c : worldChunks) {
        totalBlocks += c.blocks.size();
    }

    allColiders.reserve(totalBlocks);

    for (const auto& wall : allWalls) {
        for (const auto& b : wall.blocks) {
            allColiders.push_back(b.GetBoundingBox());
        }
    }
    for (const auto& chunk : worldChunks) {
        for (const auto& c: chunk.blocks) {
            allColiders.push_back(c.GetBoundingBox());
        }
    }
        for (const auto& b : allBlocks) {
        allColiders.push_back(b.GetBoundingBox());
    }

    // for (const auto& wall : allWalls) {
    //     for (const auto& b: wall.blocks) {
    //         allColiders.push_back(b.GetBoundingBox());
    //     }
    // }





    return 0;
}

int toggleAds(bool ads, json data) {
    ads = !ads;
    data["ads-enabled"] = ads;
    std::ofstream outFile("settings.json");
    if(outFile.is_open()){outFile << data.dump(4);outFile.close();} else {std::cerr << "Error opening settings.json. Could not open settings.json.\n";} return 0;
}

int increaseFps(json& data) {
    int fpsAmount = data["fps-limiter"];
    if (fpsAmount >= 120) {fpsAmount = 30;} else {fpsAmount += 30;}
    data["fps-limiter"] = fpsAmount;
    std::ofstream outFile("settings.json");
    if(outFile.is_open()){outFile << data.dump(4);outFile.close();} else {std::cerr << "Error opening settings.json. Could not open settings.json.\n";};
    SetTargetFPS(fpsAmount);
    std::cout << "Current FPS Max: " << fpsAmount << std::endl;
    return 0;
}

Texture2D blockTextures[10];

int main(int argc, char* argv[]) {
    char path[1024];
    if (argc > 0) {
        realpath(argv[0], path);
        char* dir = dirname(path);
        chdir(dir);
    }
    InitWindow(1475, 775, "Genesis");
    InitAudioDevice();

    std::ifstream file("settings.json");
    if (!file.is_open()) {
        std::cerr << "Error opening file. Failed to open file settings.json\n";
        return 1;
    }

    json data;
    file >> data;

    // std::string 
    bool enabledAds = data["ads-enabled"];
    int fpsMaxAmount = data["fps-limiter"];
    int chunkSize = data["chunk-size"];

    SetTargetFPS(fpsMaxAmount);

    SetExitKey(KEY_Q);    

    blockTextures[0] = LoadTexture("assets/grass.png");
    blockTextures[1] = LoadTexture("assets/blocks.png");
    
    Texture2D crosshairTexture = LoadTexture("assets/crosshair.png");
    Texture2D healthBarTexture = LoadTexture("assets/healthBar.png");
    Texture2D cokeAdTexture = LoadTexture("assets/cokeAd.png");
    if (blockTextures[0].id == 0) {TraceLog(LOG_ERROR, "Failed to load asset: blockTexture (grass.png)");std::cout<<"Failed to load asset:(grass.png)"<<std::endl;};
    if (blockTextures[1].id == 0) {TraceLog(LOG_ERROR, "Failed to load asset: stoneTexture (stone.png)");std::cout<<"Failed to load asset:(stone.png)"<<std::endl;};
    Model cubeModel = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    Model metalModel = LoadModel("assets/metalModel.glb");
    Model metalGrate = LoadModel("assets/metalGrate.glb");
    Model wallModel = LoadModel("assets/brickWall.glb");
    Model windowModel = LoadModel("assets/windowWall.glb");
    Model pistolModel = LoadModel("assets/pistolModel.glb");
    Model followerModel = LoadModel("assets/human.glb");
    Sound gunShotOne = LoadSound("assets/gunShotOne.mp3");
    cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = blockTextures[0];

    const int soundLimit = 8;
    Sound soundPool[soundLimit];
    for (int i = 0; i < soundLimit; i++) {
        soundPool[i] = LoadSoundAlias(gunShotOne);
    }
    int currentSoundIndex = 0;

    bool gamePaused = false;
    bool mainPauseMenu = false;
    bool settingsMenu = false;

    bool weaponFired = false;

    float standingHeight = 1.8f;
    float crouchingHeight = 1.2f;

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.5f, 0.0f };
    camera.target = (Vector3){ 0.0f, 2.5f, 1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };


    int cameraMode = CAMERA_FREE;

    buildObject(chunkSize);
    buildBuilding();



    // chunk mainChunk((Vector3) {-2.0f, 5.0f, 0.0f}, 1.0f, 4, 4);
    buildColliders();

    std::cout << "All colliders loaded: " << allColiders.size() << std::endl;

    // worldChunks.push_back(chunk((Vector3) {-40.0f, 0.0f, -40.0f}, 1.0f, 32, 32, "assets/heightmap2.png"));
    // worldChunks.push_back(chunk((Vector3) {-8.0f, 0.0f, -40.0f}, 1.0f, 32, 32, "assets/heightmap2.png"));
    // worldChunks.push_back(chunk((Vector3) {-40.0f, 0.0f, -8.0f}, 1.0f, 32, 32, "assets/heightmap2.png"));

    // allWalls.push_back(Wall((Vector3) {-8.0f, 0.0f, -8.0f}, 1.0f, 32, 32));
    // worldChunks.push_back(chunk((Vector3) {-7.0f, 5.0f, 0.0f}, 1.0f, 4, 4));

    // allWalls.push_back(Wall((Vector3) {5.0f, 1.0f, 5.0f}, 1.0f, 1, 1, 1, 1));

    Player player(Player::playerSpawnPosition, (Vector3) { 0.8f, standingHeight, 0.8f }, BLUE);

    // Follower Follower((Vector3) { 5.0f, 7.0f, 5.0f }, (Vector3) { 1.0f, 3.0f, 1.0f }, 100);

    followers.push_back(Follower((Vector3) { 5.0f, 7.0f, 5.0f }, (Vector3) { 1.0f, 3.0f, 1.0f }, 100));
    followers.push_back(Follower((Vector3) { 10.0f, 7.0f, 5.0f }, (Vector3) { 1.0f, 3.0f, 1.0f }, 100));

    Item weapon((Vector3){player.position.x, player.position.y, player.position.z}, 0.2f, WHITE, pistolModel);

    ad ad((Vector2) { (float) GetScreenWidth() - 250.0f, 10.0f}, 0.5f, 0.0f);

    menuButton enableAdsButton(30, -30);

    menuButton fpsIncreasButton(30, 120);



    DisableCursor();

    while (!WindowShouldClose()) {

        Vector3 oldCameraPos = camera.position;
        
        // Vector3 movement = { camera.position.x - oldCameraPos.x, 0.0f, camera.position.z - oldCameraPos.z };
        Vector3 forward = { camera.target.x - camera.position.x, camera.target.y - camera.position.y, camera.target.z - camera.position.z};
        float forwardLength = sqrtf(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
        if (forwardLength > 0.001f) {
            forward.x /= forwardLength;
            forward.y /= forwardLength;
            forward.z /= forwardLength;
        }
        Vector3 right = { forward.z, 0.0f, -forward.x };
        Vector3 ItemRight = { forward.y * camera.up.z - forward.z * camera.up.y, forward.z * camera.up.x - forward.x * camera.up.z, forward.x * camera.up.y - forward.y * camera.up.x};
        Vector3 movement = {0.0f, 0.0f, 0.0f };
        // float speed = 0.05f;

        bool queueCrouch = IsKeyDown(KEY_C);

        Vector2 crosshairPos = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        Ray aimRay = GetScreenToWorldRay(crosshairPos, camera);
        
        BoundingBox overheadCheck = {
            { player.position.x - (player.size.x / 2.0f), player.position.y + crouchingHeight, player.position.z - (player.size.z / 2.0f) },
            { player.position.x + (player.size.x / 2.0f), player.position.y + standingHeight, player.position.z + (player.size.z / 2.0f) },
        };
        
        bool blockedAbove = false;
        for (const auto& b : allColiders) {
            if (CheckCollisionBoxes(b, overheadCheck)) {
                blockedAbove = true;
                break;
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {if (settingsMenu) {settingsMenu = false; mainPauseMenu = true;} else {gamePaused = !gamePaused; mainPauseMenu = !mainPauseMenu; if(gamePaused) {EnableCursor();} else {DisableCursor();}}}
        
        if (!gamePaused) {
            UpdateCamera(&camera, CAMERA_FIRST_PERSON);
            if (IsKeyDown(KEY_LEFT_SHIFT) && !queueCrouch) {player.speed = 2.5f;} else { player.speed = 2.0f;};
            // if (IsKeyDown(KEY_C)) {player.size.y = 1.0f;} else {if() {player.size.y = 1.8f;}};
            if (queueCrouch || blockedAbove) {if (player.size.y == standingHeight) {player.position.y -= (standingHeight - crouchingHeight) / 2.0f;}player.size.y = crouchingHeight;} else {if (player.size.y == crouchingHeight) {player.position.y += (standingHeight - crouchingHeight) / 2.0f;}player.size.y = standingHeight;};
            if (IsKeyDown(KEY_W)) {movement.x += forward.x * player.speed; movement.z += forward.z * player.speed;}
            if (IsKeyDown(KEY_S)) {movement.x -= forward.x * player.speed; movement.z -= forward.z * player.speed;}
            if (IsKeyDown(KEY_A)) {movement.x += right.x * player.speed; movement.z += right.z * player.speed;}
            if (IsKeyDown(KEY_D)) {movement.x -= right.x * player.speed; movement.z -= right.z * player.speed; }
            if (IsKeyDown(KEY_SPACE) && player.isOnGround && !blockedAbove) {player.velocityY = 8.0f; player.isOnGround = false;}
            if (IsKeyPressed(KEY_ONE)) {player.currentItemNumber = 1;}if (IsKeyPressed(KEY_TWO)) {player.currentItemNumber = 2;}if (IsKeyPressed(KEY_THREE)) {player.currentItemNumber = 3;}if (IsKeyPressed(KEY_FOUR)) {player.currentItemNumber = 4;}if (IsKeyPressed(KEY_FIVE)) {player.currentItemNumber = 5;}if (IsKeyPressed(KEY_SIX)) {player.currentItemNumber = 6;}if (IsKeyPressed(KEY_SEVEN)) {player.currentItemNumber = 7;}if (IsKeyPressed(KEY_EIGHT)) {player.currentItemNumber = 8;}if (IsKeyPressed(KEY_NINE)) {player.currentItemNumber = 9;}
            if (IsKeyPressed(KEY_R)) {player.Death();}
            if (IsKeyPressed(KEY_X)) {if (player.healthBarAmount > 0) {player.healthBarAmount -= 1;}}
            if (IsKeyPressed(KEY_Z)) {fpsMaxAmount = 120; data["fps-limiter"] = fpsMaxAmount; std::ofstream outFile("settings.json"); if(outFile.is_open()){outFile << data.dump(4);outFile.close();} else {std::cerr << "Error opening settings.json. Could not open settings.json.\n";}SetTargetFPS(fpsMaxAmount);}
            if (IsKeyPressed(KEY_F)) {fpsMaxAmount = 60; data["fps-limiter"] = fpsMaxAmount; std::ofstream outFile("settings.json"); if(outFile.is_open()){outFile << data.dump(4);outFile.close();} else {std::cerr << "Error opening settings.json. Could not open settings.json.\n";}SetTargetFPS(fpsMaxAmount);}
            for (auto& follower : followers) {
                BoundingBox box = follower.GetBoundingBox();
                RayCollision hit = GetRayCollisionBox(aimRay, box);
                if (hit.hit && hit.distance <= 15.0f) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player.currentItemNumber == 1) {
                        follower.healthAmount -= 10;
                        weaponFired = true;
                        PlaySound(soundPool[currentSoundIndex]);
                        currentSoundIndex = (currentSoundIndex + 1) % soundLimit;
                        std::cout<<"Follower Hit: " << follower.healthAmount << std::endl;
                    }
                }
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player.currentItemNumber == 1) {weaponFired = true;PlaySound(soundPool[currentSoundIndex]);currentSoundIndex = (currentSoundIndex + 1) % soundLimit;for (auto& follower : followers) {BoundingBox box = follower.GetBoundingBox();RayCollision hit = GetRayCollisionBox(aimRay, box);if (hit.hit && hit.distance <= 15.0f) {follower.healthAmount -= 10;std::cout<<"Follower Hit: " << follower.healthAmount << std::endl;}}}}

        for(Follower& f: followers) {
            Vector3 followerMovement = {
                (player.position.x - f.position.x),
                0.0f,
                (player.position.z - f.position.z)
            };
            float followerLength = sqrtf(followerMovement.x * followerMovement.x + followerMovement.z * followerMovement.z);
            if ((((player.position.y  - (player.size.y / 2.0f)) - 0.85f) > (f.position.y - (f.size.y / 2.0f))) && f.isOnGround) {
                f.velocityY = 8.0f;
                f.isOnGround = false;
            }
            if (followerLength > 0.001f) {
                followerMovement.x /= followerLength;
                followerMovement.z /= followerLength;
            }
            
            // float hitBoxPadding = 1.0f;
            
            // if (
                //     player.position.x + player.size.x + hitBoxPadding <=  
                // )
            if (f.attackCooldown > 0.0f) {
                f.attackCooldown -= GetFrameTime();
                if (f.attackCooldown < 0.0f) {
                    f.attackCooldown = 0.0f;
                }
            }
            
            if (CheckCollisionBoxes(player.GetBoundingBox(), f.GetBoundingBox()) && f.attackCooldown <= 0.0f) {
                f.attackCooldown = f.attackCooldownDuration;
                player.healthBarAmount -= 1;
            }
            if (followerLength < 25.0f && !gamePaused) {
                f.Move(followerMovement, allColiders);
            }
        }
        for (int i = 0; i < (int)followers.size();) {
            if (followers[i].healthAmount <= 0) {
                followers.erase(followers.begin() + i);
            } else {
                i++;
            }
        }


        player.Move(movement, allColiders);



        if (player.healthBarAmount <= 0 || player.position.y < -50.0f) {
            player.Death();
        }

        // weapon.position = player.position;
        // Vector3 itemPosition = {
        //     camera.position.x,
        //     camera.position.y,
        //     camera.position.z + 0.5f
        // };

        // Vector3 itemPosition = {
        //     camera.position.x + (forward.x * 0.5f) + (right.x * 0.2f),
        //     camera.position.y - 0.25f,
        //     camera.position.z + (forward.z * 0.5f) - (right.z * 0.3f)
        // };
        float rightLen = sqrtf(ItemRight.x * ItemRight.x + ItemRight.y * ItemRight.y + ItemRight.z * ItemRight.z);
        if (rightLen > 0.001f) {
            ItemRight.x /= rightLen;
            ItemRight.y /= rightLen;
            ItemRight.z /= rightLen;
        }

        weapon.position = (Vector3){
            camera.position.x + (forward.x * 0.6f) + (right.x * -0.3f) + (camera.up.x * -0.25f),
            camera.position.y + (forward.y * 0.6f) + (right.y * -0.3f) + (camera.up.y * -0.25f),
            camera.position.z + (forward.z * 0.6f) + (right.z * -0.3f) + (camera.up.z * -0.25f),
        };

        Vector3 cameraViewOffest = {
            camera.target.x - camera.position.x,
            camera.target.y - camera.position.y,
            camera.target.z - camera.position.z,
        };

        Vector3 forwardFacing = Vector3Subtract(camera.target, camera.position);
        forwardFacing = Vector3Normalize(forwardFacing);

        float weaponYaw = atan2f(forwardFacing.x, forwardFacing.z) * (180.0f / PI);

        // weapon.position = (Vector3) { camera.position.x, camera.position. y - 2.0f, camera.position.z};


        camera.position = player.position;
        camera.position.y = player.position.y + 0.75f;

        camera.target.x = camera.position.x + cameraViewOffest.x;
        camera.target.z = camera.position.z + cameraViewOffest.z;
        camera.target.y = camera.position.y + cameraViewOffest.y;

        float rayLength = 5.0f;
        Vector3 rayStart = {
            camera.position.x + (forwardFacing.x * 0.5f),
            camera.position.y + ((forwardFacing.y * 0.5f) - 0.1f),
            camera.position.z + (forwardFacing.z * 0.5f)
        };
        Vector3 rayEnd = {
            camera.position.x + (forwardFacing.x * rayLength),
            camera.position.y + ((forwardFacing.y * rayLength)),
            camera.position.z + (forwardFacing.z * rayLength)
        };

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawGrid(10, 1.0f);
                for (Block& b : allBlocks) {
                    // b.Draw(cubeModel, stoneTexture);
                    // if (b.id == 1) {
                    //     b.Draw(cubeModel, metalModel);
                    // } else if (b.id == 2) {
                    //      b.Draw(metalModel, metalModel);
                    // }
                    b.Draw(cubeModel, metalModel, metalGrate, wallModel, windowModel);
                }
                for (chunk& c : worldChunks) {c.Draw(cubeModel, metalModel, metalGrate, wallModel, windowModel);}
                for (Wall& w : allWalls) {w.Draw(cubeModel, metalModel, metalGrate, wallModel, windowModel);}
                // player.Draw();
                player.Wireframe();
                for(Follower f: followers) {f.Draw(followerModel, player.position);}
                if (player.currentItemNumber == 1) {weapon.Draw((Vector3) {0.0f, 0.5f, 0.0f }, weaponYaw);}
                if (weaponFired) {
                    if (player.currentItemNumber == 1) {
                        DrawCylinderEx(rayStart, rayEnd, 0.02f, 0.02f, 4, RED);
                        weaponFired = false;
                    }
                }
            EndMode3D();
            Rectangle quitButton = { (float)(GetScreenWidth() / 2) - 50, (float)(GetScreenHeight() / 2) - 30, 110.0f, 40.0f };
            Rectangle resumeButton = { (float)(GetScreenWidth() / 2) - 50, (float)(GetScreenHeight() / 2) - 80, 110.0f, 40.0f };
            Rectangle settingsButton = { (float)(GetScreenWidth() / 2) - 50, (float)(GetScreenHeight() / 2) + 20, 110.0f, 40.0f };

            Vector2 mousePoint = GetMousePosition();
            bool isHoveringQuit = CheckCollisionPointRec(mousePoint, quitButton);
            bool isHoveringResume = CheckCollisionPointRec(mousePoint, resumeButton);
            bool isHoveringSettings = CheckCollisionPointRec(mousePoint, settingsButton);
            if (isHoveringQuit  && !settingsMenu && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gamePaused) {quick_exit(0);}
            if (isHoveringResume && !settingsMenu  && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gamePaused) {gamePaused = false; mainPauseMenu = false; DisableCursor();}
            if (isHoveringSettings && !settingsMenu  && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gamePaused) {
                mainPauseMenu = false;
                settingsMenu = true;
            }
            Color quitButtonColor = isHoveringQuit ? LIGHTGRAY : DARKGRAY;
            Color resumeButtonColor = isHoveringResume ? LIGHTGRAY : DARKGRAY;
            Color settingsButtonColor = isHoveringSettings ? LIGHTGRAY : DARKGRAY;
            
            if (gamePaused) {
                if (mainPauseMenu) {
                    int width = 300;
                    int height = 250;
                    DrawRectangle((GetScreenWidth() / 2) - (width / 2), (GetScreenHeight() / 2) - (height / 2), width, height, DARKGRAY);
                    // DrawText(TextFormat("Game Paused"), ((GetScreenWidth() / 2) - (width / 2)) * 1.33, (GetScreenHeight() / 4), 20, BLACK);
                    DrawRectangleRec(quitButton, quitButtonColor);
                    DrawRectangleLines((int)quitButton.x, (int)quitButton.y, (int)quitButton.width, (int)quitButton.height, BLACK);
                    DrawText("Quit", (int)quitButton.x + 30, (int)quitButton.y + 10, 20, WHITE);
    
                    DrawRectangleRec(resumeButton, resumeButtonColor);
                    DrawRectangleLines((int)resumeButton.x, (int)resumeButton.y, (int)resumeButton.width, (int)resumeButton.height, BLACK);
                    DrawText("Resume", (int)resumeButton.x + 15, (int)resumeButton.y + 10, 20, WHITE);
    
                    DrawRectangleRec(settingsButton, settingsButtonColor);
                    DrawRectangleLines((int)settingsButton.x, (int)settingsButton.y, (int)settingsButton.width, (int)resumeButton.height, BLACK);
                    DrawText("Settings", (int)settingsButton.x + 15, (int)settingsButton.y + 10, 20, WHITE);
                }
            }
            if (settingsMenu) {
                int settingsMenuWidth = 500;
                int settingsMenuHeight = 300;
                DrawRectangle((GetScreenWidth() / 2) - (settingsMenuWidth / 2), (GetScreenHeight() / 2) - (settingsMenuHeight / 2), settingsMenuWidth, settingsMenuWidth, DARKGRAY);

                Rectangle exitButton = { (float)(GetScreenWidth() / 2) - 50, (float)(GetScreenHeight() / 2) - 80, 110.0f, 40.0f };
                bool isHoveringExit = CheckCollisionPointRec(mousePoint, exitButton);
                if (isHoveringExit && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gamePaused && settingsMenu) {std::cout<<"Settings Exit"<<std::endl; settingsMenu = false; mainPauseMenu = true;}
                Color exitButtonColor = isHoveringExit ? LIGHTGRAY : DARKGRAY;

                DrawRectangleRec(exitButton, exitButtonColor);
                DrawRectangleLines((int)exitButton.x, (int)exitButton.y, (int)exitButton.width, (int)exitButton.height, BLACK);
                DrawText("exit", (int)exitButton.x + 15, (int)exitButton.y + 10, 20, WHITE);
    
                // enableAdsButton.Draw(50, mousePoint, toggleAds(data["ads-enabled"], data), )
                const char* adsText = enabledAds ? "Ads: On" : "Ads: Off";
                int currentFps = data["fps-limiter"];
                // const char* fpsText = "FPS", fpsMaxAmount;
                enableAdsButton.Draw(adsText, mousePoint, [&](){enabledAds = !enabledAds; toggleAds(enabledAds, data);}, gamePaused, settingsMenu);
                fpsIncreasButton.Draw(TextFormat("FPS: %d", currentFps), mousePoint, [&](){increaseFps(data);}, gamePaused, settingsMenu);

            }
            // DrawRectangle(400 - 5, 225 -5, 10, 10, RED);
            int crosshairX = (GetScreenWidth() / 2) - (crosshairTexture.width / 2);
            int crosshairY = (GetScreenHeight() / 2) - (crosshairTexture.height / 2);
            if (!gamePaused) {DrawTexture(crosshairTexture, crosshairX, crosshairY, BLACK);}
            // int healthBarX = (GetScreenWidth() * -1) + (healthBarTexture.width * 2.9);
            // int healthBarY = (GetScreenHeight() * -1) + (healthBarTexture.height * 2.375);
            float healthBarScale = 1.0f;
            int healthBarX = 20;
            int healthBarY = GetScreenHeight() - (healthBarTexture.height * healthBarScale) + 160;
            DrawTexture(healthBarTexture, healthBarX, healthBarY, WHITE);
            int healthValueY = GetScreenHeight() - (healthBarTexture.height / 4);
            int healthBarHeight = 60;
            for (int i = 0; i < player.healthBarAmount; i++) {
                if (i == 8) {healthBarHeight -= 10;} else if (i == 9) {healthBarHeight -= 25;}
                DrawRectangle(healthBarX + (i * 34.5) + 135, healthValueY, 20, healthBarHeight, { 25, 100, 100, 40 });
            };
            if (enabledAds) {ad.Draw(cokeAdTexture);}
            // std::cout << "Health Bar Y: " << healthBarY << std::endl;
        DrawText(TextFormat("FPS %i", GetFPS()), 10, 30, 20, BLACK);
        DrawText(TextFormat("X: %.3f", player.position.x), 10, 50, 20, BLACK);
        DrawText(TextFormat("Z: %.3f", player.position.z), 10, 70, 20, BLACK);
        DrawText(TextFormat("Y: %.3f", player.position.y), 10, 90, 20, BLACK);
        DrawText(TextFormat("Speed: %.2f", player.speed), 10, 110, 20, BLACK);
        DrawText(TextFormat("Vector Cordinates: { %.2f, %.2f, %.2f }", player.position.x, player.position.y, player.position.z), 10, 130, 17, BLACK);
        // DrawText(TextFormat("Follower Cordinates: { %.2f, %.2f, %.2f }", Follower.position.x, Follower.position.y, Follower.position.z), 10, 150, 17, BLACK);
        DrawText(TextFormat("Entities: %i", allColiders.size()), 10, 170, 20, BLACK);
        // DrawText(TextFormat("Distance: %f", followerLength), 10, 190, 20, BLACK);
        DrawText(TextFormat("Game Paused: %s", gamePaused ? "True" : "False"), 10, 210, 20, BLACK);
        DrawText(TextFormat("Dev Build Number 0.08 - NOT COMPLETE"), 10, 10, 20, BLACK);

    EndDrawing();
    }
    for (chunk& c : worldChunks) {c.Unload();}
    UnloadModel(cubeModel);
    UnloadModel(metalModel);
    UnloadModel(metalGrate);
    UnloadModel(wallModel);
    UnloadModel(windowModel);
    UnloadModel(followerModel);
    UnloadTexture(blockTextures[0]);
    UnloadTexture(blockTextures[1]);
    UnloadTexture(crosshairTexture);
    UnloadTexture(healthBarTexture);
    UnloadTexture(cokeAdTexture);
    UnloadSound(gunShotOne);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}