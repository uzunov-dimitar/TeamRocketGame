#ifndef SHIP_H
#define SHIP_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Object.h"
#include "Controls.h"
#include "IND_Animation.h"

class Ship : public Object {
private:
	double speed;
	const int acceleration;
	const int maxSpeed;
	IND_Animation* mAnimationShip;
	IND_Animation* mAnimationStill;
	IND_Animation* mAnimationLeft;
	IND_Animation* mAnimationRight;
public:
	Ship();

	void createShip(CIndieLib* const, const char*, const float, const float, const float, const float);
	void checkCoords();
	void moveShip(Controls*);

	~Ship();
};
#endif
