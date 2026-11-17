#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <map>
#include <unordered_map>
#include "raylib.h"
#include "Projectile.h"
#include <memory>

enum class EnemyType {
    Roamer,
    Charger,
    Shooter,
    BOSS_Giant,

    Leaper,
    Sniper,
    BOSS_Drifter,

    Gunner,
    Swarmer,
    BOSS_Dragon
    // Add more as needed
};

class Enemy {
public:
    static std::map<EnemyType, Enemy> enemyStatsTable;

    int spawned;
    std::string AI;
    bool isBoss;

    float x;
    float y;

    float speed;
    float moveAngle;

    int movementLength;
    int movementTime;
    int pauseLength;
    int pauseTime;

    float aimSpeed;
    float aimAngle;

    float size;
    float sizeDead;
    Color colorAlive;
    Color colorDead;

    bool dead;
    int health;
    int maxHealth;

    int shootRate;
    int cooldown;
    Projectile shotValues{ 0,0,0,0,0,0,0,0 };

    int touchDamage;
    int xpValue;              // also used as spawn cost

    // Minimum level this enemy may appear at (default 1)
    int minSpawnLevel{ 1 };

    // Sprite support (shared ownership so Enemy copies are safe)
    std::shared_ptr<Texture2D> texturePtr{};
    float textureRotationOffsetDeg{ 0.0f };

    std::string textureAssetPath{};

    // Global cache to avoid multiple loads / accidental unload races
    static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_textureCache;

    // Declare ensure loader (implemented in Enemy.cpp)
    void ensureTextureLoaded();

    //Functions
    Enemy();

    Enemy(int newSpawned, std::string newAI, bool isBoss, int newX, int newY, float newSpeed, float newMoveAngle, int newMovementLength, int newMovementTime,
        int newPauseLength, int newPauseTime, float newAimSpeed, float newAimAngle, float newSize, float newSizeDead, Color colorAlive, Color colorDead, 
        bool newDead, int newHealth, int newMaxHealth, int newShootRate, int newShotCooldown, Projectile newShotValues, int newTouchDamage, int newXPValue, int newMinSpawnLevel);

    Enemy coreAI(Enemy enemy, int playerX, int playerY, int minX, int maxX, int minY, int maxY);

    Enemy moveAI(Enemy enemy, int playerX, int playerY, int minX, int maxX, int minY, int maxY);

    float aimAI(Enemy enemy, int targetX, int targetY);

    bool shootAICheck(Enemy enemy, float targetX, float targetY);

    Enemy randomSpawnLoc(Enemy enemy, int minX, int maxX, int minY, int maxY, int playerX, int playerY);

    // Use instance method (no pass-by-value copies)
    Projectile shoot();

    // Load texture for this enemy instance from assets/ (path relative to assets/)
    bool loadTextureFromAssets(const std::string& filename);

    // Does this enemy have a valid texture?
    bool hasTexture() const { return texturePtr && texturePtr->id != 0; }
};
#endif