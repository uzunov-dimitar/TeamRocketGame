#include "Save.h"

Save::Save() : loadFile(new ifstream()), saveFile(new ofstream())
{
}

CIndieLib * Save::getMI() const
{
	return mI;
}

void Save::setMI(CIndieLib* mI)
{
	this->mI = mI;
	initialize(mI);
}

ifstream * Save::getLoadFile() const
{
	return loadFile;
}

void Save::getLoadFile(ifstream * loadFile)
{
	this->loadFile = loadFile;
}

ofstream* Save::getSaveFile() const
{
	return saveFile;
}

void Save::setSaveFile(ofstream* saveFile)
{
	this->saveFile = saveFile;
}

void Save::makeSave(CIndieLib* mI, Ship* mShip, vector<Planet*>& mPlanets)
{
	if (mShip != NULL && mShip->getHealth() > 0)
	{
		setMI(mI);
		getSaveFile()->open("../SpaceGame/Saves/quickSave.txt", ios::trunc);
		if (!getSaveFile()->is_open())
		{
			writeError(1000, 100, "Save", "Can't open file for writing!");
			return;
		}

		writeShip(mShip);
		short int i = 0;
		for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
		{
			writePlanet(*it, i);
			i++;
		}
		getSaveFile()->close();
	}
}

void Save::writeLine(string key, string value)
{
	*getSaveFile() << "[" + key + "]=[" + value + "]\n";
}

void Save::writeObject(string name, Object* mObject)
{
	writeLine(name + "-pathSurface", mObject->getPathSurface());
	writeLine(name + "-posX", to_string(mObject->getPosX()));
	writeLine(name + "-posY", to_string(mObject->getPosY()));
	writeLine(name + "-scaleX", to_string(mObject->getScaleX()));
	writeLine(name + "-scaleY", to_string(mObject->getScaleY()));
	writeLine(name + "-speedX", to_string(mObject->getSpeedX()));
	writeLine(name + "-speedY", to_string(mObject->getSpeedY()));
	writeLine(name + "-angleZ", to_string(mObject->getAngleZ()));
	writeLine(name + "-angularSpeed", to_string(mObject->getAngularSpeed()));
	writeLine(name + "-angularAcceleration", to_string(mObject->getAngularAcceleration()));
	writeLine(name + "-maxAngularSpeed", to_string(mObject->getMaxAngularSpeed()));
}

void Save::writePlanet(Planet* mPlanet, int i)
{
	writeObject("planet" + to_string(i), mPlanet);
	writeLine("planet" + to_string(i) + "-circleTrajectory", to_string(mPlanet->isCircletrajectory()));
	writeLine("planet" + to_string(i) + "-orbitRadius", to_string(mPlanet->getOrbitRadius()));
	writeLine("planet" + to_string(i) + "-timer", to_string(mPlanet->getTimer()));
	writeLine("planet" + to_string(i) + "-shootingFrequency", to_string(mPlanet->getShootingFrequency()));
	for (vector<Bullet*>::iterator it = mPlanet->getBullets().begin(); it != mPlanet->getBullets().end(); ++it)
	{
		// So far a bullet does not have any additional properties compared to an Object
		writeBullet("planet" + to_string(i) + "-bullet" + to_string(it - mPlanet->getBullets().begin()), *it);
	}
	for (vector<Satellite*>::iterator it = mPlanet->getSatellites().begin(); it != mPlanet->getSatellites().end(); ++it)
	{
		// it - mPlanet->getSatellites().begin() 
		// equals the serial id of the satellite
		writeSatellite("planet" + to_string(i) + "-satellite" + to_string(it - mPlanet->getSatellites().begin()), *it);
	}
}

void Save::writeSatellite(string name, Satellite* mSatellite)
{
	writeObject(name, mSatellite);
	writeLine(name + "-orbitRadius", to_string(mSatellite->getOrbitRadius()));
}

void Save::writeShip(Ship* mShip)
{
	// write general object properties
	writeObject("ship", mShip);

	// specific properties
	writeLine("ship-health", to_string(mShip->getHealth()));
	writeLine("ship-numFiredBullets", to_string(mShip->getNumFiredBullets()));
	writeLine("ship-score", to_string(mShip->getScore()));
	writeLine("ship-lastHitPlanet", to_string(mShip->getLastHitPlanet()));

	writeLine("ship-speed", to_string(mShip->getSpeed()));
	writeLine("ship-acceleration", to_string(mShip->getAcceleration()));
	writeLine("ship-jolt", to_string(mShip->getJolt()));
	writeLine("ship-maxSpeed", to_string(mShip->getMaxSpeed()));

	writeLine("ship-timer", to_string(mShip->getTimer()));

	writeLine("ship-mAnimationStill", mShip->getAnimationStill()->getName());
	writeLine("ship-mAnimationShip", mShip->getAnimationShip()->getName());
	writeLine("ship-mAnimationLeft", mShip->getAnimationLeft()->getName());
	writeLine("ship-mAnimationRight", mShip->getAnimationRight()->getName());
	writeLine("ship-mAnimationExplode", mShip->getAnimationExplode()->getName());

	for (vector<Bullet*>::iterator it = mShip->getBullets().begin(); it != mShip->getBullets().end(); ++it)
	{
		// So far a bullet does not have any additional properties compared to an Object
		writeBullet("ship-bullet" + to_string(it - mShip->getBullets().begin()), *it);
	}
}

void Save::writeBullet(string name, Bullet* mBullet)
{
	writeObject(name, mBullet);
	writeLine(name + "-speedXY", to_string(mBullet->getSpeedXY()));
}

void Save::loadSave(CIndieLib* mI, Ship*& mShip, vector<Planet*>& mPlanets)
{
	setMI(mI);
	getLoadFile()->open("../SpaceGame/Saves/quickSave.txt", ios::in);
	if (!getLoadFile()->is_open())
	{
		writeError(1000, 200, "Save", "Can't open file for reading!");
		return;
	}
	mShip = new Ship();
	mShip->setMI(getMI());
	while (readLine(mShip, mPlanets));
	getLoadFile()->close();

	getMI()->_input->update();
	getMI()->_render->beginScene();
	getMI()->_entity2dManager->renderEntities2d();
	getMI()->_render->endScene();
}

bool Save::readLine(Ship* mShip, vector<Planet*>& mPlanets)
{
	string line = "";
	if (std::getline(*getLoadFile(), line))
	{
		string objectName = line.substr(line.find_first_of("[")+1, line.find_first_of("-") - line.find_first_of("[") - 1);
		string property = line.substr(line.find_first_of("-") + 1, line.find_first_of("]") - line.find_first_of("-") - 1);
		line = line.substr(line.find_first_of("=")+1);
		string value = line.substr(line.find_first_of("=") + 2, line.find_first_of("]") - line.find_first_of("[") - 1);
		if (!objectName.compare(0, 4, "ship") )
		{
			readShip(mShip, property, value);
		}
		else
		{
			int id = std::stoi(objectName.substr(objectName.find_first_of("t") + 1));
			if (mPlanets.size() <= id)
			{
				mPlanets.push_back(new Planet());
				mPlanets.back()->setMI(getMI());
			}
			readPlanet(mPlanets.back(), property, value);
		}
		return true;
	}
	else 
	{
		return false;
	}
}

bool Save::readObject(Object* mObject, string& property, string& value)
{
	if (!property.compare("posX"))
	{
		mObject->setPosX(stof(value));
		return true;
	}
	if (!property.compare("posY"))
	{
		mObject->setPosY(stof(value));
		return true;
	}
	if (!property.compare("scaleX"))
	{
		mObject->setScaleX(stof(value));
		return true;
	}
	if (!property.compare("scaleY"))
	{
		mObject->setScaleY(stof(value));
		return true;
	}
	if (!property.compare("speedX"))
	{
		mObject->setSpeedX(stof(value));
		return true;
	}
	if (!property.compare("speedY"))
	{
		mObject->setSpeedY(stof(value));
		return true;
	}
	if (!property.compare("angularSpeed"))
	{
		mObject->setAngularSpeed(stof(value));
		return true;
	}
	if (!property.compare("angularAcceleration"))
	{
		mObject->setAngularAcceleration(stof(value));
		return true;
	}
	if (!property.compare("maxAngularSpeed"))
	{
		mObject->setMaxAngularSpeed(stof(value));
		return true;
	}
	if (!property.compare("angleZ"))
	{
		mObject->setAngleZ(stof(value));
		return true;
	}
	if (!property.compare("pathSurface"))
	{
		mObject->setPathSurface(value.c_str());
		getMI()->_entity2dManager->add(mObject->getEntity2d());
		mObject->getEntity2d()->setHotSpot(0.5f, 0.5f);
		return true;
	}
	// if none of the IFs executes, the property is not one of an object therefore return false
	return false;
}

void Save::readPlanet(Planet* mPlanet, string& property, string& value)
{
	if (!readObject(mPlanet, property, value))
	{
		
		if (!property.compare("circleTrajectory"))
		{
			mPlanet->setCircleTrajectory(stoi(value));
		}
		if (!property.compare("orbitRadius"))
		{
			mPlanet->setOrbitRadius(stof(value));
			mPlanet->getEntity2d()->setBoundingCircle("planet", mPlanet->getSurface()->getWidth() / 2.0f, mPlanet->getSurface()->getWidth() / 2.0f, mPlanet->getSurface()->getWidth() / 2.0f);
		}
		if (!property.compare("timer"))
		{
			mPlanet->setTimer(stof(value));
		}
		if (!property.compare("shootingFrequency"))
		{
			mPlanet->setShootingFrequency(stof(value));
		}
		if (!property.compare(0, 6, "bullet"))
		{
			// extract the serial number of the bullet
			int id = std::stoi(property.substr(6, property.find_first_of("-") - 6));
			if (mPlanet->getBullets().size() <= id)
			{
				mPlanet->getBullets().push_back(new Bullet());
				mPlanet->getBullets().back()->setMI(getMI());
			}

			// change property so that it contains only the actual property of the bullet
			property = property.substr(property.find_first_of("-") + 1, property.find_first_of("]") - property.find_first_of("-") - 1);
			readBullet(mPlanet->getBullets().back(), property, value);
		}
		if (!property.compare(0, 9, "satellite"))
		{
			// extract the serial number of the satellite
			int id = std::stoi(property.substr(9, 1));
			if (mPlanet->getSatellites().size() <= id)
			{
				mPlanet->getSatellites().push_back(new Satellite());
				mPlanet->getSatellites().back()->setMI(getMI());
			}

			// change property so that it contains only the actual property of the satellite
			property = property.substr(property.find_first_of("-") + 1, property.find_first_of("]") - property.find_first_of("-") - 1);
			readSatellite(mPlanet->getSatellites().back(), property, value);
		}
	}
}

void Save::readSatellite(Satellite* mSatellite, string& property, string& value)
{
	if (!readObject(mSatellite, property, value))
	{
		if (!property.compare("orbitRadius"))
		{
			mSatellite->setOrbitRadius(stof(value));
			mSatellite->getEntity2d()->setBoundingCircle("satellite", mSatellite->getSurface()->getWidth() / 2.0f, mSatellite->getSurface()->getWidth() / 2.0f, mSatellite->getSurface()->getWidth() / 2.0f);
			getMI()->_animationManager->addToSurface(mSatellite->getAnimationDust(), "../SpaceGame/resources/animations/dust.xml", IND_ALPHA, IND_32, 255, 0, 255);
		}
	}
}

void Save::readShip(Ship* mShip, string& property, string& value)
{
	if (!readObject(mShip, property, value))
	{
		if (!property.compare("health"))
		{
			mShip->setHealth(stoi(value));
		}
		if (!property.compare("numFiredBullets"))
		{
			mShip->setNumFiredBullets(stoi(value));
		}
		if (!property.compare("score"))
		{
			mShip->setScore(stoi(value));
		}
		if (!property.compare("lastHitPlanet"))
		{
			mShip->setLastHitPlanet(stoi(value));
		}
		if (!property.compare("speed"))
		{
			mShip->setSpeed(stof(value));
		}
		if (!property.compare("acceleration"))
		{
			mShip->setAcceleration(stof(value));
		}
		if (!property.compare("jolt"))
		{
			mShip->setJolt(stof(value));
		}
		if (!property.compare("maxSpeed"))
		{
			mShip->setMaxSpeed(stof(value));
		}
		if (!property.compare("timer"))
		{
			mShip->setTimer(stof(value));
		}
		if (!property.compare("mAnimationStill"))
		{
			//Manage the animation
			getMI()->_animationManager->addToSurface(mShip->getAnimationStill(), value.c_str(), IND_ALPHA, IND_32);
			getMI()->_entity2dManager->add(mShip->getAnim2dShip());
			mShip->getAnim2dShip()->setAnimation(mShip->getAnimationStill());
			mShip->loadPropsAnim2d();

			// manage the 2d entity
			mShip->getEntity2d()->setPosition(mShip->getPosX(), mShip->getPosY(), 1);
			// set bounding areas
			mShip->getEntity2d()->setBoundingAreas("../SpaceGame/resources/Spaceship with motor new/spaceship_collisions.xml");
		}
		if (!property.compare("mAnimationShip"))
		{
			getMI()->_animationManager->addToSurface(mShip->getAnimationShip(), value.c_str(), IND_ALPHA, IND_32);
		}
		if (!property.compare("mAnimationLeft"))
		{
			getMI()->_animationManager->addToSurface(mShip->getAnimationLeft(), value.c_str(), IND_ALPHA, IND_32);
		}
		if (!property.compare("mAnimationRight"))
		{
			getMI()->_animationManager->addToSurface(mShip->getAnimationRight(), value.c_str(), IND_ALPHA, IND_32);
		}
		if (!property.compare("mAnimationExplode"))
		{
			getMI()->_animationManager->addToSurface(mShip->getAnimationExplode(), value.c_str(), IND_ALPHA, IND_32);
		}
		if (!property.compare(0, 6, "bullet"))
		{
			// extract the serial number of the bullet
			int id = std::stoi(property.substr(6, property.find_first_of("-") - 6));
			if (mShip->getBullets().size() <= id)
			{
				mShip->getBullets().push_back(new Bullet());
				mShip->getBullets().back()->setMI(getMI());
			}

			// change property so that it contains only the actual property of the bullet
			property = property.substr(property.find_first_of("-") + 1, property.find_first_of("]") - property.find_first_of("-") - 1);
			readBullet(mShip->getBullets().back(), property, value);
		}
	}
}

void Save::readBullet(Bullet* mBullet, string& property, string& value)
{
	if (!readObject(mBullet, property, value))
	{
		if (!property.compare("speedXY"))
		{
			mBullet->setSpeedXY(stof(value));
			mBullet->getEntity2d()->setBoundingAreas("../SpaceGame/resources/green_beam_collisions.xml");
		}
	}
}

Save::~Save()
{
	delete loadFile;
	delete saveFile;
}