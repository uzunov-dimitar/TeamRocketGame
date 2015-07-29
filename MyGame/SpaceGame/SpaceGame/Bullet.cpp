#include "Bullet.h"

void Bullet::createBullet(CIndieLib*const mI, const char* path, float posX, float posY, float angleZ)
{
	setMI(mI);

	
	setPathSurface(path);
	mI->_surfaceManager->add(getSurface(), path, IND_ALPHA, IND_32);

	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setSurface(getSurface());

	getEntity2d()->setHotSpot(0.5f, 0.5f); // comment out to see both entity2d and mAnim2dShip
	setPosition(posX, posY);
	setAngleZ(angleZ);

	float scale = (0.02*(getMI()->_window->getHeight() / getSurface()->getHeight()));
	setScale(scale, scale);
}

bool Bullet::updateBullet(float mDelta)
{
	setPosX(getPosX() + getMI()->_window->getWidth() * cos(getAngleZRadian()) * mDelta / 10.0f);
	setPosY(getPosY() - getMI()->_window->getWidth() * sin(getAngleZRadian()) * mDelta / 10.0f);

	return this->checkCoords();
}
