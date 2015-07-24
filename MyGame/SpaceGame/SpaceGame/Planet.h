#ifndef PLANET_H
#define PLANET_H

#include "Object.h"
#include "IND_Surface.h"

class Planet : public Object {
private:
	IND_Surface* mSurfacePlanet;
public:
	void createPlanet(CIndieLib* const, const char*, const float, const float, const float, const float);

	~Planet();
};
#endif