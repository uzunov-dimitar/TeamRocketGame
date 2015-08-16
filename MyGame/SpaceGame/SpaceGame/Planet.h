#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <string>
#include "Object.h"
#include "Satellite.h"

class Planet : public Object {
private:
	// if false, moveInEllipse
	bool circleTrajectory;
	float orbitRadius;
	IND_Entity2d* lineX;
	IND_Entity2d* lineY;
	vector<Satellite*> mSatellites;
public:
	Planet();
	
	bool isCircletrajectory() const;
	void setCircleTrajectory(bool);
	float getOrbitRadius() const;
	void setOrbitRadius(float);

	IND_Entity2d* getLineX() const;
	void setLineX(IND_Entity2d*);
	IND_Entity2d* getLineY() const;
	void setLineY(IND_Entity2d*);

	vector<Satellite*>& getSatellites();
	void setSatellites(vector<Satellite*>);

	void createPlanet(CIndieLib* const, const char*, const float, const float, const float, const float, const bool, const float);

	bool addSatellite();

	void updatePlanet(float);
	void moveInCircle();
	void moveInEllipse();

	virtual ~Planet();
};
#endif