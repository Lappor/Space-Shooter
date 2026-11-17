#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls
#include "raylib.h"                 //Main Raylib core library
#include "raymath.h"                //Raylib Math functions library
#include "rlgl.h"                   //RayLib OpenGL library
#include <cmath>                    //CMath
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>  

#include "Enemy.h"
#include "Projectile.h"
#include "Player.h"
#include "Button.h"
#include "GameState.h"
#include "EnemyManager.h"

// NOTE: Gamepad name ID depends on drivers and OS
#define XBOX360_LEGACY_NAME_ID  "Xbox Controller"
#define XBOX360_NAME_ID     "Xbox 360 Controller"
#define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

//Create a class for projectiles:
// Define a custom struct

bool checkCollisionCircles(float aX, float aY, float aSize, float bX, float bY, float bSize)
{
    int xDist;
    int yDist;
    xDist = aX - bX;
    if (xDist < 0)        //Get the magnitude of the distance
    {
        xDist = xDist * (-1);
    }

    yDist = aY - bY;    
    if (yDist < 0)        //Get the magnitude of the distance
    {
        yDist = yDist * (-1);
    }

    if (xDist < (aSize + bSize) and yDist < (aSize + bSize))
    {
        return true;
    }
    else
    {
        return false;
    }
}



void main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1300;
    const int screenHeight = 900;

    //Player variables
    Player player(
        screenWidth / 2,                    //newX
        screenHeight / 2,                   //newY
        48,                                 //newSize
        0,                                  //newHit
        3,                                  //newHealth
        3,                                  //newMaxHealth
        6,                                  //newMoveSpeed
        0,                                  //newShotCooldown
        30,                                 //newShotRate
        3,                                  //newAimSpeed 
        18,                                 //newAimDist
        0,                                  //newAimAngleRads
        8,                                  //newAimSize                              
        { 0,0,0,10,180,8,false,5 },           //newShotValues
        10,                                 //newTouchDamage        
        0                                   //newXP
    );

    // Which sprite file the player should use
    const char* playerSpriteFile = "assets/ships/player/player.png";
    std::cout << playerSpriteFile;

    //Health Bar Size
    float healthBarSize = 180;

    //XP Bar Size
    float xpBarSize = 500;

    //Track enemies killed
    float enemiesHit = 0;

    //Track time passed
    int timePassed = 0;

    //Store all collisions
    bool collision = 0;

    //Track kills
    int kills = 0;

    //Set the delay timer
    int delayTimer = 0;

    //Set up inputs for the Display menu.
    char inputA[5] = "";
    char inputB[5] = "";

    //Set up the level system.
    int currentLevel = 1;
    int xpToNextLevel = 50;
    float progressionIncrease = 1.4;

    bool isPressNew = false;
    //Set up the gamestate enum

    GameState gamestate = MAIN_MENU;

    //-----Define the Buttons-----
    #pragma region Buttons
    //Start Button
    Rectangle startRect = {
        screenWidth / 2 - 200,                  //x of top left corner
        300,                                //y of top left corner
        400,                                //width
        100 };                              //height

    Button startButton(
        startRect,          //Rectangle to use
        "Start",            //Text
        48,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Settings Button
    Rectangle settingsRect = {
        screenWidth / 2 - 200,                  //x of top left corner
        450,                                //y of top left corner
        400,                                //width
        100 };                              //height

    Button settingsButton(
        settingsRect,          //Rectangle to use
        "Settings",            //Text
        48,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Quit Button
    Rectangle quitRect = {
        screenWidth / 2 - 200,                  //x of top left corner
        600,                                //y of top left corner
        400,                                //width
        100 };                              //height

    Button quitButton(
        quitRect,          //Rectangle to use
        "Quit",            //Text
        48,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Return Button
    Rectangle returnRect = {
        screenWidth / 2 - 200,                  //x of top left corner
        300,                                //y of top left corner
        400,                                //width
        100 };                              //height

    Button returnButton(
        returnRect,          //Rectangle to use
        "Return",            //Text
        48,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );


    //Screen Size Button
    Rectangle screenSizeRect = {
        screenWidth / 2 - 200,                  //x of top left corner
        450,                                //y of top left corner
        400,                                //width
        100 };                              //height

    Button screenSizeButton(
        screenSizeRect,          //Rectangle to use
        "Change Screen Size",            //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Cheats Button
    Rectangle cheatsRect = {
        screenWidth - 140,                  //x of top left corner
        screenHeight - 100,                                //y of top left corner
        120,                                //width
        80 };                              //height

    Button cheatsButton(
        cheatsRect,          //Rectangle to use
        "Cheats",            //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour

    );
    //Small Return Button
    Rectangle smallReturnRect = {
        screenWidth - 140,                  //x of top left corner
        screenHeight - 100,                                //y of top left corner
        120,                                //width
        80 };                              //height
    Button smallReturnButton(
        smallReturnRect,          //Rectangle to use
        "Return",            //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );


    //Upgrades
    //Damage Upgrade
    Rectangle damageRect = {
        screenWidth / 2 - 400,                  //x of top left corner
        350,                                //y of top left corner
        200,                                //width
        100 };                              //height

    Button damageButton(
        damageRect,          //Rectangle to use
        "Damage Up",            //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Speed Upgrade
    Rectangle speedRect = {
        screenWidth / 2 - 100,                  //x of top left corner
        350,                                //y of top left corner
        200,                                //width
        100 };                              //height

    Button speedButton(
        speedRect,          //Rectangle to use
        "Speed Up",            //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Fire Rate Upgrade
    Rectangle fireRateRect = {
        screenWidth / 2 + 200,                  //x of top left corner
        350,                                //y of top left corner
        200,                                //width
        100 };                              //height

    Button fireRateButton(
        fireRateRect,          //Rectangle to use
        "Fire Rate Up",            //Text
        28,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Health Upgrade
    Rectangle healthRect = {
        screenWidth / 2 - 400,                  //x of top left corner
        500,                                //y of top left corner
        200,                                //width
        100 };                              //height

    Button healthButton(
        healthRect,         //Rectangle to use
        "Health Up",        //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Size Upgrade
    Rectangle sizeRect = {
        screenWidth / 2 - 100,                  //x of top left corner
        500,                                //y of top left corner
        200,                                //width
        100 };                              //height

    Button sizeButton(
        sizeRect,          //Rectangle to use
        "Size Down",            //Text
        32,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );

    //Projectile Speed Upgrade
    Rectangle shotSpeedRect = {
        screenWidth / 2 + 200,                  //x of top left corner
        500,                                //y of top left corner
        200,                                //width
        100 };                              //height

    Button shotSpeedButton(
        shotSpeedRect,          //Rectangle to use
        "Shot Speed Up",            //Text
        28,                 //Font Size
        WHITE,              //Text Colour
        GRAY,               //Current Colour
        GRAY,               //Idle Colour
        BLUE,               //Selected Colour
        LIGHTGRAY,          //Hover Colour
        SKYBLUE             //Selected + Hover Colour
    );
#pragma endregion

    bool quit = false;
    bool spawnDone = false;
    int spawnTimer = 0;

    //Projectiles from Class
    //----------------------------------------------------------------------------------------
    std::vector<Projectile> bullets;        //Declare a vector of class Projectile

    //Enemies from Class
    //----------------------------------------------------------------------------------------
    EnemyManager enemies;                   // Manage enemies and spawning

    //Collision Variables
    float distX = 99999;
    float distY = 99999;

    //Aiming Angle
    float targetAngle = 0;
    Vector2 mousePosition = GetMousePosition();

    InitWindow(screenWidth, screenHeight, "2D Shooter");

    // Load player texture via player's loader (path is relative to "assets/")
    if (!player.loadTextureFromAssets("ships/player/player.png"))
    {
        // Fallback: try direct full path if player's loader failed
        Texture2D t = LoadTexture(playerSpriteFile);
        if (t.id != 0) {
            player.texture = t;
            player.hasTexture = true;
        }
        else {
            player.hasTexture = false;
        }
    }

    // Set per-player rotation offset if the sprite's forward direction differs.
    // Typical values: 0.0f (sprite faces up), -90.0f (sprite faces right), 180.0f (sprite faces down)
    player.textureRotationOffsetDeg = 180.0f; // tune if needed

    // Texture for background
    Texture2D backgroundTexture = LoadTexture("assets/backgrounds/bg_01.png"); // path relative to project root / assets/
    bool hasBackground = (backgroundTexture.id != 0);

    int fps = 60;

    SetTargetFPS(fps);               // Set game to run at 60 frames-per-second

    int gamepad = 0; // which gamepad to display
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose() && quit == false)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        //Update gamestates.

        if (player.health <= 0)
        {
            gamestate = DEAD;
        }

        if (currentLevel > 30)
        {
            gamestate = WIN;
        }

        //Check gamestates.
        BeginDrawing();

        if (hasBackground)
        {
            // Draw the whole background stretched to the screen.
            Rectangle src = { 0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height };
            Rectangle dst = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(backgroundTexture, src, dst, origin, 0.0f, WHITE);
        }
        else
        {
            ClearBackground(RAYWHITE);
        }

        bool isMouseReleased = true;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            isMouseReleased = true;
        }
        if (gamestate == MAIN_MENU or gamestate == SETTINGS_MENU or gamestate == CHEAT_MENU)
        {
            DrawText("Star Shooter", screenWidth / 2 - MeasureText("Star Shooter", 100) / 2, 100, 100, SKYBLUE);
        }

        if (gamestate == MAIN_MENU)
        {
            //Check if the start button has been clicked.

            if (startButton.DrawButton())
            {
                //Set the start button back to false
                startButton.selected = false;

                //Reset the player.
                player.reset(screenWidth, screenHeight);

                //Clear the enemies and projectiles.
                enemies.clear();
                bullets.clear();

                //Set up the level up system.    
                currentLevel = 1;
                xpToNextLevel = 50;
                progressionIncrease = 1.4;

                //Set the timer to 0.
                timePassed = 0;

                //Set the delay timer to 3 seconds.
                delayTimer = 3 * fps;

                //Set the gamestate to starting
                gamestate = STARTING;
            }

            if (settingsButton.DrawButton())
            {
                settingsButton.selected = false;
                screenSizeButton.selected = true;            //Inverts the selection of the screen size button. This is a dirty fix, but it is one.
                gamestate = SETTINGS_MENU;
            }

            quit = quitButton.DrawButton();
        }

        if (gamestate == SETTINGS_MENU)
        {
            if (returnButton.DrawButton())
            {
                returnButton.selected = false;
                gamestate = MAIN_MENU;
            }

            if (screenSizeButton.DrawButton())
            {
                screenSizeButton.text = "Not yet implemented";
            }
            else
            {
                screenSizeButton.text = "Change Screen Size";
            }
            /*
            if (cheatsButton.DrawButton())
            {
                cheatsButton.selected = false;                      //Same dirty fix as earlier.
                smallReturnButton.selected = true;
                gamestate = CHEAT_MENU;
            }
            */
        }
        /*
        if (gamestate == CHEAT_MENU)
        {
            DrawText("2D Shooter", screenWidth / 2 - MeasureText("2D Shooter", 100) / 2, 100, RED);

            DrawText("Cheats", screenWidth / 2 - MeasureText("Cheats", 100) / 2, 220, 80, RED);
            //Infinite Health


            //Infinite Damage


            //Hyperspeed


            //Skip to Level X


            //Return
            if (smallReturnButton.DrawButton())
            {
                smallReturnButton.selected = false;
                gamestate = SETTINGS_MENU;
            }
        }
        */
        if (gamestate == WIN)
        {

            DrawText("You Won!", (screenWidth - MeasureText("You Won!", 100)) / 2, screenHeight / 2 - 200, 100, BLACK);

            //Display the number of kills

            std::string label = "Number of enemies killed: ";
            std::string value = std::to_string(kills);

            int labelWidth = MeasureText(label.c_str(), 60);
            int valueWidth = MeasureText(value.c_str(), 60);
            int totalWidth = labelWidth + valueWidth;

            int startX = (screenWidth - totalWidth) / 2;
            int yText = screenHeight / 2 - 60;

            //Draw label
            DrawText(label.c_str(), startX, yText, 60, BLACK);

            //Draw kill number
            DrawText(value.c_str(), startX + labelWidth, yText, 60, BLACK);

            //
            DrawText("Press Space to return to the main menu", (screenWidth - MeasureText("Press Space to return to the main menu", 40)) / 2, screenHeight / 2 + 100, 40, BLACK);

        }

        if (gamestate == PAUSE)
        {
            DrawText("Pause", (screenWidth - MeasureText("Pause", 100)) / 2, screenHeight / 2 - 200, 100, BLACK);

            if (IsKeyReleased(KEY_P))
            {
                isPressNew = true;
            }

            if (IsKeyPressed(KEY_P) && isPressNew == true)
            {
                isPressNew = false;
                delayTimer = 3 * fps;
                gamestate = STARTING;
            }

        }

        if (gamestate == GAME)
        {
            timePassed += 1;
            //Update Variables
            if (player.hit > 0)
            {
                player.hit += -1;
            }

            //---------------Controls---------------
            //Aiming
            //-------------------------------------------------------------------------
            mousePosition = GetMousePosition();
            player.aimAngleRads = atan2(mousePosition.x - player.x, mousePosition.y - player.y);
            //-------------------------------------------------------------------------

            //Set up WASD
            if (IsKeyDown(KEY_W))
            {
                player.y -= player.moveSpeed;
                if (player.y < player.size)
                {
                    player.y = player.size;
                }
            }
            if (IsKeyDown(KEY_S))
            {
                player.y += player.moveSpeed;
                if (player.y > screenHeight - (player.size))
                {
                    player.y = screenHeight - (player.size);
                }
            }
            if (IsKeyDown(KEY_A))
            {
                player.x -= player.moveSpeed;
                if (player.x < player.size)
                {
                    player.x = player.size;
                }
            }
            if (IsKeyDown(KEY_D))
            {
                player.x += player.moveSpeed;
                if (player.x > screenWidth - player.size)
                {
                    player.x = screenWidth - (player.size);
                }
            }

            if (IsKeyReleased(KEY_P))
            {
                isPressNew = true;
            }

            if (IsKeyPressed(KEY_P) && isPressNew == true)
            {
                isPressNew = false;
                gamestate = PAUSE;
            }

            //Click to shoot
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) or IsKeyDown(KEY_SPACE))
            {
                if (player.hit <= 0 && player.shotCooldown == 0)
                {
                    bullets.push_back(player.shoot());
                    player.shotCooldown = player.shotRate;
                }
            }

            //Enemy AI
            //-------------------------------------------------------------------------------------------
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].health > 0 and enemies[i].spawned > 0)
                {
                    enemies[i] = enemies[i].moveAI(enemies[i], player.x, player.y, 0, screenWidth, 0, screenHeight);

                    //Enemy Aiming
                    enemies[i].aimAngle = enemies[i].aimAI(enemies[i], player.x, player.y);

                    //Enemy Shooting

                    if (enemies[i].shootAICheck(enemies[i], player.x, player.y) == true)
                    {
                        bullets.push_back(enemies[i].shoot());
                        enemies[i].cooldown = enemies[i].shootRate;
                    }
                    else
                    {
                        enemies[i].cooldown += -1;
                    }

                }
                else if (enemies[i].spawned > 0)
                {
                    enemies[i].spawned += -1;
                }

            }

            //Enemy Spawning
            //----------------------------------------------------------------

#pragma region oldEnemySpawning
			//Currently disabled, but this is the old enemy spawning system. It was replaced with a more robust system that allows for more control over the spawning of enemies.
            if (0) {
                if ((currentLevel <= 9 && timePassed % 120 == 0) ||
                    (currentLevel > 10 && currentLevel <= 19 && timePassed % 90 == 0) ||
                    (currentLevel > 20 && currentLevel <= 29 && timePassed % 60 == 0))
                {
                    if (spawnDone == false)
                    {
                        if (currentLevel <= 9)
                        {
                            // Roamers
                            for (int n = 0; n < ((currentLevel - 1) / 2) + 1; n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Roamer);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }

                            // Chargers
                            for (int n = 0; n < (currentLevel - 4) / 2; n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Charger);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }

                            // Shooters
                            for (int n = 0; n < ((currentLevel) / 6) / 2; n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Shooter);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }
                        }
                        else if (currentLevel > 10 && currentLevel <= 19)
                        {
                            for (int n = 0; n < (((currentLevel - 11) / 2) + 1) % 5 + 1; n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Leaper);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }

                            for (int n = 0; n < (currentLevel - 16); n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Sniper);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }
                        }
                        else if (currentLevel > 20 && currentLevel <= 29)
                        {
                            for (int n = 0; n < ((currentLevel - 21) / 2) + 1; n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Gunner);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }

                            for (int n = 0; n < (currentLevel - 27) * 4; n++)
                            {
                                auto it = Enemy::enemyStatsTable.find(EnemyType::Swarmer);
                                if (it != Enemy::enemyStatsTable.end())
                                {
                                    Enemy newEnemy = it->second;
                                    newEnemy.x = 100; newEnemy.y = 150;
                                    enemies.spawn(newEnemy, /*randomizePos=*/false);
                                }
                            }
                        }
                    }
                }
            }
#pragma endregion


#pragma region EnemySpawning
            //Enemy wave spawning
            
			//Check if it's been the correct amount of time to spawn a new wave of enemies based on the current level.
            bool spawnIntervalHit =
                ((currentLevel <= 9 && timePassed % 120 == 0) ||
                    (currentLevel > 10 && currentLevel <= 19 && timePassed % 90 == 0) ||
                    (currentLevel > 20 && timePassed % 60 == 0));

			//If it's a boss wave or if the spawn interval has been hit, and the spawn is not done, then spawn.
            if ((spawnIntervalHit || currentLevel % 10 == 0) && spawnDone == false)
            {

                // First, if it is a boss wave, show a warning for 5 seconds, then spawn the boss.
                if ((currentLevel == 10 || currentLevel == 20 || currentLevel == 30) && spawnDone == false)
                {
                    // Draw warning for the player for 5 seconds before the actual spawn
                    if (timePassed % 300 != spawnTimer)
                    {
                        DrawText("!!!Boss Spawning!!!",
                            (screenWidth - MeasureText("!!!Boss Spawning!!!", 100)) / 2,
                            screenHeight / 2,
                            100, RED);
                    }
                    else
                    {
                        EnemyType bossType;
                        if (currentLevel == 10) bossType = EnemyType::BOSS_Giant;
                        else if (currentLevel == 20) bossType = EnemyType::BOSS_Drifter;
                        else /* if (currentLevel == 30) */ bossType = EnemyType::BOSS_Dragon;

                        auto it = Enemy::enemyStatsTable.find(bossType);
                        if (it != Enemy::enemyStatsTable.end())
                        {
                            Enemy newEnemy = it->second; // copy template

                            // Keep the boss at a chosen corner relative to the player
                            if (player.x > screenWidth / 2) newEnemy.x = 150;
                            else newEnemy.x = screenWidth - 150;

                            if (player.y > screenHeight / 2) newEnemy.y = 150;
                            else newEnemy.y = screenHeight - 150;

                            // Spawn (no randomization)
                            enemies.spawn(newEnemy, /*randomizePos=*/false);

                        }

                        // Mark the boss wave done so it doesn't spawn another
                        spawnDone = true;
                    }
                }
                //If it is not a boss wave, spawn enemies until there are enough enemies for the player to level up.
                else {
                    int targetXP = xpToNextLevel * 1.2; // How much should spawn
					int accumulatedXP = 0;              // How much has been spawned so far

                    // Gather eligible non-boss types for this level and find the cheapest cost
                    std::vector<EnemyType> eligible;
                    int cheapestCost = INT_MAX;
                    for (auto &kv : Enemy::enemyStatsTable)
                    {
                        const Enemy &tmpl = kv.second;
                        if (tmpl.isBoss) continue;
                        if (tmpl.minSpawnLevel > currentLevel) continue;

                        int cost = tmpl.xpValue > 0 ? tmpl.xpValue : 1; // xpValue is used as spawn cost
                        eligible.push_back(kv.first);
                        if (cost < cheapestCost) cheapestCost = cost;
                    }

                    // Nothing to spawn
                    if (eligible.empty() || cheapestCost == INT_MAX)
                    {
                        spawnDone = true;
                    }
                    else
                    {
                        // Debug counter
                        int spawnedThisWave = 0;

                        // Keep spawning until accumulatedXP >= targetXP
                        while (accumulatedXP < targetXP)
                        {
                            int need = targetXP - accumulatedXP;

                            // Find types that fit the remaining need
                            std::vector<EnemyType> fitList;
                            for (auto &t : eligible)
                            {
                                int c = Enemy::enemyStatsTable.at(t).xpValue;
                                if (c <= need) fitList.push_back(t);
                            }

                            EnemyType choice;
                            int cost;

                            if (!fitList.empty())
                            {
                                // Prefer a random fitting enemy
                                choice = fitList[GetRandomValue(0, (int)fitList.size() - 1)];
                                cost = Enemy::enemyStatsTable.at(choice).xpValue;
                            }
                            else
                            {
                                // No fitting type — select the cheapest eligible (will overshoot and end loop)
                                choice = eligible[0];
                                cost = Enemy::enemyStatsTable.at(choice).xpValue;
                                for (auto &t : eligible)
                                {
                                    int c = Enemy::enemyStatsTable.at(t).xpValue;
                                    if (c < cost)
                                    {
                                        choice = t;
                                        cost = c;
                                    }
                                }
                            }

                            // Create the enemy from the template
                            Enemy newEnemy = Enemy::enemyStatsTable.at(choice);

                            // Do NOT change xpValue: keep xpValue equal to template so accumulatedXP matches spawn reward
                            // Apply variation only to non-XP fields
                            float var = 0.95f + GetRandomValue(0, 10) / 100.0f; // smaller variation
                            newEnemy.health = std::max(1, (int)(newEnemy.health * var));
                            newEnemy.maxHealth = newEnemy.health;
                            newEnemy.speed *= var;
                            // newEnemy.xpValue = Enemy::enemyStatsTable.at(choice).xpValue; // ensure unchanged

                            // Spawn at random valid location
                            enemies.spawn(newEnemy, /*randomizePos=*/true, 0, screenWidth, 0, screenHeight, player.x, player.y);

                            accumulatedXP += cost;
                            ++spawnedThisWave;

                            // Safety: if a cost is zero or negative for some reason, break to avoid infinite loop
                            if (cost <= 0) break;
                            // Another safety: max spawn cap (protect against pathological templates)
                            if (spawnedThisWave > 200) break;
                        }

                        // Optional debug draw while testing:
                        // DrawText(("Spawned: " + std::to_string(spawnedThisWave)).c_str(), 10, 60, 20, RED);
                        // DrawText(("AccumXP: " + std::to_string(accumulatedXP) + "/" + std::to_string(targetXP)).c_str(), 10, 80, 20, RED);

                        spawnDone = true;
                    }
                }
            }
#pragma endregion

            //----------Projectiles----------
            //Check each entry in the bullets vector.
            for (int i = 0; i < bullets.size(); i++) {
                if (bullets[i].lifespan > 0) {
                    bullets[i] = bullets[i].move(bullets[i]);
                }
                //If the lifespan is at 0, delete the bullet.
                else
                {
                    bullets.erase(bullets.begin() + i, bullets.begin() + i + 1);
                }
            }

            //------------------------------Colliision Checks------------------------------
            //Check for collision between Enemy and Players
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].health > 0 and player.hit == 0)
                {
                    collision = checkCollisionCircles(enemies[i].x, enemies[i].y, enemies[i].size, player.x, player.y, player.size);
                    if (collision == true)
                    {
                        player.hit = 10;
                        player.health -= enemies[i].touchDamage;
                        enemies[i].health -= player.touchDamage;
                        if (enemies[i].health <= 0)
                        {
                            kills += 1;
                        }
                    }
                }
            }

            //----------Check for collision between the target and the projectile----------

            //Check for collision for bullets
            for (int i = 0; i < enemies.size(); i++)
            {
                for (int n = 0; n < bullets.size(); n++)
                {
                    //Player bullets with enemy
                    if (bullets[n].isPlayerTarget == 0 && enemies[i].health > 0 && bullets[n].lifespan > 0)
                    {
                        collision = checkCollisionCircles(bullets[n].x, bullets[n].y, bullets[n].size,
                            enemies[i].x, enemies[i].y, enemies[i].size);
                        if (collision == true)
                        {
                            int temp = enemies[i].health;
                            enemies[i].health -= bullets[n].damage;
                            bullets[n].damage -= temp;
                            if (bullets[n].damage <= 0)
                            {
                                bullets[n].lifespan = 0;
                            }
                            if (enemies[i].health <= 0)
                            {
                                kills += 1;
                                player.xp += enemies[i].xpValue;
                            }
                        }
                    }
                    //Enemy bullets with player
                    else if (bullets[n].isPlayerTarget == 1 && player.hit <= 0 && bullets[n].lifespan > 0)
                    {
                        collision = checkCollisionCircles(bullets[n].x, bullets[n].y, bullets[n].size,
                            player.x, player.y, player.size);
                        if (collision == true)
                        {
                            player.hit = 10;
                            bullets[n].lifespan = 0;
                            player.health -= bullets[n].damage;
                        }
                    }
                }
            }
            //--------------------------------

            if (player.shotCooldown > 0)
            {
                player.shotCooldown += -1;
            }

            if (player.xp >= xpToNextLevel)
            {
                player.xp = 0;
                currentLevel += 1;
                xpToNextLevel *= progressionIncrease;

                gamestate = LEVEL_UP;
            }

        }

        //---------------------Draw---------------------------------
        if (gamestate == GAME or gamestate == DEAD or gamestate == STARTING or gamestate == LEVEL_UP or gamestate == PAUSE)
        {
            //Draw the Player
            if (player.hit <= 0)
            {
                if (player.hasTexture && player.texture.id != 0)
                {
                    // Source is entire texture
                    Rectangle src = { 0.0f, 0.0f, (float)player.texture.width, (float)player.texture.height };
                    // Destination size: use player.size as radius, so width/height = size*2
                    Rectangle dst = { (float)player.x, (float)player.y, (float)player.size * 2.0f, (float)player.size * 2.0f };
                    Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };
                    // Single consistent rotation: aim angle + user-configurable offset
                    float rotationDeg = -player.aimAngleRads * (180.0f / PI) + player.textureRotationOffsetDeg;

                    DrawTexturePro(player.texture, src, dst, origin, rotationDeg, WHITE);

                    // When textured ship is visible, hide the separate aiming icon (it's redundant).
                }
                else
                {
                    DrawCircle(player.x, player.y, player.size, GREEN); //Draw the player (fallback)
                    // Draw the crosshair for non-textured player
                    DrawCircle(player.x + (player.size * 1.5f) * sin(player.aimAngleRads), player.y + (player.size * 1.5f) * cos(player.aimAngleRads), 8, GREEN); //Draw the crosshair
                }
            }
            else
            {
                if (player.hasTexture && player.texture.id != 0)
                {
                    Rectangle src = { 0.0f, 0.0f, (float)player.texture.width, (float)player.texture.height };
                    Rectangle dst = { (float)player.x, (float)player.y, (float)player.size * 2.0f, (float)player.size * 2.0f };
                    Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };
                    // Keep the same offset during "hit" rendering
                    float rotationDeg = -player.aimAngleRads * (180.0f / PI) + player.textureRotationOffsetDeg;

                    DrawTexturePro(player.texture, src, dst, origin, rotationDeg, Color{ 0, 228, 48, 150 });
                }
                else
                {
                    DrawCircle(player.x, player.y, player.size, Color{ 0, 228, 48, 150 }); //Draw the respawning player
                }
            }

            //Draw the enemies and their crosshairs
            for (int i = 0; i < enemies.size(); i++) {
                if (enemies[i].health > 0 && enemies[i].spawned > 0)
                {
                    if (enemies[i].hasTexture())
                    {
                        // Draw texture with same rotation convention as player
                        Texture2D& tex = *enemies[i].texturePtr;
                        Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
                        Rectangle dst = { (float)enemies[i].x, (float)enemies[i].y, (float)enemies[i].size * 2.0f, (float)enemies[i].size * 2.0f };
                        Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };
                        float rotationDeg = -enemies[i].aimAngle * (180.0f / PI) + enemies[i].textureRotationOffsetDeg;
                        DrawTexturePro(tex, src, dst, origin, rotationDeg, WHITE);

                        // No need to draw crosshair - the ship shows where its aiming.
                        //DrawCircle(...);
                    }
                    else
                    {
                        DrawCircle(enemies[i].x, enemies[i].y, enemies[i].size, enemies[i].colorAlive); // original fallback
                        DrawCircle(enemies[i].x + ((enemies[i].size * 1.5) * sin(enemies[i].aimAngle)), enemies[i].y + (enemies[i].size * 1.5) * cos(enemies[i].aimAngle), 8, enemies[i].colorAlive);    //Draw the crosshair
                    }

                }
                else if (enemies[i].spawned > 0)
                {
                    DrawCircle(enemies[i].x, enemies[i].y, enemies[i].sizeDead, enemies[i].colorDead);
                }
                else
                {
                    //enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
                }
            }

            //Draw the bullets
            for (int i = 0; i < bullets.size(); i++) {
                if (bullets[i].isPlayerTarget == 0 and bullets[i].lifespan > 0)
                {
                    DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size, GREEN);
                }
                else if (bullets[i].lifespan > 0)
                {
                    DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size, RED);
                }
            }

            //Draw the healthbar
            DrawRectangle(10 + healthBarSize * (player.health / player.maxHealth), 10, healthBarSize * ((player.maxHealth - player.health) / player.maxHealth), 20, { 80, 80, 80, 200 });
            DrawRectangle(10, 10, healthBarSize * (player.health / player.maxHealth), 20, { 0,117, 44, 200 });

            //Draw the XP Bar, or the Boss Health Bar on Boss Waves.
            if (currentLevel % 10 == 0)
            {
                if (enemies.size() > 0) {
                    DrawRectangle(((screenWidth - xpBarSize) / 2) + xpBarSize * (float(enemies[0].health) / float(enemies[0].maxHealth)), 10, xpBarSize - (xpBarSize * (float(enemies[0].health) / float(enemies[0].maxHealth))), 40, { 115, 20, 30, 200 });

                    DrawRectangle((screenWidth - xpBarSize) / 2, 10, xpBarSize * (float((enemies[0].health)) / float(enemies[0].maxHealth)), 40, { 0,121, 241, 200 });

                }
            }
            else {
                DrawRectangle(((screenWidth - xpBarSize) / 2) + xpBarSize * (float(player.xp) / float(xpToNextLevel)), 10, xpBarSize - (xpBarSize * (float(player.xp) / float(xpToNextLevel))), 40, { 80, 80, 80, 200 });

                DrawRectangle((screenWidth - xpBarSize) / 2, 10, xpBarSize * (float(player.xp) / float(xpToNextLevel)), 40, { 0,121, 241, 200 });
            }
        }

        if (gamestate == STARTING)
        {
            if (delayTimer <= 0)
            {
                gamestate = GAME;
            }
            delayTimer -= 1;

            DrawText(std::to_string(1 + delayTimer / fps).c_str(), (screenWidth - MeasureText(std::to_string(1 + delayTimer / fps).c_str(), 120)) / 2, screenHeight / 2 - 120, 120, BLACK);
        }

        if (gamestate == LEVEL_UP)
        {
            //Display 'Level up!' at the top, and 'Level x' below it.
            DrawText("Level up!", (screenWidth - (MeasureText("Level up!", 100))) / 2, 30, 100, BLUE);

            std::string label = "Level: ";
            std::string value = std::to_string(currentLevel);

            int labelWidth = MeasureText(label.c_str(), 60);
            int valueWidth = MeasureText(value.c_str(), 60);
            int totalWidth = labelWidth + valueWidth;

            int startX = (screenWidth - totalWidth) / 2;
            int yText = 120;

            //Draw level text
            DrawText(label.c_str(), startX, yText, 60, BLACK);

            //Draw level number
            DrawText(value.c_str(), startX + labelWidth, yText, 60, BLACK);

            if (damageButton.DrawButton())
            {
                damageButton.selected = false;
                player.shotValues.damage *= 1.2;
                player.health = player.maxHealth;
                enemies.clear();
                bullets.clear();
                gamestate = STARTING;
            }

            if (player.moveSpeed <= 10)
            {
                if (speedButton.DrawButton())
                {
                    speedButton.selected = false;
                    player.moveSpeed *= 1.2;
                    player.health = player.maxHealth;
                    enemies.clear();
                    bullets.clear();
                    gamestate = STARTING;
                }
            }

            if (player.shotRate >= 1)
            {
                if (fireRateButton.DrawButton())
                {
                    fireRateButton.selected = false;
                    player.shotRate *= 0.8;
                    player.health = player.maxHealth;
                    enemies.clear();
                    bullets.clear();
                    gamestate = STARTING;
                }
            }

            if (healthButton.DrawButton())
            {
                healthButton.selected = false;
                player.maxHealth *= 1.5;
                player.health = player.maxHealth;
                enemies.clear();
                bullets.clear();
                gamestate = STARTING;
            }

            if (player.size >= 12)
            {
                if (sizeButton.DrawButton())
                {
                    sizeButton.selected = false;
                    player.size *= 0.8;
                    player.health = player.maxHealth;
                    enemies.clear();
                    bullets.clear();
                    gamestate = STARTING;
                }
            }

            if (player.shotValues.speed <= 20)
            {
                if (shotSpeedButton.DrawButton())
                {
                    shotSpeedButton.selected = false;
                    player.shotValues.speed *= 1.2;
                    player.health = player.maxHealth;
                    enemies.clear();
                    bullets.clear();
                    gamestate = STARTING;
                }
            }
            //Set the delay timer to 3 seconds.
            delayTimer = 3 * fps;

            spawnDone = false;

            spawnTimer = (timePassed - 1) % 300;
        }

        if (gamestate == DEAD)
        {

            if (IsKeyPressed(KEY_SPACE))
            {
                player.health = 3;
                gamestate = MAIN_MENU;
            }
            DrawText("You Died!", (screenWidth - MeasureText("You Died!", 100)) / 2, screenHeight / 2 - 200, 100, BLACK);

            //Display the number of kills

            std::string label = "Number of enemies killed: ";
            std::string value = std::to_string(kills);

            int labelWidth = MeasureText(label.c_str(), 60);
            int valueWidth = MeasureText(value.c_str(), 60);
            int totalWidth = labelWidth + valueWidth;

            int startX = (screenWidth - totalWidth) / 2;
            int yText = screenHeight / 2 - 60;

            //Draw label
            DrawText(label.c_str(), startX, yText, 60, BLACK);

            //Draw kill number
            DrawText(value.c_str(), startX + labelWidth, yText, 60, BLACK);

            //
            DrawText("Press Space to return to the main menu", (screenWidth - MeasureText("Press Space to return to the main menu", 40)) / 2, screenHeight / 2 + 100, 40, BLACK);

        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload player texture if loaded
    if (player.hasTexture) {
        UnloadTexture(player.texture);
    }

    player.unloadTexture();

    // At cleanup (before CloseWindow()), unload the background if loaded:
    if (hasBackground)
    {
        UnloadTexture(backgroundTexture);
    }

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}