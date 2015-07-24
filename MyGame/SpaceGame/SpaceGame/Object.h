#ifndef OBJECT_H
#define OBJECT_H

#include "CINDielib.h"
#include "IND_Entity2d.h"

class Object {
private:
	CIndieLib* mI;
	IND_Entity2d* entity2d;
	float posX, posY;
	float scaleX, scaleY;
	float angleZ;
public:
	Object();

	CIndieLib* getMI() const;
	void setMI(CIndieLib*);
	IND_Entity2d* getEntity2d() const;
	void setEntity2d(IND_Entity2d*);

	void saveProperties();
	void loadProperties();

	virtual ~Object();
};
#endif
