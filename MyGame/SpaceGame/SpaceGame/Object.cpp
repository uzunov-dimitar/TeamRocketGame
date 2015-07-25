#include "Object.h"

Object::Object() : mSurface(IND_Surface::newSurface()), mEntity2d(IND_Entity2d::newEntity2d()), posX(0.0f), posY(0.0f), scaleX(0.0f), scaleY(0.0f), angleZ(0.0f)
{
}

float Object::getPosX() const
{
	return posX;
}

void Object::setPosX(float posX)
{
	this->posX = posX;
	getEntity2d()->setPosition(posX, getEntity2d()->getPosY(), 0);
}

float Object::getPosY() const
{
	return posY;
}

void Object::setPosY(float posY)
{
	this->posY = posY;
	getEntity2d()->setPosition(getEntity2d()->getPosX(), posY, 0);
}

float Object::getScaleX() const
{
	return scaleX;
}

void Object::setScaleX(float scaleX)
{
	this->scaleX = scaleX;
	getEntity2d()->setScale(scaleX, getEntity2d()->getScaleY());
}

float Object::getScaleY() const
{
	return scaleY;
}

void Object::setScaleY(float scaleY)
{
	this->scaleY = scaleY;
	getEntity2d()->setScale(getEntity2d()->getScaleX(), scaleY);
}

float Object::getAngleZ() const
{
	return angleZ;
}

void Object::setAngleZ(float angleZ)
{
	this->angleZ = angleZ;
	getEntity2d()->setAngleXYZ(0, 0, angleZ);
}

CIndieLib * Object::getMI() const
{
	return mI;
}

void Object::setMI(CIndieLib * mI)
{
	this->mI = mI;
}

IND_Surface * Object::getSurface() const
{
	return mSurface;
}

void Object::setSurface(IND_Surface * mSurface)
{
	this->mSurface = mSurface;
}

IND_Entity2d * Object::getEntity2d() const
{
	return mEntity2d;
}

void Object::setEntity2d(IND_Entity2d * entity2d)
{
	this->mEntity2d = entity2d;
}

void Object::checkCoords()
{
	// Check for negative coordinates

	if (getEntity2d()->getPosX() < 0)
	{
		getEntity2d()->setPosition(0, getEntity2d()->getPosY(), 0);
	}

	if (getEntity2d()->getPosY() < 0)
	{
		getEntity2d()->setPosition(getEntity2d()->getPosX(), 0, 0);
	}

	// Check for too big coordinates

	if (getEntity2d()->getPosX() > getMI()->_window->getWidth())
	{
		getEntity2d()->setPosition(getMI()->_window->getWidth(), getEntity2d()->getPosY(), 0);
	}

	if (getEntity2d()->getPosY() > getMI()->_window->getHeight())
	{
		getEntity2d()->setPosition(getEntity2d()->getPosX(), getMI()->_window->getHeight(), 0);
	}
}

Object::~Object()
{
	getSurface()->destroy();
	getEntity2d()->destroy();
}
