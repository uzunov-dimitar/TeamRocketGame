#ifndef PLANET_H
#define PLANET_H

#include "Object.h"

class Planet : public Object {
private:
public:
	Planet();

	float getRadius()const;

	void createPlanet(CIndieLib* const, const char*, const float, const float, const float);
	void movePlanet(float);

	~Planet();
};
#endif