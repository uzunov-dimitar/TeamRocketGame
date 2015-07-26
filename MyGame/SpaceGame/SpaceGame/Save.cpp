#include "Save.h"

void Save::makeSave(CIndieLib* mI, Ship* mShip, vector<Planet*>& mPlanets)
{
	initialize(mI);
	save.open("Saves/quickSave.txt", ios::trunc);
	if (!save.is_open())
	{
		writeError(1000, 100, "Save", "Can't open file!");
		return;
	}
	writeShip(mShip);
	short int i = 0;
	for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
	{
		writePlanet(*it, i);
		i++;
	}
	save.close();
}

void Save::loadSettings()
{
}

void Save::writeLine(string key, string value)
{
	save << "[" + key + "]=[" + value + "]\n";
}

void Save::writeObject(string name, Object* mObject)
{
	writeLine(name + "-posX", to_string(mObject->getPosX()));
	writeLine(name + "-posY", to_string(mObject->getPosY()));
	writeLine(name + "-scaleX", to_string(mObject->getScaleX()));
	writeLine(name + "-scaleY", to_string(mObject->getScaleY()));
	writeLine(name + "-angleZ", to_string(mObject->getAngleZ()));
	writeLine(name + "-pathSurface", mObject->getPathSurface());
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
	writeLine("ship-mAnimationLeft", mShip->getAnimationLeft()->getName());
}
