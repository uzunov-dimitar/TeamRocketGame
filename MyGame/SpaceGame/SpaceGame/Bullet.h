#ifndef BULLET_H
#define BULLET_H

#include "Object.h"

class Bullet : public Object {
private:
	float speedXY;
public:
	Bullet(float speedXY = 0);
	// @override
	virtual float getAngleZRadian() const;

	float getSpeedXY() const;
	void setSpeedXY(float);

	void setTint(short, short, short);
	void createBullet(CIndieLib* const, const char*, float, float, float);
	bool updateBullet(float);
};
#endif