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
	setMI(mI);
	getSaveFile()->open("Saves/quickSave.txt", ios::trunc);
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
	writeLine(name + "-angleZ", to_string(mObject->getAngleZ()));
}

void Save::writePlanet(Planet* mPlanet, int i)
{
	writeObject("planet" + to_string(i), mPlanet);
	writeLine("planet" + to_string(i) + "-circleTrajectory", to_string(mPlanet->isCircletrajectory()));
	writeLine("planet" + to_string(i) + "-orbitRadius", to_string(mPlanet->getOrbitRadius()));
	short int id = 0;
	for (vector<Satellite*>::iterator it = mPlanet->getSatellites().begin(); it != mPlanet->getSatellites().end(); ++it)
	{
		writeSatellite("planet" + to_string(i) + "-satellite" + to_string(id), *it);
		id++;
	}
}

void Save::writeSatellite(string name, Satellite* mSatellite)
{
	writeObject(name, mSatellite);
	writeLine(name + "-orbitRadius", to_string(mSatellite->getOrbitRadius()));
}

void Save::writeShip(Ship* mShip)
{
	writeObject("ship", mShip);
	writeLine("ship-health", to_string(mShip->getHealth()));
	writeLine("ship-numFiredBullets", to_string(mShip->getNumFiredBullets()));
	writeLine("ship-score", to_string(mShip->getScore()));
	writeLine("ship-speed", to_string(mShip->getSpeed()));
	writeLine("ship-acceleration", to_string(mShip->getAcceleration()));
	writeLine("ship-jolt", to_string(mShip->getJolt()));
	writeLine("ship-maxSpeed", to_string(mShip->getMaxSpeed()));
	writeLine("ship-mAnimationStill", mShip->getAnimationStill()->getName());
	writeLine("ship-mAnimationShip", mShip->getAnimationShip()->getName());
	writeLine("ship-mAnimationLeft", mShip->getAnimationLeft()->getName());
	writeLine("ship-mAnimationRight", mShip->getAnimationRight()->getName());
	int i = 0;
	for (vector<Bullet*>::iterator it = mShip->getBullets().begin(); it != mShip->getBullets().end(); ++it)
	{
		// So far a bullet does not have any additional properties compared to an Object
		writeObject("bullet" + to_string(i), *it);
		i++;
	}
}

void Save::loadSave(CIndieLib* mI, Ship*& mShip, vector<Planet*>& mPlanets)
{
	setMI(mI);
	getLoadFile()->open("Saves/quickSave.txt", ios::in);
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
		if (!objectName.compare(0, 6, "ship") )
		{
			readShip(mShip, property, value);
		}
		else if (!objectName.compare(0, 6, "bullet"))
		{
			int id = std::stoi(objectName.substr(objectName.find_first_of("t")+1));
			if (mShip->getBullets().size() <= id) 
			{
				mShip->getBullets().push_back(new Bullet());
				mShip->getBullets().back()->setMI(getMI());
			}
			readBullet(mShip->getBullets().back(), property, value);
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
		if (!property.compare("mAnimationStill"))
		{
			//Manage the animation
			getMI()->_animationManager->addToSurface(mShip->getAnimationStill(), value.c_str(), IND_ALPHA, IND_32);
			getMI()->_entity2dManager->add(mShip->getAnim2dShip());
			mShip->getAnim2dShip()->setAnimation(mShip->getAnimationStill());
			mShip->loadPropsAnim2d();

			mShip->getEntity2d()->setPosition(mShip->getPosX(), mShip->getPosY(), 1);

			// Manage Sound
			mShip->setSoundEngine(irrklang::createIrrKlangDevice());

			if (!mShip->getSoundEngine())
			{
				writeError(1000, 100, "SoundEngine", "can't create device.");
			}
			mShip->setRocketSound(mShip->getSoundEngine()->play2D("irrKlang/media/v-start.wav", true, true, true));
			mShip->setBlasterSoundSource(mShip->getSoundEngine()->addSoundSourceFromFile("irrKlang/media/blaster.wav"));
			mShip->setBlasterSound(mShip->getSoundEngine()->play2D("irrKlang/media/blaster.wav", false, true, true));
			mShip->getSoundEngine()->setSoundVolume(0.1f);
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
	}
}

void Save::readBullet(Bullet* mBullet, string& property, string& value)
{
	if (!readObject(mBullet, property, value))
	{
		// No additional properties to read
	}
}

Save::~Save()
{
	delete loadFile;
	delete saveFile;
}