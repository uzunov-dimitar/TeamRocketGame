#include "Object.h"

Object::Object() : posX(0.0f), posY(0.0f), scaleX(0.0f), scaleY(0.0f), angleZ(0)
{
}

CIndieLib * Object::getMI() const
{
	return mI;
}

void Object::setMI(CIndieLib * mI)
{
	this->mI = mI;
}

IND_Entity2d * Object::getEntity2d() const
{
	return entity2d;
}

void Object::setEntity2d(IND_Entity2d * entity2d)
{
	this->entity2d = entity2d;
}

void Object::saveProperties()
{
	posX = entity2d->getPosX();
	posY = entity2d->getPosY();
	scaleX = entity2d->getScaleX();
	scaleY = entity2d->getScaleY();
	angleZ = entity2d->getAngleZ();
}

void Object::loadProperties()
{
	entity2d->setHotSpot(0.5f, 0.5f);
	entity2d->setPosition(posX, posY, 0);
	entity2d->setScale(scaleX, scaleY);
	entity2d->setAngleXYZ(0, 0, angleZ);
}

Object::~Object()
{
	entity2d->destroy();
}
