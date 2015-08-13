#include <ctime>
#include <vector>
#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "irrKlang.h"
#include "Controls.h"
#include "Hud.h"
#include "Planet.h"
#include "Save.h"
#include "Ship.h"
#include "Satellite.h"
#include "Menu.h"
/*
==================
Main
==================
*/

void checkCollisions(CIndieLib* const, vector<Planet*>, Ship*);
int IndieLib()
{
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;
	
	// ----- Get Window Dimensions

	int winWidth = mI->_window->getWidth();
	int winHeight = mI->_window->getHeight();

	srand(static_cast<unsigned int>(time(0)));

	// ----- Surface loading -----

	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "resources/Backgrounds/18.jpg", IND_OPAQUE, IND_32)) return 0;
	
	// Loading 2D Entities

	// Background
	IND_Entity2d* mBack = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBack);
	mBack->setSurface(mSurfaceBack);
	mBack->setScale((float)winWidth / mSurfaceBack->getWidth(), (float)winHeight / mSurfaceBack->getHeight());

	Controls* controls = new Controls();
	controls->loadSettings();

	ErrorHandler* error = new ErrorHandler();
	error->initialize(mI);

	Hud* mHud = new Hud();
	mHud->createHud(mI);

	Menu* mMenu = new Menu();
	mMenu->createMenu(mI);

	Save* quickSave = new Save();

	vector<Planet*> mPlanets;
	Ship* mShip = NULL;

	bool loadSave = false;
	float mDelta = 0.0f;
	//mI->_entity2dManager->
	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit() && !mMenu->isExit())
	{
		// get delta time
		mDelta = mI->_render->getFrameTime() / 1000.0f;
		
		if (mI->_input->isKeyPressed(controls->getMenu()))
		{
			mMenu->show();
		}
		if (!mMenu->isHidden())
		{
			mMenu->updateMenu(mHud, mPlanets, mShip);
			loadSave = mHud->getLoadingText()->isShow();
		}
		else
		{
			if (loadSave)
			{
				mDelta = 0.0;
				loadSave = false;
				mHud->getLoadingText()->setShow(false);
				quickSave->loadSave(mI, mShip, mPlanets);
				mHud->showHud();
			}

			mHud->updateHud(mShip);

			if (mI->_input->onKeyPress(controls->getQuickSave()))
			{
				quickSave->makeSave(mI, mShip, mPlanets);
			}

			if (mI->_input->onKeyPress(controls->getQuickLoad()))
			{
				deleteObjects(mHud, mShip, mPlanets);
				loadSave = true;
			}
			else
			{
				checkCollisions(mI, mPlanets, mShip);
				mShip->updateShip(controls, mDelta);
				for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
				{
					(*it)->updatePlanet(mDelta);
				}
			}
		}
		//mI->_render->showFpsInWindowTitle();
		mI->_input->update();
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();
	}

	// ----- Free -----
	delete controls;
	delete error;
	delete mHud;
	delete mMenu;
	delete quickSave;
	mI->_surfaceManager->remove(mSurfaceBack);
	mI->_entity2dManager->remove(mBack);
	deleteObjects(mHud, mShip, mPlanets);
	mI->end();
	return 0;
}

void checkCollisions(CIndieLib* const mI, vector<Planet*> mPlanets, Ship* mShip)
{
	for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
	{
		if (mI->_entity2dManager->isCollision((*it)->getEntity2d(), "planet", mShip->getEntity2d(), "ship_vertice")
		    || mI->_entity2dManager->isCollision((*it)->getEntity2d(), "planet", mShip->getEntity2d(), "ship_body"))
		{
			mShip->setSpeedX((*it)->getSpeedX() * 1.5f);
			mShip->setSpeedY((*it)->getSpeedY() * 1.5f);
			if (mI->_entity2dManager->isCollision((*it)->getEntity2d(), "planet", mShip->getEntity2d(), "ship_vertice"))
			{
				// Note: Y-axis is inverted, i.e. moving up is a negative movement
				if ((cos(mShip->getAngleZRadian()) > 0 && (*it)->getSpeedY() > 0) || (cos(mShip->getAngleZRadian()) < 0 && (*it)->getSpeedY() < 0))
				{
					mShip->setAngularSpeed(300.0f);
				}
				else
				{
					mShip->setAngularSpeed(-300.0f);
				}
			}
		}
	}
}