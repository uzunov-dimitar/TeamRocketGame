#ifndef MENU_H
#define MENU_H

#include <string>
#include "CIndieLib.h"
#include "IND_Font.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"

class Menu {
private:
	// to hold the id of the selected option
	short int selectedID;

	CIndieLib* mI;
	IND_Font* mFont;
	IND_Surface* mousePointerSurface;
	IND_Entity2d* mousePointer;
	vector<IND_Entity2d*> menuOptions;
public:
	Menu();

	short getSelectedID() const;
	void setSelectedID(short);

	CIndieLib* getMI() const;
	void setMI(CIndieLib*const);

	IND_Font* getFont() const;
	void setFont(IND_Font*);
	IND_Surface* getMousePointerSurface() const;
	void setMousePointerSurface(IND_Surface*);

	IND_Entity2d* getMousePointer() const;
	void  setMousePointer(IND_Entity2d*);
	vector<IND_Entity2d*>& getMenuOptions();
	void setMenuOptions(vector<IND_Entity2d*>);

	void createMenu(CIndieLib* const);

	void updateMenu();

	~Menu();
};
#endif