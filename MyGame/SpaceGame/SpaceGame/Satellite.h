#ifndef SATELLITE_H
#define SATELLITE_H

#include "Object.h"
#include "IND_Animation.h"

class Satellite : public Object{
private:
	float orbitRadius;

	IND_Animation* mAnimationDust;
public:
	Satellite();

	float getOrbitRadius() const;
	void setOrbitRadius(float);

	IND_Animation* getAnimationDust() const;
	void setAniamtionDust(IND_Animation*);

	void createSatellite(CIndieLib* const, const char*, const float, const float, const float, const float);

	void updateSatellite(float, float, float);

	void moveInCircle(float, float);

	void destroy();
	bool destroyed();

	~Satellite();
};
#endif