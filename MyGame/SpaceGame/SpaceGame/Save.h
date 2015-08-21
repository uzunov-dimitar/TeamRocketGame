#ifndef SAVE_H
#define SAVE_H

#include <fstream>
#include <string>
#include "ErrorHandler.h"
#include "Bullet.h"
#include "Planet.h"
#include "Ship.h"

class Save : public ErrorHandler {
private:
	ifstream* loadFile;
	ofstream* saveFile;
	CIndieLib* mI;
public:
	Save();

	CIndieLib* getMI() const;
	void setMI(CIndieLib*);

	ifstream* getLoadFile() const;
	void getLoadFile(ifstream*);
	ofstream* getSaveFile() const;
	void setSaveFile(ofstream*);
	
	// functions related to saving
	void makeSave(CIndieLib*, Ship*, vector<Planet*>&);

	void writeLine(string, string);
	void writeObject(string, Object*);
	void writePlanet(Planet*, int);
	void writeSatellite(string, Satellite*);
	void writeShip(Ship*);
	void writeBullet(string, Bullet*);

	// functions related to loading
	void loadSave(CIndieLib*, Ship*&, vector<Planet*>&);

	bool readLine(Ship*, vector<Planet*>&);
	bool readObject(Object*, string&, string&);
	void readPlanet(Planet*, string&, string&);
	void readSatellite(Satellite*, string&, string&);
	void readShip(Ship*, string&, string&);
	void readBullet(Bullet*, string&, string&);

	~Save();
};

#endif
