#ifndef CONTROLS_H
#define CONTROLS_H

#include <fstream>
#include <map>
#include <string>
#include "CIndieLib.h"
#include "ErrorHandler.h"

class Controls : public ErrorHandler {
private:
	map<string, IND_Key> keyMap;
public:
	IND_Key getAdvance();
	IND_Key getRotateLeft();
	IND_Key getRotateRight();
	IND_Key getQuickSave();
	IND_Key getQuickLoad();

	void loadSettings();

	~Controls();
};
#endif

