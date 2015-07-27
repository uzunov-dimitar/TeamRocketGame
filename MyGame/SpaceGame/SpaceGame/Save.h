#ifndef SAVE_H
#define SAVE_H

#include <fstream>
#include <string>
#include "ErrorHandler.h"
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

	void makeSave(CIndieLib*, Ship*, vector<Planet*>&);
	void loadSave(CIndieLib*, Ship*&, vector<Planet*>&);

	void writeLine(string, string);
	void writeObject(string, Object*);
	void writePlanet(Planet*, int);
	void writeShip(Ship*);

	bool readLine(Ship*, vector<Planet*>&);
	bool readObject(Object*, string&, string&);
	void readShip(Ship*, string&, string&);
	void readPlanet(Planet*, string&, string&);

	~Save();
};

#endif
