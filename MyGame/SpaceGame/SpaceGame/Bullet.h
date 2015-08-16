#ifndef BULLET_H
#define BULLET_H

#include "Object.h"

class Bullet : public Object {
private:
public:
	// @overwrite
	virtual float getAngleZRadian() const;

	void createBullet(CIndieLib* const, const char*, float, float, float);
	bool updateBullet(float);
};
#endif