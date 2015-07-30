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
	setMI(mI); //create

	// Manage the surface
	setPathSurface(path); //

	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setHotSpot(0.5f, 0.5f); // <-O
	
	// Manage the 2d entity
	
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

void Planet::movePlanet(float mDelta)
{
	// move at 45 degrees per second
	setAngleZ(getAngleZ() + (45 * mDelta));
	
	// Move in Circle
	/*setPosX(getMI()->_window->getWidth()/2.0f + getRadius() * cos(getAngleZRadian()));
	setPosY(getMI()->_window->getHeight()/2.0f + getRadius() * sin(getAngleZRadian()));*/

	// Move in Ellipse
	setPosX(getMI()->_window->getWidth() / 2.0f + (getMI()->_window->getWidth() - getWidth()) * cos(getAngleZRadian()) / 2.0f);
	setPosY(getMI()->_window->getHeight() / 2.0f + (getMI()->_window->getHeight() - getHeight()) * sin(getAngleZRadian()) / 2.0f);
}

Planet::~Planet()
{
}