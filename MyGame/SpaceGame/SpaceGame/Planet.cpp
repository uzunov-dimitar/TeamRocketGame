#include "Planet.h"

void Planet::createPlanet(CIndieLib * const mI, const char * path, const float posX, const float posY, const float scaleX, const float scaleY)
{
	setMI(mI);
	initialize(mI);
	getMI()->_surfaceManager->add(getSurface(), path, IND_ALPHA, IND_32);

	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setHotSpot(0.5f, 0.5f);
	getEntity2d()->setSurface(getSurface());
	getEntity2d()->setPosition(posX, posY, 0);
	getEntity2d()->setScale(scaleX, scaleY);
}

Planet::~Planet()
{
}
