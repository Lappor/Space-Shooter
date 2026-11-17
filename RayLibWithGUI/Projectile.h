#ifndef PROJECTILE_H
#define PROJECTILE_H


class Projectile {
public:
    float angle{};
    float x{};
    float y{};
    float speed{};
    int lifespan{};
    int size{};
    bool isPlayerTarget{}; //Either 0 (enemy) or 1 (player)
    int damage{};

    Projectile(float newAngle, float newX, float newY, float newSpeed, int newLifespan, int newSize, bool newTarget, int newDamage);
    
    Projectile move(Projectile projectile);

};

#endif