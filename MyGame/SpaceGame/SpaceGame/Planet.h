#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <string>
#include "Object.h"
#include "Bullet.h"
#include "Satellite.h"
#include "SoundEngine.h"

class Planet : public Object {
private:
	// if false, moveInEllipse
	bool circleTrajectory;
	float orbitRadius;

	float timer;
	float shootingFrequency;

	IND_Entity2d* lineX;
	IND_Entity2d* lineY;

	vector<Bullet*> mBullets;

	vector<Satellite*> mSatellites;
public:
	Planet();
	
	bool isCircletrajectory() const;
	void setCircleTrajectory(bool);
	float getOrbitRadius() const;
	void setOrbitRadius(float);

	float getTimer() const;
	void setTimer(float);
	float getShootingFrequency() const;
	void setShootingFrequency(float);

	IND_Entity2d* getLineX() const;
	void setLineX(IND_Entity2d*);
	IND_Entity2d* getLineY() const;
	void setLineY(IND_Entity2d*);

	vector<Bullet*>& getBullets();
	void setBullets(vector<Bullet*>);

	vector<Satellite*>& getSatellites();
	void setSatellites(vector<Satellite*>);

	void createPlanet(CIndieLib* const, const char*, const float, const float, const float, const float, const bool, const float);

	bool addSatellite();

	void updatePlanet(float, double, double);
	void moveInCircle();
	void moveInEllipse();

	virtual ~Planet();
};
#endif