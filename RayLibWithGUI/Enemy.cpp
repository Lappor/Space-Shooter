#include "Enemy.h"
#include <cmath>                    //CMath
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <map>
#include <vector>
#include <memory>
#include <filesystem>
namespace fs = std::filesystem;


// Define static cache
std::unordered_map<std::string, std::shared_ptr<Texture2D>> Enemy::s_textureCache;

Enemy::Enemy(int newSpawned, std::string newAI, bool newIsBoss, int newX, int newY, float newSpeed, float newMoveAngle, int newMovementLength, int newMovementTime,
    int newPauseLength, int newPauseTime, float newAimSpeed, float newAimAngle, float newSize, float newSizeDead, Color newColorAlive, Color newColorDead, 
    bool newHit, int newHealth, int newMaxHealth, int newShootRate, int newShotCooldown, Projectile newShotValues, int newTouchDamage, int newXPValue, int newMinSpawnLevel)
{
    spawned = newSpawned;
    AI = newAI;
    isBoss = newIsBoss;

    x = newX;
    y = newY;

    speed = newSpeed;
    moveAngle = newMoveAngle;

    movementLength = newMovementLength;
    movementTime = newMovementTime;
    pauseLength = newPauseLength;
    pauseTime = newPauseTime;

    aimSpeed = newAimSpeed;
    aimAngle = newAimAngle;
    
    size = newSize;
    sizeDead = newSizeDead;
    colorAlive = newColorAlive;
    colorDead = newColorDead;

    dead = newHit;
    health = newHealth;
    maxHealth = newMaxHealth;

    shootRate = newShootRate;
    cooldown = newShotCooldown;
    shotValues = newShotValues;

    touchDamage = newTouchDamage;

    xpValue = newXPValue;

    minSpawnLevel = newMinSpawnLevel;
}

// Default constructor: set safe zero/neutral defaults
Enemy::Enemy()
    : spawned(0), AI(""), isBoss(false),
      x(0.0f), y(0.0f),
      speed(0.0f), moveAngle(0.0f),
      movementLength(0), movementTime(0), pauseLength(0), pauseTime(0),
      aimSpeed(0.0f), aimAngle(0.0f),
      size(0.0f), sizeDead(0.0f),
      colorAlive{ 0, 0, 0, 255 }, colorDead{ 0, 0, 0, 255 },
      dead(false), health(0), maxHealth(0),
      shootRate(0), cooldown(0),
      shotValues{ 0, 0, 0, 0, 0, 0, 0, 0 },
      touchDamage(0), xpValue(0),
      minSpawnLevel(1),
      texturePtr(nullptr), textureRotationOffsetDeg(0.0f),
      textureAssetPath()
{
}



//Determines where the AI should move.

Enemy Enemy::coreAI(Enemy enemy, int playerX, int playerY, int minX, int maxX, int minY, int maxY)
{
    enemy = enemy.moveAI(enemy, playerX, playerY, minX, maxX, minY, maxY);

    enemy.aimAngle = enemy.aimAI(enemy, playerX, playerY);

    return enemy;
}

Enemy Enemy::moveAI(Enemy enemy, int playerX, int playerY, int minX, int maxX, int minY, int maxY)
{
    //Check if the enemy is alive and exists
    if (enemy.dead <= 0 and enemy.spawned > 0)
    {
        //Enemy Movement
        //Move in the direction needed until its moved for the correct length of time.
        if (enemy.movementTime < enemy.movementLength)
        {
            //Move in the angle the enemy is supposed to move.
            enemy.x += enemy.speed * sin(enemy.moveAngle);
            enemy.y += enemy.speed * cos(enemy.moveAngle);

            //Correct position - prevent moving off the edge of the map
            if (enemy.x < minX + enemy.size)
            {
                enemy.x = minX + enemy.size;
            }
            else if (enemy.x > maxX - enemy.size)
            {
                enemy.x = maxX - enemy.size;
            }

            if (enemy.y < minY + enemy.size)
            {
                enemy.y = minY + enemy.size;
            }
            else if (enemy.y > maxY - enemy.size)
            {
                enemy.y = maxY - enemy.size;
            }

            enemy.movementTime += 1;
        }
        //Wait the pause period.
        else if (enemy.pauseTime < enemy.pauseLength)
        {
            enemy.pauseTime += 1;
        }
        //Determine the new move direction.
        else
        {
            if (enemy.AI == "BasicRanged")
            {
                enemy.moveAngle = float((rand() % 360)) * 3.14 / float(180);
                enemy.pauseTime = 0;
                enemy.movementTime = 0;
            }
            else if (enemy.AI == "Charger")
            {
                enemy.moveAngle = enemy.aimAngle;
                enemy.pauseTime = 0;
                enemy.movementTime = 0;
            }
            if (enemy.AI == "SmartRanged")
            {
                float randX = 0.0f;
                float randY = 0.0f;

                do {
                    // Choose a point *anywhere on screen* but away from player
                    randX = GetRandomValue(minX, maxX - enemy.size);
                    randY = GetRandomValue(minY, maxY - enemy.size);

                    // Optional padding from player or central area
                } while (
                    (
                    (fabs(randX - playerX) <= 100 && fabs(randY - playerY) <= 100)
                    ));

                enemy.moveAngle = atan2(float(randX) - float(enemy.x), float(randY) - float(enemy.y));
                enemy.pauseTime = 0;
                enemy.movementTime = 0;
            }

            else if (enemy.AI == "BOSS_Giant")
            {
                enemy.moveAngle = atan2(float(playerX) - float(enemy.x), float(playerY) - float(enemy.y));
                enemy.pauseTime = 0;
                enemy.movementTime = 0;
            }

            else if (enemy.AI == "BOSS_Drifter")
            {
                enemy.moveAngle = atan2(float(playerX) - float(enemy.x), float(playerY) - float(enemy.y));
                enemy.pauseTime = 0;
                enemy.movementTime = 0;
            }

            else if (enemy.AI == "BOSS_Dragon")
            {

            }

        }
    }
    else if (enemy.spawned > 0)
    {
        enemy.spawned += -1;
    }

    return enemy;
}

//Determines where the AI should be aiming.
float Enemy::aimAI(Enemy enemy, int targetX, int targetY)
{
    float targetAngle = atan2(float(targetX) - float(enemy.x), float(targetY) - float(enemy.y));
    float angleDiff = targetAngle - enemy.aimAngle;

    // Normalize the angle difference to the range [-pi, pi]
    while (angleDiff > PI) angleDiff -= 2 * PI;
    while (angleDiff < -PI) angleDiff += 2 * PI;

    // Adjust aim angle by aimSpeed in the correct direction
    if (abs(angleDiff) > enemy.aimSpeed)
    {
        if (angleDiff > 0)
            enemy.aimAngle += enemy.aimSpeed;
        else
            enemy.aimAngle -= enemy.aimSpeed;
    }
    else
    {
        //If its close enough to snap directly, do so
        enemy.aimAngle = targetAngle;
    }
    return enemy.aimAngle;
}

//Determines whether the AI can shoot.
bool Enemy::shootAICheck(Enemy enemy, float targetX, float targetY)
{
    float targetAngle = atan2(float(targetX) - float(enemy.x), float(targetY) - float(enemy.y));
    if (enemy.cooldown <= 0)
    {
        if (abs(enemy.aimAngle - targetAngle) < enemy.aimSpeed)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//Determines the random spawn location of an enemy.
Enemy Enemy::randomSpawnLoc(Enemy enemy, int minX, int maxX, int minY, int maxY, int playerX, int playerY)
{
    float randX = 0.0f;
    float randY = 0.0f;

    do {
        // Choose a point *anywhere on screen* but away from player
        randX = GetRandomValue(minX, maxX - enemy.size);
        randY = GetRandomValue(minY, maxY - enemy.size);

        // Optional padding from player or central area
    } while (
        ((randX > 100 && randX < maxX - 100) &&
            (randY > 100 && randY < maxY - 100)) ||
        (fabs(randX - playerX) <= 100 && fabs(randY - playerY) <= 100)
        );

    float randMoveAngle = float(rand() % 360) * PI / float(180);
    float randAimAngle = float(rand() % 360) * PI / float(180);

    enemy.x = randX;
    enemy.y = randY;
    enemy.moveAngle = randMoveAngle;
    enemy.aimAngle = randAimAngle;
    return enemy;
}

// Make shoot a member (no copy)
Projectile Enemy::shoot()
{
    Projectile newProjectile = Projectile(aimAngle,
        x + (size) * sin(aimAngle),
        y + (size) * cos(aimAngle),
        shotValues.speed, shotValues.lifespan,
        shotValues.size, shotValues.isPlayerTarget, shotValues.damage);
    return newProjectile;
}

// Load texture for this enemy instance (path relative to "assets/")
bool Enemy::loadTextureFromAssets(const std::string& filename)
{
    if (filename.empty()) return false;

    // Reuse existing cached texture if present
    auto cacheIt = s_textureCache.find(filename);
    if (cacheIt != s_textureCache.end())
    {
        texturePtr = cacheIt->second;
        return true;
    }

    std::string path = std::string("assets/") + filename;
    if (!fs::exists(path)) {
        std::cerr << "Enemy::loadTextureFromAssets - file not found: " << path << std::endl;
        return false;
    }

    Texture2D t = LoadTexture(path.c_str());
    if (t.id == 0) {
        std::cerr << "Enemy::loadTextureFromAssets - LoadTexture failed: " << path << std::endl;
        return false;
    }

    // Allocate on heap and create shared_ptr with deleter to unload texture exactly once
    std::shared_ptr<Texture2D> ptr(new Texture2D(t), [](Texture2D* p) {
        if (p && p->id != 0) UnloadTexture(*p);
        delete p;
    });

    // Cache and assign
    s_textureCache[filename] = ptr;
    texturePtr = ptr;

    std::cout << "Loaded texture: " << path << std::endl;
    return true;
}

void Enemy::ensureTextureLoaded()
{
    if (!textureAssetPath.empty() && (!texturePtr || texturePtr->id == 0))
    {
        if (!loadTextureFromAssets(textureAssetPath))
        {
            std::cerr << "ensureTextureLoaded failed for: " << textureAssetPath << std::endl;
        }
    }
}

// Local/internal to this file
std::map<EnemyType, Enemy> Enemy::enemyStatsTable = {
    { EnemyType::Roamer,    {
    300,                                    //Spawned
    "BasicRanged",                          //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    1,                                      //Speed
    0,                                      //Move Angle
    120,                                    //Movement Length
    0,                                      //Movement Time
    30,                                     //Pause Length
    0,                                      //Pause Time
    0.01f,                                  //Aim Speed
    0,                                      //Aim Angle
    40,                                     //Size
    12,                                     //Size (Dead)
    RED,
    GRAY,
    false,                                  //Dead
    50,                                       //Health    
    50,                                       //Max Health
    480,                                    //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    3,                                      //Speed
    180,                                    //Lifespan
    6,                                      //Size
    true,                                   //Targets Player?
    10                                       //Damage
    },                                      //---------------------
    1,                                      //Touch Damage
    10,                                     //XP Value
    1                                       //Min Spawn Level
    } },

    { EnemyType::Charger,    {
    300,                                    //Spawned
    "Charger",                              //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    3,                                      //Speed
    0,                                      //Move Angle
    120,                                    //Movement Length
    120,                                    //Movement Time
    30,                                     //Pause Length
    0,                                      //Pause Time
    1.0f,                                   //Aim Speed
    0,                                      //Aim Angle
    48,                                     //Size
    16,                                     //Size (Dead)
    LIGHTGRAY,
    GRAY,
    false,                                  //Dead
    160,                                    //Health    
    160,                                    //Max Health
    9999999,                                //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    0,                                      //Speed
    0,                                      //Lifespan
    8,                                      //Size
    true,                                   //Targets Player?
    0                                       //Damage
    },                                      //---------------------
    25,                                     //Touch Damage
    25,                                     //XP Value
    3                                       //Min Spawn Level
    } },

    { EnemyType::Shooter,    {
    300,                                    //Spawned
    "SmartRanged",                          //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    1,                                      //Speed
    0,                                      //Move Angle
    30,                                     //Movement Length
    0,                                      //Movement Time
    0,                                      //Pause Length
    0,                                      //Pause Time
    0.005f,                                 //Aim Speed
    0,                                      //Aim Angle
    48,                                     //Size
    16,                                     //Size (Dead)
    {115,20,30,255},                        //Dark Red
    GRAY,
    false,                                  //Dead
    100,                                    //Health    
    100,                                    //Max Health
    120,                                    //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    8,                                      //Speed
    180,                                    //Lifespan
    8,                                      //Size
    true,                                   //Targets Player?
    15                                      //Damage
    },                                      //---------------------
    5,                                      //Touch Damage
    40,                                     //XP Value
    5                                       //Min Spawn Level
    } },

    { EnemyType::BOSS_Giant,    {
    300,                                    //Spawned
    "BOSS_Giant",                           //AI
    true,                                   //Is Boss
    0,                                      //X
    0,                                      //Y
    3,                                      //Speed
    0,                                      //Move Angle
    120,                                    //Movement Length
    0,                                      //Movement Time
    15,                                     //Pause Length
    0,                                      //Pause Time
    0.01f,                                  //Aim Speed
    0,                                      //Aim Angle
    240,                                    //Size
    96,                                     //Size (Dead)
    DARKGREEN,
    GRAY,
    false,                                  //Dead
    10000,                                  //Health    
    10000,                                  //Max Health
    300,                                    //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    8,                                      //Speed
    300,                                    //Lifespan
    60,                                     //Size
    true,                                   //Targets Player?
    40                                      //Damage
    },                                      //---------------------
    50,                                     //Touch Damage
    99999999,                               //XP Value
    10                                      //Min Spawn Level (boss)
    } },

    { EnemyType::Leaper,    {
    300,                                    //Spawned
    "Charger",                              //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    20,                                     //Speed
    0,                                      //Move Angle
    15,                                     //Movement Length
    0,                                      //Movement Time
    120,                                    //Pause Length
    0,                                      //Pause Time
    0.02f,                                  //Aim Speed
    0,                                      //Aim Angle
    60,                                     //Size
    20,                                     //Size (Dead)
    DARKBLUE,
    GRAY,
    false,                                  //Dead
    500,                                    //Health    
    500,                                    //Max Health
    99999999,                               //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    0,                                      //Speed
    0,                                      //Lifespan
    8,                                      //Size
    true,                                   //Targets Player?
    0                                       //Damage
    },                                      //---------------------
    50,                                     //Touch Damage
    120,                                    //XP Value
    11                                      //Min Spawn Level
    } },

    { EnemyType::Sniper,    {
    300,                                    //Spawned
    "SmartRanged",                          //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    8,                                      //Speed
    0,                                      //Move Angle
    30,                                     //Movement Length
    0,                                      //Movement Time
    60,                                     //Pause Length
    0,                                      //Pause Time
    0.005f,                                 //Aim Speed
    0,                                      //Aim Angle
    40,                                     //Size
    12,                                     //Size (Dead)
    BLACK,
    GRAY,
    false,                                  //Dead
    3,                                      //Health    
    3,                                      //Max Health
    300,                                    //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    12,                                     //Speed
    180,                                    //Lifespan
    10,                                     //Size
    true,                                   //Targets Player?
    90                                      //Damage
    },                                      //---------------------
    10,                                     //Touch Damage
    250,                                    //XP Value
    12                                      //Min Spawn Level
    } },

    { EnemyType::BOSS_Drifter,    {
    300,                                    //Spawned
    "BOSS_Drifter",                         //AI
    true,                                   //Is Boss
    0,                                      //X
    0,                                      //Y
    25,                                     //Speed
    0,                                      //Move Angle
    20,                                     //Movement Length
    0,                                      //Movement Time
    40,                                     //Pause Length
    0,                                      //Pause Time
    1.0f,                                   //Aim Speed
    0,                                      //Aim Angle
    72,                                     //Size
    24,                                     //Size (Dead)
    SKYBLUE,
    GRAY,
    false,                                  //Dead
    80000,                                  //Health    
    80000,                                  //Max Health
    10,                                     //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    30,                                     //Speed
    180,                                    //Lifespan
    10,                                     //Size
    true,                                   //Targets Player?
    10                                      //Damage
    },                                      //---------------------
    100,                                    //Touch Damage
    9999999,                                //XP Value
    20                                      //Min Spawn Level (boss)
    } },

    { EnemyType::Gunner,    {
    300,                                    //Spawned
    "SmartRanged",                          //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    4,                                      //Speed
    0,                                      //Move Angle
    60,                                     //Movement Length
    0,                                      //Movement Time
    60,                                     //Pause Length
    0,                                      //Pause Time
    0.01f,                                  //Aim Speed
    0,                                      //Aim Angle
    48,                                     //Size
    16,                                     //Size (Dead)
    DARKBLUE,
    GRAY,
    false,                                  //Dead
    2000,                                   //Health    
    2000,                                   //Max Health
    15,                                     //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    3,                                      //Speed
    180,                                    //Lifespan
    8,                                      //Size
    true,                                   //Targets Player?
    50                                      //Damage
    },                                      //---------------------
    30,                                     //Touch Damage
    1000,                                   //XP Value
    21                                      //Min Spawn Level
    } },

    { EnemyType::Swarmer,    {
    300,                                    //Spawned
    "Charger",                              //AI
    false,                                  //Is Boss
    0,                                      //X
    0,                                      //Y
    3,                                      //Speed
    0,                                      //Move Angle
    30,                                     //Movement Length
    0,                                      //Movement Time
    0,                                      //Pause Length
    0,                                      //Pause Time
    1.0f,                                   //Aim Speed
    0,                                      //Aim Angle
    30,                                     //Size
    10,                                     //Size (Dead)
    DARKGREEN,
    GRAY,
    false,                                  //Dead
    200,                                    //Health    
    200,                                    //Max Health
    99999999,                               //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    0,                                      //Speed
    0,                                      //Lifespan
    0,                                      //Size
    true,                                   //Targets Player?
    0                                       //Damage
    },                                      //---------------------
    20,                                     //Touch Damage
    100,                                    //XP Value
    22                                      //Min Spawn Level
    } },

    { EnemyType::BOSS_Dragon,    {
    300,                                    //Spawned
    "BOSS_Dragon",                          //AI
    true,                                   //Is Boss
    0,                                      //X
    0,                                      //Y
    2,                                      //Speed
    0,                                      //Move Angle
    180,                                    //Movement Length
    0,                                      //Movement Time
    30,                                     //Pause Length
    0,                                      //Pause Time
    0.005f,                                 //Aim Speed
    0,                                      //Aim Angle
    360,                                    //Size
    120,                                    //Size (Dead)
    ORANGE,
    GRAY,
    false,                                  //Dead
    1000000,                                //Health    
    1000000,                                //Max Health
    1,                                      //Shot Rate
    0,                                      //Shot Cooldown
    Projectile{                             //-----Shot Values-----
    0,                                      //Angle
    0,                                      //X
    0,                                      //Y
    5,                                      //Speed
    60,                                     //Lifespan
    12,                                     //Size
    true,                                   //Targets Player?
    60                                      //Damage
    },                                      //---------------------
    500,                                    //Touch Damage
    9999999,                                //XP Value
    30                                      //Min Spawn Level (boss)
    } },
};

// Sets up the textures for enemies that have assets.
namespace {
    struct EnemyTextureInitializer {
        EnemyTextureInitializer() {
            // only set for types whose assets exist in assets/
            if (Enemy::enemyStatsTable.count(EnemyType::Roamer))
                Enemy::enemyStatsTable[EnemyType::Roamer].textureAssetPath = "ships/enemies/enemy_drone_red.png";
            if (Enemy::enemyStatsTable.count(EnemyType::Charger))
                Enemy::enemyStatsTable[EnemyType::Charger].textureAssetPath = "ships/enemies/enemy_charger.png";
            if (Enemy::enemyStatsTable.count(EnemyType::Shooter))
                Enemy::enemyStatsTable[EnemyType::Shooter].textureAssetPath = "ships/enemies/enemy_big_red.png";
            if (Enemy::enemyStatsTable.count(EnemyType::Leaper))
                Enemy::enemyStatsTable[EnemyType::Leaper].textureAssetPath = "ships/enemies/enemy_big_green.png";
            if (Enemy::enemyStatsTable.count(EnemyType::Sniper))
                Enemy::enemyStatsTable[EnemyType::Sniper].textureAssetPath = "ships/enemies/enemy_big_blue.png";
            if (Enemy::enemyStatsTable.count(EnemyType::Gunner))
                Enemy::enemyStatsTable[EnemyType::Gunner].textureAssetPath = "ships/enemies/enemy_drone_blue.png";
            if (Enemy::enemyStatsTable.count(EnemyType::Swarmer))
                Enemy::enemyStatsTable[EnemyType::Swarmer].textureAssetPath = "ships/enemies/enemy_drone_green.png";
            // Bosses
            if (Enemy::enemyStatsTable.count(EnemyType::BOSS_Giant))
                Enemy::enemyStatsTable[EnemyType::BOSS_Giant].textureAssetPath = "ships/enemies/boss_big.png";
            if (Enemy::enemyStatsTable.count(EnemyType::BOSS_Drifter))
                Enemy::enemyStatsTable[EnemyType::BOSS_Drifter].textureAssetPath = "ships/enemies/boss_fast.png";
            if (Enemy::enemyStatsTable.count(EnemyType::BOSS_Dragon))
                Enemy::enemyStatsTable[EnemyType::BOSS_Dragon].textureAssetPath = "ships/enemies/boss_final.png";
                
        }
    } _enemyTextureInitializer;
}