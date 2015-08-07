#ifndef SATELLITE_H
#define SATELLITE_H

#include "Object.h"

class Satellite : public Object{
private:
	float orbitRadius;
public:
	Satellite();

	float getOrbitRadius() const;
	void setOrbitRadius(float);

	void createSatellite(CIndieLib* const, const char*, const float, const float, const float, const float);

	void updateSatellite(float, float, float);
	void moveInCircle(float, float);
};
#endif