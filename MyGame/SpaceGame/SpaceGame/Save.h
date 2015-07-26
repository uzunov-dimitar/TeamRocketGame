#ifndef SAVE_H
#define SAVE_H

#include <fstream>
#include <string>
#include "ErrorHandler.h"
#include "Planet.h"
#include "Ship.h"

class Save : public ErrorHandler {
private:
	ofstream save;
public:
	void makeSave(CIndieLib*, Ship*, vector<Planet*>&);
	void loadSettings();

	void writeLine(string, string);
	void writeObject(string, Object*);
	void writePlanet(Planet*, int);
	void writeShip(Ship*);
};

#endif
