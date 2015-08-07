#include "Menu.h"

Menu::Menu() : selectedID(0), mI(NULL), mFont(IND_Font::newFont()), mousePointerSurface(IND_Surface::newSurface()), mousePointer(IND_Entity2d::newEntity2d())
{
}

short Menu::getSelectedID() const
{
	return selectedID;
}

void Menu::setSelectedID(short selectedID)
{
	if (selectedID < 0)
	{
		setSelectedID(selectedID + getMenuOptions().size());
	}
	else
	{
		if (selectedID >= getMenuOptions().size())
		{
			setSelectedID(selectedID - getMenuOptions().size());
		}
		else
		{
			this->selectedID = selectedID;
		}
	}
}

CIndieLib * Menu::getMI() const
{
	return mI;
}

void Menu::setMI(CIndieLib* const mI)
{
	this->mI = mI;
}

IND_Font * Menu::getFont() const
{
	return mFont;
}

void Menu::setFont(IND_Font* mFont)
{
	this->mFont = mFont;
}

IND_Surface * Menu::getMousePointerSurface() const
{
	return mousePointerSurface;
}

void Menu::setMousePointerSurface(IND_Surface* mousePointerSurface)
{
	this->mousePointerSurface = mousePointerSurface;
}

IND_Entity2d * Menu::getMousePointer() const
{
	return mousePointer;
}

void Menu::setMousePointer(IND_Entity2d* mousePointer)
{
	this->mousePointer = mousePointer;
}

vector<IND_Entity2d*>& Menu::getMenuOptions()
{
	return menuOptions;
}

void Menu::setMenuOptions(vector<IND_Entity2d*> menuOptions)
{
	this->menuOptions = menuOptions;
}

void Menu::createMenu(CIndieLib* const mI)
{
	setMI(mI);

	getMI()->_fontManager->add(getFont(), "resources/font_big.png", "resources/font_big.xml", IND_ALPHA, IND_32);
	getMI()->_surfaceManager->add(getMousePointerSurface(), "resources/cursor.png", IND_OPAQUE, IND_32);

	// Manage the mouse pointer
	getMI()->_entity2dManager->add(getMousePointer());
	getMousePointer()->setSurface(getMousePointerSurface());
	getMousePointer()->setBoundingAreas("resources/cursor_collisions.xml");

	int winWidth = getMI()->_window->getWidth();
	int winHeight = getMI()->_window->getHeight();

	for (int i = 0; i < 3; i++)
	{
		getMenuOptions().push_back(IND_Entity2d::newEntity2d());
		getMI()->_entity2dManager->add(getMenuOptions().back());
		getMenuOptions().back()->setFont(getFont());
		getMenuOptions().back()->setAlign(IND_CENTER);
		float optionHeight = (0.4f + i * 0.1f) * winHeight;
		getMenuOptions().back()->setPosition(winWidth / 2.0f, optionHeight, 0);
		getMenuOptions().back()->setTint(0, 255, 0);
		getMenuOptions().back()->setBoundingRectangle(("option" + to_string(i)).c_str(), 0, 0, 30, 30);
		getMenuOptions().back()->setShow(false);
	}

	getMenuOptions().at(0)->setText("New Game");
	getMenuOptions().at(1)->setText("Lload Ggame");
	getMenuOptions().at(2)->setText("Exit");
}

void Menu::updateMenu()
{
	getMousePointer()->setPosition(getMI()->_input->getMouseX(), getMI()->_input->getMouseY(), 1);
}

Menu::~Menu()
{
	getMI()->_fontManager->remove(getFont());
	getMI()->_surfaceManager->remove(getMousePointerSurface());

	getMI()->_entity2dManager->remove(getMousePointer());
	for (vector<IND_Entity2d*>::iterator it = getMenuOptions().begin(); it != getMenuOptions().end(); ++it)
	{
		getMI()->_entity2dManager->remove(*it);
	}
	getMenuOptions().clear();
}
