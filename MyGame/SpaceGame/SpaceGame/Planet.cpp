#include "Planet.h"

Planet::Planet() : circleTrajectory(false), orbitRadius(0.0f), timer(0.0f), shootingFrequency(0.0f), lineX(IND_Entity2d::newEntity2d()), lineY(IND_Entity2d::newEntity2d())
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

float Planet::getTimer() const
{
	return timer;
}

void Planet::setTimer(float timer)
{
	this->timer = timer;
}

float Planet::getShootingFrequency() const
{
	return shootingFrequency;
}

void Planet::setShootingFrequency(float shootingFrequency)
{
	this->shootingFrequency = shootingFrequency;
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

vector<Bullet*>& Planet::getBullets()
{
	return mBullets;
}

void Planet::setBullets(vector<Bullet*> mBullets)
{
	this->mBullets = mBullets;
}

vector<Satellite*>& Planet::getSatellites()
{
	return mSatellites;
}

void Planet::setSatellites(vector<Satellite*> mSatellites)
{
	this->mSatellites = mSatellites;
}

void Planet::createPlanet(CIndieLib* const mI, const char * path, const float posX, const float posY, const float angleZRadian, const float scalePercent, const bool circleTrajectory, const float orbitRadius)
{
	// Initialize the master instance + error handler
	setMI(mI);

	// Manage the surface
	setPathSurface(path);

	// Manage the 2d entity
	getMI()->_entity2dManager->add(getEntity2d());
	getEntity2d()->setHotSpot(0.5f, 0.5f); // <-O

	getEntity2d()->setPosition(posX, posY, 1);
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

	setShootingFrequency((50 + rand() % 50) / 10.0f);
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
			getSatellites().back()->createSatellite(getMI(), ("../SpaceGame/resources/Planets/" + to_string(rand()%8+1) + ".png").c_str(), getPosX(), getPosY(), getAngleZ(), orbitSatellite);
		}
		else
		{
			getSatellites().back()->createSatellite(getMI(), ("../SpaceGame/resources/Planets/" + to_string(rand() % 8 + 1) + ".png").c_str(), getPosX(), getPosY(), getSatellites().at(0)->getAngleZ()+31 + rand()%300, orbitSatellite);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Planet::updatePlanet(float mDelta, double shipPosX, double shipPosY)
{
	setTimer(getTimer() + mDelta);

	if (getSatellites().size() > 0  && getTimer() >= shootingFrequency)
	{
		setTimer(0.0f);
		//SoundEngine::getBlasterSound()->drop();
		//SoundEngine::setBlasterSound(SoundEngine::getSoundEngine()->play2D(SoundEngine::getBlasterSoundSource(), false, false, true));
		getBullets().push_back(new Bullet(getMI()->_window->getWidth() / 2.5f));
		double hypotenuse = sqrt((getPosX() - shipPosX)*(getPosX() - shipPosX) + (getPosY() - shipPosY)*(getPosY() - shipPosY));
		double sine = (shipPosY - getPosY()) / hypotenuse;
		double cosine = (shipPosX - getPosX()) / hypotenuse;
		double angle = 0.0f;
		if (asin(sine) >= 0.0f)
		{
			angle = acos(cosine);
		}
		else
		{
			angle = 2*M_PI - acos(cosine);
		}
		angle = (angle * 180.0f) / M_PI;
		getBullets().back()->createBullet(getMI(), "../SpaceGame/resources/green_beam.png", getPosX(), getPosY(), angle + 90.0f);//+ 45 + (rand()%90) );
		getBullets().back()->setTint(255, 128, 0);
	}
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
		if ((*it)->isDestroyed())
		{
			delete *it;
			getSatellites().erase(it);
			--it;
		}
		else
		{
			(*it)->updateSatellite(getPosX(), getPosY(), mDelta);
		}
	}

	// Manage Bullets:
	for (vector<Bullet*>::iterator it = getBullets().begin(); it < getBullets().end(); ++it)
	{
		if ((*it)->updateBullet(mDelta))
		{
			vector<Bullet*>::iterator it2 = it;
			delete (*it);
			getBullets().erase(it);
		}
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
	// bullets
	for (vector<Bullet*>::iterator it = getBullets().begin(); it != getBullets().end(); ++it)
	{
		delete (*it);
	}
	getBullets().clear();
	// satellites
	for (vector<Satellite*>::iterator it = getSatellites().begin(); it != getSatellites().end(); ++it)
	{
		delete (*it);
	}
	getSatellites().clear();
}