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
	initialize(mI);
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

void Save::loadSave(CIndieLib* mI, Ship*& mShip, vector<Planet*>& mPlanets)
{
	initialize(mI);
	setMI(mI);
	getLoadFile()->open("Saves/quickSave.txt", ios::in);
	if (!getLoadFile()->is_open())
	{
		writeError(1000, 200, "Save", "Can't open file for reading!");
		return;
	}
	delete mShip;
	mShip = new Ship();
	mShip->setMI(getMI());
	for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
	{
		delete (*it);
	}
	mPlanets.clear();
	writeError(1000, 100, "size", mPlanets.size());
	while (readLine(mShip, mPlanets));
	getLoadFile()->close();
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

void Save::writePlanet(Planet * mPlanet, int i)
{
	writeObject("planet" + to_string(i), mPlanet);
}

void Save::writeShip(Ship* mShip)
{
	writeObject("ship", mShip);
	writeLine("ship-health", to_string(mShip->getHealth()));
	writeLine("ship-speed", to_string(mShip->getSpeed()));
	writeLine("ship-acceleration", to_string(mShip->getAcceleration()));
	writeLine("ship-maxSpeed", to_string(mShip->getMaxSpeed()));
	writeLine("ship-mAnimationStill", mShip->getAnimationStill()->getName());
	writeLine("ship-mAnimationShip", mShip->getAnimationShip()->getName());
	writeLine("ship-mAnimationLeft", mShip->getAnimationLeft()->getName());
	writeLine("ship-mAnimationRight", mShip->getAnimationRight()->getName());
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
		if (!objectName.compare(0, 6, "ship"))
		{
			readShip(mShip, property, value);
		}
		else
		{
			int id = std::stoi(objectName.substr(objectName.find_first_of("t")+1));
			writeError(1000, 100, "size", mPlanets.size());
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
		getMI()->_surfaceManager->add(mObject->getSurface(), mObject->getPathSurface(), IND_ALPHA, IND_32);
		getMI()->_entity2dManager->add(mObject->getEntity2d());
		mObject->getEntity2d()->setSurface(mObject->getSurface());
		mObject->getEntity2d()->setHotSpot(0.5f, 0.5f);
		return true;
	}
	return false;
}

void Save::readShip(Ship * mShip, string& property, string& value)
{
	if (!readObject(mShip, property, value))
	{
		if (!property.compare("health"))
		{
			mShip->setHealth(stoi(value));
		}
		if (!property.compare("speed"))
		{
			mShip->setSpeed(stof(value));
		}
		if (!property.compare("acceleration"))
		{
			mShip->setAcceleration(stof(value));
		}
		if (!property.compare("maxSpeed"))
		{
			mShip->setMaxSpeed(stof(value));
		}
		if (!property.compare("mAnimationStill"))
		{
			getMI()->_animationManager->addToSurface(mShip->getAnimationStill(), value.c_str(), IND_ALPHA, IND_32);
			getMI()->_entity2dManager->add(mShip->getAnim2dShip());
			mShip->getAnim2dShip()->setAnimation(mShip->getAnimationStill());
			mShip->loadPropsAnim2d();
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

void Save::readPlanet(Planet* mPlanet, string& property, string& value)
{
	if (!readObject(mPlanet, property, value))
	{
	}
}

Save::~Save()
{
	delete loadFile;
	delete saveFile;
}