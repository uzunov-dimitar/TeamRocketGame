#ifndef PLANET_H
#define PLANET_H

#include "Object.h"

class Planet : public Object {
private:
public:
	void createPlanet(CIndieLib* const, const char*, const float, const float, const float, const float);

	~Planet();
};
#endif