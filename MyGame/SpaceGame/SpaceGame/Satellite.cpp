#include "Satellite.h"

Satellite::Satellite() : orbitRadius(0)
{
}

float Satellite::getOrbitRadius() const
{
	return orbitRadius;
}

void Satellite::setOrbitRadius(float orbitRadius)
{
	this->orbitRadius = orbitRadius;
}

void Satellite::createSatellite(CIndieLib * const mI, const char * path, const float posX, const float posY, const float angleZ, const float orbitRadius)
{
	// Initialize the master instance + error handler
	setMI(mI); //create

	// Manage the surface
	setPathSurface(path);

	// Manage the 2d entity
	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setHotSpot(0.5f, 0.5f); // <-O

	setAngleZ(angleZ);
	setOrbitRadius(orbitRadius);

	setPosX(posX + cos(getAngleZRadian())*getOrbitRadius());
	setPosY(posY + cos(getAngleZRadian())*getOrbitRadius());


	// Set Scale
	int winWidth = getMI()->_window->getWidth();
	int winHeight = getMI()->_window->getHeight();
	float entWidth = getSurface()->getWidth();
	float entHeight = getSurface()->getHeight();
	float scale = ((0.02*(winWidth / entWidth)) < (0.02*(winHeight / entHeight)) ?
																			(0.02*(winWidth / entWidth)) :
																			(0.02*(winHeight / entHeight)));
	setScale(scale, scale);
}

void Satellite::updateSatellite(float posX, float posY, float mDelta)
{
	setAngleZ(getAngleZ() - (270 * mDelta));
	moveInCircle(posX, posY);
}

void Satellite::moveInCircle(float posX, float posY)
{
	setPosX(posX + getOrbitRadius() * cos(-getAngleZRadian()));
	setPosY(posY + getOrbitRadius() * sin(-getAngleZRadian()));
}
