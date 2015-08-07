#include "Planet.h"

Planet::Planet() : circleTrajectory(false), orbitRadius(0)
{
}

bool Planet::isCircletrajectory() const
{
	return circleTrajectory;
}

void Planet::setCircleTrajectory(bool circleTrajectory)
{
	this->circleTrajectory = circleTrajectory;
}

float Planet::getOrbitRadius() const
{
	return orbitRadius;
}

void Planet::setOrbitRadius(float orbitRadius)
{
	this->orbitRadius = orbitRadius;
}

vector<Satellite*>& Planet::getSatellites()
{
	return mSatellites;
}

void Planet::setSatellites(vector<Satellite*> mSatellites)
{
	this->mSatellites = mSatellites;
}

void Planet::createPlanet(CIndieLib * const mI, const char * path, const float posX, const float posY, const float angleZRadian, const float scalePercent, const bool circleTrajectory, const float orbitRadius)
{
	// Initialize the master instance + error handler
	setMI(mI);

	// Manage the surface
	setPathSurface(path);

	// Manage the 2d entity
	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setHotSpot(0.5f, 0.5f); // <-O

	setPosition(posX, posY);
	setAngleZ(angleZRadian / M_PI * 180);

	// Set Scale
	int winWidth = getMI()->_window->getWidth();
	int winHeight = getMI()->_window->getHeight();
	float entWidth = getSurface()->getWidth();
	float entHeight = getSurface()->getHeight();
	float scale = ((scalePercent*(winWidth / entWidth)) < (scalePercent*(winHeight / entHeight)) ?
																					(scalePercent*(winWidth / entWidth)) :
																					(scalePercent*(winHeight / entHeight)));
	setScale(scale, scale);

	setCircleTrajectory(circleTrajectory);
	setOrbitRadius(orbitRadius);
}

bool Planet::addSatellite()
{
	if (getSatellites().size() < 2)
	{
		getSatellites().push_back(new Satellite());
		float orbitSatellite = (getWidth() + 0.065f * getMI()->_window->getHeight()) / 2.0f;
		if (getSatellites().size() == 1)
		{
			getSatellites().back()->createSatellite(getMI(), ("resources/Planets/" + to_string(rand()%8+1) + ".png").c_str(), getPosX(), getPosY(), getAngleZ(), orbitSatellite);
		}
		else
		{
			getSatellites().back()->createSatellite(getMI(), ("resources/Planets/" + to_string(rand() % 8 + 1) + ".png").c_str(), getPosX(), getPosY(), getSatellites().at(0)->getAngleZ()+31 + rand()%300, orbitSatellite);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Planet::updatePlanet(float mDelta)
{
	// move at 45 degrees per second
	setAngleZ(getAngleZ() - (45 * mDelta));

	if (isCircletrajectory())
	{
		moveInCircle();
	}
	else
	{
		moveInEllipse();
	}
	for (vector<Satellite*>::iterator it = getSatellites().begin(); it != getSatellites().end(); ++it)
	{
		(*it)->updateSatellite(getPosX(), getPosY(), mDelta);
	}
}

void Planet::moveInCircle()
{
	setPosX(getMI()->_window->getWidth() / 2.0f + getOrbitRadius() * cos(-getAngleZRadian()));
	setPosY(getMI()->_window->getHeight() / 2.0f + getOrbitRadius() * sin(-getAngleZRadian()));
}

void Planet::moveInEllipse()
{
	setPosX(getMI()->_window->getWidth() / 2.0f + (getOrbitRadius() / getMI()->_window->getHeight()) * getMI()->_window->getWidth() * cos(-getAngleZRadian()));
	setPosY(getMI()->_window->getHeight() / 2.0f + getOrbitRadius() * sin(-getAngleZRadian()));
}

Planet::~Planet()
{
	for (vector<Satellite*>::iterator it = getSatellites().begin(); it != getSatellites().end(); ++it)
	{
		delete (*it);
	}
	getSatellites().clear();
}