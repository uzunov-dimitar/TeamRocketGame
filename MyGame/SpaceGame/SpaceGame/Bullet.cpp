#include "Bullet.h"

void Bullet::createBullet(CIndieLib*const mI, const char* path, float posX, float posY, float angleZ)
{
	setMI(mI);

	setPathSurface(path);

	getMI()->_entity2dManager->add(getEntity2d());

	getEntity2d()->setHotSpot(0.5f, 0.5f);
	setPosition(posX, posY);
	setAngleZ(angleZ);

	float scale = (0.02*(getMI()->_window->getHeight() / getSurface()->getHeight()));
	setScale(scale, scale);
}

bool Bullet::updateBullet(float mDelta)
{
	setPosX(getPosX() + getMI()->_window->getWidth() * cos(getAngleZRadian()) * mDelta / 2.0f);
	setPosY(getPosY() - getMI()->_window->getWidth() * sin(getAngleZRadian()) * mDelta / 2.0f);

	return this->checkCoords();
}
