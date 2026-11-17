#include "Projectile.h"
#include <cmath>                    //CMath
#include <math.h>
#include "raymath.h"
#include <string>


Projectile::Projectile(float newAngle, float newX, float newY, float newSpeed, int newLifespan, int newSize, bool newTarget, int newDamage)
{
	angle = newAngle;
	x = newX;
	y = newY;
	speed = newSpeed;
	lifespan = newLifespan;
	size = newSize;
	isPlayerTarget = newTarget;
	damage = newDamage;
}

Projectile Projectile::move(Projectile projectile)
{
	//Check if the bullet's lifespan is greater than 0.
	projectile.x += projectile.speed * sin(projectile.angle);
	projectile.y += projectile.speed * cos(projectile.angle);
	//Reduce the bullet's lifespan by 1.
	projectile.lifespan += -1;
	return projectile;
}