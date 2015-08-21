#include "Bullet.h"

Bullet::Bullet(float speedXY): speedXY(speedXY)
{
}

// @override
float Bullet::getAngleZRadian() const
{
	return (-getAngleZ() + 90.0f) / 180.0f * M_PI;
}

float Bullet::getSpeedXY() const
{
	return speedXY;
}

void Bullet::setSpeedXY(float speedXY)
{
	this->speedXY = speedXY;
}

void Bullet::setTint(short pR, short pG, short pB)
{
	getEntity2d()->setTint(pR, pG, pB);
}

void Bullet::createBullet(CIndieLib*const mI, const char* path, float posX, float posY, float angleZ)
{
	setMI(mI);

	setPathSurface(path);

	getMI()->_entity2dManager->add(getEntity2d());

	getEntity2d()->setHotSpot(0.5f, 0.5f);
	setPosition(posX, posY);
	setAngleZ(angleZ);
	getEntity2d()->setBoundingAreas("../SpaceGame/resources/green_beam_collisions.xml");

	float scale = (0.02*(getMI()->_window->getHeight() / getSurface()->getHeight()));
	setScale(scale, scale);

	setSpeedX(getSpeedXY() * cos(getAngleZRadian()));
	setSpeedY(getSpeedXY() * sin(-getAngleZRadian()));
}

bool Bullet::updateBullet(float mDelta)
{
	setPosX(getPosX() + getSpeedX() * mDelta);
	setPosY(getPosY() + getSpeedY() * mDelta);

	return this->checkCoords();
}