#include "Player.h"
#include "raylib.h"
#include <filesystem>
#include <algorithm>
#include <vector>

namespace fs = std::filesystem;

Player::~Player()
{
    unloadTexture();
}

void Player::unloadTexture()
{
    if (hasTexture) {
        UnloadTexture(texture);
        hasTexture = false;
        texture = Texture2D{};
    }
}

bool Player::loadTextureFromAssets(const std::string& filename)
{
    // Unload any previously loaded texture first for caution
    unloadTexture();

    std::string path;

    if (!filename.empty())
    {
        // Use forward slashes — raylib accepts both on Windows
        path = std::string("assets/") + filename;
        Texture2D t = LoadTexture(path.c_str());
        if (t.id != 0) {
            texture = t;
            hasTexture = true;
            return true;
        }
        return false;
    }

    // Auto-discover first supported image in assets/ (png, jpg, jpeg, bmp, tga, gif)
    if (!fs::exists("assets") || !fs::is_directory("assets")) return false;

    const std::vector<std::string> exts = { ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".gif" };

    for (const auto& entry : fs::directory_iterator("assets"))
    {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (std::find(exts.begin(), exts.end(), ext) != exts.end())
        {
            path = entry.path().string();
            Texture2D t = LoadTexture(path.c_str());
            if (t.id != 0) {
                texture = t;
                hasTexture = true;
                return true;
            }
        }
    }

    return false;
}

Player::Player(int newX, int newY, int newSize, int newHit, int newHealth, int newMaxHealth, int newMoveSpeed, int newShotCooldown, int newShotRate,
    int newAimSpeed, int newAimDist, float newAimAngleRads, int newAimSize, Projectile newShotValues, int newTouchDamage, int newXP)
{
    x = newX;
    y = newY;
    size = newSize;
    hit = newHit;
    health = newHealth;
    maxHealth = newMaxHealth;
    moveSpeed = newMoveSpeed;
    shotCooldown = newShotCooldown;
    shotRate = newShotRate;
    aimSpeed = newAimSpeed;
    aimDist = newAimDist;
    aimAngleRads = newAimAngleRads;
    aimSize = newAimSize;
    shotValues = newShotValues;
    touchDamage = newTouchDamage;
    xp = newXP;
}

Projectile Player::shoot()
{
    float spawnOffset = (float)size * 1.0f; // scale spawn distance with player size
    Projectile newProjectile = Projectile(
        aimAngleRads,
        x + (spawnOffset) * sin(aimAngleRads),
        y + (spawnOffset) * cos(aimAngleRads),
        shotValues.speed,
        shotValues.lifespan,
        shotValues.size,
        shotValues.isPlayerTarget,
        shotValues.damage);
    return newProjectile;
}

void Player::reset(int screenWidth, int screenHeight) {
    x = screenWidth / 2;
    y = screenHeight / 2;
    size = 48;
    hit = 0;
    health = 100;
    maxHealth = 100;
    moveSpeed = 6;
    shotCooldown = 0;
    shotRate = 30;
    aimSpeed = 3;
    aimDist = 18;
    aimAngleRads = 0;
    aimSize = 8;
    shotValues = { 0, 0, 0, 10, 180, 8, false, 20 };
    touchDamage = 10;
    xp = 0;
}