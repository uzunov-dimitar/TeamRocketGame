#include "Planet.h"

Planet::Planet() : circleTrajectory(false), orbitRadius(0), lineX(IND_Entity2d::newEntity2d()), lineY(IND_Entity2d::newEntity2d())
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

IND_Entity2d* Planet::getLineX() const
{
	return lineX;
}

void Planet::setLineX(IND_Entity2d* lineX)
{
	this->lineX = lineX;
}

IND_Entity2d* Planet::getLineY() const
{
	return lineY;
}

void Planet::setLineY(IND_Entity2d* lineY)
{
	this->lineY = lineY;
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
	getEntity2d()->setBoundingCircle("planet", getSurface()->getWidth()/2.0f, getSurface()->getWidth() / 2.0f, getSurface()->getWidth() / 2.0f);

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
	setAngularSpeed(45.0f);
	/*getLineX()->setPrimitive2d(IND_LINE);
	getLineY()->setPrimitive2d(IND_LINE);
	getMI()->_entity2dManager->add(getLineX());
	getMI()->_entity2dManager->add(getLineY());*/
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
	// rotate
	setAngleZ(getAngleZ() - (getAngularSpeed() * mDelta));

	if (isCircletrajectory())
	{
		moveInCircle();
		float speed = (getAngularSpeed() * M_PI / 180.0f) * getOrbitRadius();
		setSpeedX(-sin(getAngleZRadian()) * speed);
		setSpeedY(-cos(getAngleZRadian()) * speed);
	}
	else
	{
		moveInEllipse();
		float speedMajor = (getAngularSpeed() * M_PI / 180.0f) * (getOrbitRadius() / getMI()->_window->getHeight()) * getMI()->_window->getWidth();
		float speedMinor = (getAngularSpeed() * M_PI / 180.0f) * getOrbitRadius();
		setSpeedX(-sin(getAngleZRadian()) * speedMajor);
		setSpeedY(-cos(getAngleZRadian()) * speedMinor);
	}
	for (vector<Satellite*>::iterator it = getSatellites().begin(); it != getSatellites().end(); ++it)
	{
		(*it)->updateSatellite(getPosX(), getPosY(), mDelta);
	}
	//getLineX()->setLine(getPosX(), getPosY(), getPosX() + getSpeedX(), getPosY());
	//getLineY()->setLine(getPosX(), getPosY(), getPosX() , getPosY() + getSpeedY());
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