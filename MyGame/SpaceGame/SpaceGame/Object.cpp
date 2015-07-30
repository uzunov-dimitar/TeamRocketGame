#include "Object.h"

Object::Object() : posX(0.0f), posY(0.0f), scaleX(0.0f), scaleY(0.0f), angleZ(0.0f), pathSurface(new char[100]), mSurface(IND_Surface::newSurface()), mEntity2d(IND_Entity2d::newEntity2d())
{
}

float Object::getPosX() const
{
	return posX;
}

void Object::setPosX(float posX)
{
	this->posX = posX;
	getEntity2d()->setPosition(posX, getEntity2d()->getPosY(), getEntity2d()->getPosZ());
}

float Object::getPosY() const
{
	return posY;
}

void Object::setPosY(float posY)
{
	this->posY = posY;
	getEntity2d()->setPosition(getEntity2d()->getPosX(), posY, getEntity2d()->getPosZ());
}

void Object::setPosition(float posX, float posY)
{
	setPosX(posX);
	setPosY(posY);
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

void Object::setScale(float scaleX, float scaleY)
{
	setScaleX(scaleX);
	setScaleY(scaleY);
}

float Object::getWidth() const
{
	return getSurface()->getWidth()*getScaleX();
}

float Object::getHeight() const
{
	return getSurface()->getHeight()*getScaleY();
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

float Object::getAngleZRadian() const
{
	// indielib interprets moving clockwise in a positive degree and anticlockwise as a negative degree 
	// in math (including the implementation of sin and cos in c++) it is the opposite
	// that's why take the opposite of the degree 
	// and then add 90 degrees to it since the rocket faces +90 degrees by default
	// then convert to radians
	return (-getAngleZ() + 90) / 180.0f * M_PI;
}

char * Object::getPathSurface() const
{
	return pathSurface;
}

void Object::setPathSurface(const char * pathSurface)
{
	strncpy(this->pathSurface, pathSurface, 99);
	getMI()->_surfaceManager->add(getSurface(), getPathSurface(), IND_ALPHA, IND_32);
	getEntity2d()->setSurface(getSurface());
}

CIndieLib * Object::getMI() const
{
	return mI;
}

void Object::setMI(CIndieLib * mI)
{
	this->mI = mI;
	// for the ErrorHandler;
	initialize(mI);
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

bool Object::checkCoords()
{
	// Check for negative coordinates
	bool outOfScreen = false;
	if (getPosX() < 0)
	{
		setPosition(0, getPosY());
		outOfScreen = true;
	}

	if (getPosY() < 0)
	{
		setPosition(getPosX(), 0);
		outOfScreen = true;
	}

	// Check for too big coordinates

	if (getPosX() > getMI()->_window->getWidth())
	{
		setPosition(getMI()->_window->getWidth(), getPosY());
		outOfScreen = true;
	}

	if (getPosY() > getMI()->_window->getHeight())
	{
		setPosition(getPosX(), getMI()->_window->getHeight());
		outOfScreen = true;
	}
	return outOfScreen;
}

Object::~Object()
{
	getMI()->_surfaceManager->remove(getSurface());
	getMI()->_entity2dManager->remove(getEntity2d());
	delete[] pathSurface;
}