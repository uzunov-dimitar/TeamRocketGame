#ifndef MENU_H
#define MENU_H

#include <string>
#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "ErrorHandler.h"
#include "Planet.h"
#include "Ship.h"
#include "Hud.h"

// utility functions
void startNewGame(CIndieLib* const, Hud*, vector<Planet*>&, Ship*&);
void deleteObjects(Hud*, Ship*&, vector<Planet*>&);

class Menu : public ErrorHandler {
private:
	bool hidden;
	bool exit;
	// scale for the buttons (buttons with the same size are assumed)
	float scale;
	// to hold the id of the selected option
	short int selectedID;

	CIndieLib* mI;
	IND_Surface* mousePointerSurface;
	IND_Entity2d* mousePointer;
	vector<IND_Surface*> idleBtnSurfaces;
	vector<IND_Surface*> activeBtnSurfaces;
	vector<IND_Entity2d*> menuOptions;
public:
	Menu();

	bool isHidden() const;
	void setHidden(float);
	bool isExit() const;
	void setExit(float);

	float getScale() const;
	void setScale(float);
	short getSelectedID() const;
	void setSelectedID(short);

	CIndieLib* getMI() const;
	void setMI(CIndieLib*const);

	// Mouse Pointer
	IND_Surface* getMousePointerSurface() const;
	void setMousePointerSurface(IND_Surface*);
	IND_Entity2d* getMousePointer() const;
	void  setMousePointer(IND_Entity2d*);

	// Options
	// Button Surfaces
	vector<IND_Surface*>& getIdleBtnSurfaces();
	void setIdleBtnSurfaces(vector<IND_Surface*>);
	vector<IND_Surface*>& getActiveBtnSurfaces();
	void setActiveBtnSurfaces(vector<IND_Surface*>);

	// 2d Entities
	vector<IND_Entity2d*>& getMenuOptions();
	void setMenuOptions(vector<IND_Entity2d*>);

	// main functions
	void createMenu(CIndieLib* const);

	void updateMenu(Hud*, vector<Planet*>&, Ship*&);

	// hide / show the menu
	void hide();
	void show();

	// used to restore the properties of a menu option after its surface was changed
	void restoreProperties(IND_Entity2d*, short int);

	~Menu();
};
#endif