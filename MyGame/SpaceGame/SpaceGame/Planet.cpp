#include "Planet.h"

Planet::Planet()
{
}

float Planet::getRadius() const
{
	return (getMI()->_window->getHeight() - getHeight())/2.0f;
}

void Planet::createPlanet(CIndieLib * const mI, const char * path, const float posX, const float posY, const float percent)
{
	// Initialize the master instance + error handler
	setMI(mI);
	initialize(mI);

	// Manage the surface
	setPathSurface(path);
	getMI()->_surfaceManager->add(getSurface(), path, IND_ALPHA, IND_32);

	// Manage the 2d entity
	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setSurface(getSurface());

	getEntity2d()->setHotSpot(0.5f, 0.5f);
	setPosition(posX, posY);

	// Set Scale
	int winWidth = getMI()->_window->getWidth();
	int winHeight = getMI()->_window->getHeight();
	float entWidth = getSurface()->getWidth();
	float entHeight = getSurface()->getHeight();
	float scale = ((percent*(winWidth / entWidth)) < (percent*(winHeight / entHeight)) ?
																					(percent*(winWidth / entWidth)) :
																					(percent*(winHeight / entHeight)));
	setScale(scale, scale);
}

void Planet::movePlanet()
{
	// get delta time
	float mDelta = getMI()->_render->getFrameTime() / 1000.0f;

	// move at 45 degrees per second
	setAngleZ(getAngleZ() + (45 * mDelta));

	setPosX(getMI()->_window->getWidth()/2.0f + getRadius() * cos(getAngleZRadian()));
	setPosY(getMI()->_window->getHeight()/2.0f + getRadius() * sin(getAngleZRadian()));
}

Planet::~Planet()
{
}
