#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Projectile.h"
#include <string>

class Player {
public:
    int x;
    int y;
    int size;

    int hit;
    float health;
    float maxHealth;

    int moveSpeed;

    int shotCooldown;
    int shotRate;

    float aimAngle;
    int aimSpeed;

    int aimDist;
    float aimAngleRads;
    int aimSize;

    Projectile shotValues{ 0,0,0,0,0,0,0,0 };

    int touchDamage{ 0 };
    int xp;

    // Sprite support
    Texture2D texture{};
    bool hasTexture{ false };

    // Adjustable rotation offset (degrees) to align sprite orientation with aiming.
    // Tune this if the sprite's "forward" in the image differs from in-game forward.
    float textureRotationOffsetDeg{ 0.0f };

    Player(int newX, int newY, int newSize, int newHit, int newHealth, int newMaxHealth, int newMoveSpeed, int newShotCooldown, int newShotRate,
        int newAimSpeed, int newAimDist, float newAimAngleRads, int newAimSize, Projectile newShotValues, int newTouchDamage, int newXP);

    Projectile shoot();

    void reset(int screenWidth, int screenHeight);

    // Load a specific file from assets (e.g. "player.png"), or auto-load first supported image in "assets/" if filename empty.
    bool loadTextureFromAssets(const std::string& filename = "");

    // Unload texture if loaded.
    void unloadTexture();

    // Destructor ensures texture is freed.
    ~Player();
};

#endif