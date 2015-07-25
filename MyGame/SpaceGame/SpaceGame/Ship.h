#ifndef SHIP_H
#define SHIP_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Object.h"
#include "Controls.h"
#include "IND_Animation.h"

class Ship : public Object {
private:
	int health;
	float speed;
	const float acceleration;
	const float maxSpeed;
	IND_Animation* mAnimationShip;
	IND_Animation* mAnimationStill;
	IND_Animation* mAnimationLeft;
	IND_Animation* mAnimationRight;
	IND_Entity2d* mAnim2dShip;
public:
	Ship(float acceleration = 100.0f, float maxSpeed = 400.0f);

	int getHealth() const;
	void setHealth(int);
	float getSpeed() const;
	void setSpeed(float);
	float getAcceleration() const;
	float getMaxSpeed() const;

	IND_Animation* getAnimationShip() const;
	void setAnimationShip(IND_Animation*);
	IND_Animation* getAnimationStill() const;
	void setAnimationStill(IND_Animation*);
	IND_Animation* getAnimationLeft() const;
	void setAnimationLeft(IND_Animation*);
	IND_Animation* getAnimationRight() const;
	void setAnimationRight(IND_Animation*);
	IND_Entity2d* getAnim2dShip() const;
	void setAnim2dShip(IND_Entity2d*);

	void createShip(CIndieLib* const, const char*, const float, const float);
	void moveShip(Controls*);

	void loadProperties();
	void saveProperties();

	~Ship();
};
#endif
