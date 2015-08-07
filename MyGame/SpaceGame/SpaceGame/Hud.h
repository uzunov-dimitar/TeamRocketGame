#ifndef HUD_H
#define HUD_H

#include "CIndieLib.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "Ship.h"

class Hud {
private:
	CIndieLib* mI;
	IND_Font* mFont;
	IND_Entity2d* bulletsText;
	IND_Entity2d* healthText;
	IND_Entity2d* scoreText;
	IND_Entity2d* loadingText;
public:
	Hud();

	CIndieLib* getMI() const;
	void setMI(CIndieLib*const);

	IND_Font* getFont() const;
	void setFont(IND_Font*);
	IND_Entity2d* getBulletsText() const;
	void setBulletsText(IND_Entity2d*);
	IND_Entity2d* getHealthText() const;
	void setHealthText(IND_Entity2d*);
	IND_Entity2d* getScoreText() const;
	void setScoreText(IND_Entity2d*);
	IND_Entity2d* getLoadingText() const;
	void setLoadingText(IND_Entity2d*);

	void createHud(CIndieLib*const);

	void updateHud(Ship*);

	void hideHud();
	void showHud();

	~Hud();
};
#endif