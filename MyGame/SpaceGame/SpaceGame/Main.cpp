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

void  deleteObjects(Ship*, vector<Planet*>&);

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
	for (int i = 0; i < 8; i++)
	{
		mPlanets.push_back(new Planet());

		// Random Percent 
		float randPercent = (0.05f + (rand()%5+1)/100.0f);
		float randDegree = (i*M_PI / 4.0f); //+ (rand() % 60 + 1) / 100.0f;

		float radius = (1 - randPercent)*winHeight/2.0f;
		float posX = winWidth / 2.0 + radius*cos(randDegree);
		float posY = winHeight / 2.0 + radius*sin(randDegree);

		// there are 3 orbits: 50%, 65%, and 80% of half of the window's height
		float orbitRadius = (winHeight / 2.0f) * ((60 + 15 * (i % 3)) / 100.0f);

		mPlanets.back()->createPlanet(mI, ("resources/Planets/" + to_string(i+1) + ".png").c_str(), posX, posY, -randDegree, randPercent, rand()%2, orbitRadius);
		while (mPlanets.back()->addSatellite());
	}

	Ship* mShip = new Ship(100, 0, 0, 0, winWidth/20.0f, winWidth/3.0f);
	mShip->createShip(mI, "resources/Spaceship with motor new/1.png", winWidth/2, winHeight/2);

	bool loadSave = false;
	float mDelta = 0.0f;
	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		mMenu->updateMenu();
		// get delta time
		mDelta = mI->_render->getFrameTime() / 1000.0f;

		mHud->updateHud(mShip);

		if (loadSave)
		{
			mDelta = 0.0;
			loadSave = false;
			mHud->getLoadingText()->setShow(false);
			quickSave->loadSave(mI, mShip, mPlanets);
			mHud->showHud();
		}

		if (mI->_input->onKeyPress(controls->getQuickSave()))
		{
			quickSave->makeSave(mI, mShip, mPlanets);
		}

		if (mI->_input->onKeyPress(controls->getQuickLoad()))
		{
			deleteObjects(mShip, mPlanets);
			mHud->hideHud();
			mHud->getLoadingText()->setShow(true);
			loadSave = true;
		}
		else
		{
			mShip->updateShip(controls, mDelta);
			for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
			{
				(*it)->updatePlanet(mDelta);
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
	deleteObjects(mShip, mPlanets);
	mI->end();
	return 0;
}

void  deleteObjects(Ship* mShip, vector<Planet*>& mPlanets)
{
	delete mShip;
	for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
	{
		delete (*it);
	}
	mPlanets.clear();
}