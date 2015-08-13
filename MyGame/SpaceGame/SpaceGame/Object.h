#ifndef OBJECT_H
#define OBJECT_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "CINDielib.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "ErrorHandler.h"

class Object : public ErrorHandler {
private:
	float posX, posY;
	float scaleX, scaleY;
	float speedX, speedY; // add to save!

	// In Degrees!
	// NOTE: in indielib clockwise movement is considered positive and not negative!
	float angleZ;
	float angularSpeed; // add to save
	float angularAcceleration; // add to save
	float maxAngularSpeed; //add to save

	char* pathSurface;
	CIndieLib* mI;
	IND_Surface* mSurface;
	IND_Entity2d* mEntity2d;
public:
	Object();

	float getPosX() const;
	void setPosX(float);
	float getPosY() const;
	void setPosY(float);
	void setPosition(float, float);

	float getScaleX() const;
	void setScaleX(float);
	float getScaleY() const;
	void setScaleY(float);
	void setScale(float, float);

	float getWidth() const;
	float getHeight() const;

	float getSpeedX() const;
	void setSpeedX(float);
	float getSpeedY() const;
	void setSpeedY(float);
	float getSpeedXY() const;

	float getAngleZ() const;
	void setAngleZ(float);
	virtual float getAngleZRadian() const;
	float getAngularSpeed() const;
	void setAngularSpeed(float);
	float getAngularAcceleration() const;
	void setAngularAcceleration(float);
	float getMaxAngularSpeed() const;
	void setMaxAngularSpeed(float);

	char* getPathSurface() const;
	void setPathSurface(const char*);

	CIndieLib* getMI() const;
	void setMI(CIndieLib*const);
	IND_Surface* getSurface() const;
	void setSurface(IND_Surface*);
	IND_Entity2d* getEntity2d() const;
	void setEntity2d(IND_Entity2d*);

	bool checkCoords();

	virtual ~Object();
};
#endif
