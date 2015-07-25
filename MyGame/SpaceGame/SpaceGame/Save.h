#ifndef SAVE_H
#define SAVE_H

#include <fstream>
#include <string.h>
#include <map>
#include <iostream>
#include "ErrorHandler.h"

class Save : public ErrorHandler {
private:
public:
	void loadSettings();
};

#endif
