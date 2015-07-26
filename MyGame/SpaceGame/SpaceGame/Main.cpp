#include <ctime>
#include <vector>
#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "Controls.h"
#include "Planet.h"
#include "Save.h"
#include "Ship.h"

/*
==================
Main
==================
*/
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

	IND_Entity2d* mBack = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBack);
	mBack->setSurface(mSurfaceBack);
	mBack->setScale((float)winWidth / mSurfaceBack->getWidth(), (float)winHeight / mSurfaceBack->getHeight());

	Controls* controls = new Controls();
	controls->loadSettings();

	ErrorHandler* error = new ErrorHandler();
	error->initialize(mI);

	vector<Planet*> mPlanets;
	for (int i = 0; i < 8; i++)
	{
		mPlanets.push_back(new Planet());

		float randPercent = (0.15f - (rand()%10+1)/100.0f);
		float randDegree = (i*M_PI / 4.0f) + (rand() % 60 + 1) / 100.0f;

		float radius = (1 - randPercent)*winHeight/2.0f;
		float posX = winWidth / 2.0 + radius*cos(randDegree);
		float posY = winHeight / 2.0 + radius*sin(randDegree);

		mPlanets.back()->createPlanet(mI, ("resources/Planets/" + to_string(i + 1) + ".png").c_str(), posX, posY, randPercent);
		mPlanets.back()->setAngleZ(-randDegree / M_PI * 180);
	}

	Ship* mShip = new Ship(100, winWidth/7.0f, winWidth/3.0f);
	mShip->createShip(mI, "resources/Spaceship with motor new/1.png", winWidth/2, winHeight/2);

	Save* quickSave = new Save();

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		if (mI->_input->onKeyPress(controls->getQuickSave()))
		{
			quickSave->makeSave(mI, mShip, mPlanets);
		}
		mShip->moveShip(controls);
		int i = 1;
		for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
		{
			(*it)->movePlanet();
			i++;
		}
		//mI->_render->showFpsInWindowTitle();
		mI->_input->update();
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();
	}

	// ----- Free -----

	mI->end();

	return 0;
}