#ifndef OBJECT_H
#define OBJECT_H

#include "CINDielib.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "ErrorHandler.h"

class Object : public ErrorHandler {
private:
	float posX, posY;
	float scaleX, scaleY;
	float angleZ;
	CIndieLib* mI;
	IND_Surface* mSurface;
	IND_Entity2d* mEntity2d;
public:
	Object();

	float getPosX() const;
	void setPosX(float);
	float getPosY() const;
	void setPosY(float);
	float getScaleX() const;
	void setScaleX(float);
	float getScaleY() const;
	void setScaleY(float);
	float getAngleZ() const;
	void setAngleZ(float);

	CIndieLib* getMI() const;
	void setMI(CIndieLib*);
	IND_Surface* getSurface() const;
	void setSurface(IND_Surface*);
	IND_Entity2d* getEntity2d() const;
	void setEntity2d(IND_Entity2d*);

	void checkCoords();

	virtual ~Object();
};
#endif
