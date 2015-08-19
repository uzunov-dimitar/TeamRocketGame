#include "Bullet.h"

float Bullet::getAngleZRadian() const
{
	return (-getAngleZ() + 90.0f) / 180.0f * M_PI;
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

	setSpeedX(getMI()->_window->getWidth() * cos(getAngleZRadian()));
	setSpeedY(- getMI()->_window->getWidth() * sin(getAngleZRadian()));
}

bool Bullet::updateBullet(float mDelta)
{
	setPosX(getPosX() + getSpeedX() * mDelta);
	setPosY(getPosY() + getSpeedY() * mDelta);

	return this->checkCoords();
}