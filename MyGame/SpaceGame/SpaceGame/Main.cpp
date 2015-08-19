#include <ctime>
#include <vector>
#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "IND_Timer.h"
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

void checkShipPlanetsCollisions(CIndieLib* const, vector<Planet*>, Ship*);
void checkBulletsPlanetsCollisions(CIndieLib* const, vector<Planet*>, vector<Bullet*>&);

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
	if (!mI->_surfaceManager->add(mSurfaceBack, "../SpaceGame/resources/Backgrounds/18.jpg", IND_OPAQUE, IND_32)) return 0;

	/*IND_Animation* mTestA = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mTestA, "resources/animations/dust.xml", IND_ALPHA, IND_32, 255, 0, 255)) return 0;
	mTestA->getActualFramePos(0);*/

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

	IND_Timer* mTimer = new IND_Timer;
	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit() && !mMenu->isExit())
	{
		// get delta time
		mDelta = mI->_render->getFrameTime() / 1000.0f;

		if (mI->_input->isKeyPressed(controls->getMenu()))
		{
			mMenu->show();
			//mShip->getSoundEngine()->setAllSoundsPaused(true);
		}
		if (!mMenu->isHidden())
		{
			mMenu->updateMenu(mHud, quickSave, mPlanets, mShip);
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

			if (mShip != NULL)
			{
				if (mI->_input->onKeyPress(controls->getQuickSave()))
				{
					quickSave->makeSave(mI, mShip, mPlanets);
				}

				mHud->updateHud(mShip);

				if (mI->_input->onKeyPress(controls->getQuickLoad()))
				{
					deleteObjects(mHud, mShip, mPlanets);
					loadSave = true;
				}
				else
				{
					//----- Collisions -----
					checkShipPlanetsCollisions(mI, mPlanets, mShip);
					checkBulletsPlanetsCollisions(mI, mPlanets, mShip->getBullets());

					//----- Movement update -----
					mShip->updateShip(controls, mDelta);
					for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
					{
						(*it)->updatePlanet(mDelta);
					}
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

void checkShipPlanetsCollisions(CIndieLib* const mI, vector<Planet*> mPlanets, Ship* mShip)
{
	float halfWidth = mI->_window->getWidth() / 2.0f;
	float halfHeight = mI->_window->getHeight() / 2.0f;
	for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
	{
		if (mI->_entity2dManager->isCollision((*it)->getEntity2d(), "planet", mShip->getEntity2d(), "ship_vertice")
		    || mI->_entity2dManager->isCollision((*it)->getEntity2d(), "planet", mShip->getEntity2d(), "ship_body"))
		{	
			// calculate the position of the ship relative to the planet
			// also do some calculations to shift the center of the rocket for better accuracy
			float posX = (mShip->getPosX() + 0.25f * mShip->getHeight() * cos(mShip->getAngleZRadian())) - (*it)->getPosX();
			float posY = (mShip->getPosY() - 0.25f * mShip->getHeight() * sin(mShip->getAngleZRadian())) - (*it)->getPosY();

			// put a minus for the inverted Y axis
			float newPosY =  posX * sin(-(*it)->getAngleZRadian()) - posY * cos(-(*it)->getAngleZRadian());

			bool result = (newPosY > 0.0f ? false : true);

			if (result)
			{
				mShip->setSpeedX(-(*it)->getSpeedX() * 2.0f);
				mShip->setSpeedY(-(*it)->getSpeedY() * 2.0f);
			}
			else
			{
				mShip->setSpeedX((*it)->getSpeedX() * 2.0f);
				mShip->setSpeedY((*it)->getSpeedY() * 2.0f);
				if (mI->_entity2dManager->isCollision((*it)->getEntity2d(), "planet", mShip->getEntity2d(), "ship_vertice"))
				{
					// Note: Y-axis is inverted, i.e. moving up is a negative movement
					if ((cos(mShip->getAngleZRadian()) > 0 && (*it)->getSpeedY() > 0) || (cos(mShip->getAngleZRadian()) < 0 && (*it)->getSpeedY() < 0))
					{
						mShip->setAngularSpeed(360.0f);
					}
					else
					{
						mShip->setAngularSpeed(-360.0f);
					}
				}
			}

			// decrease health
			if ((mShip->getTimer()->getTicks() / 1000.f) > 1.0f || mShip->getLastHitPlanet() != (it - mPlanets.begin()))
			{
				mShip->setHealth(mShip->getHealth() - 10);
				mShip->getTimer()->start();
				mShip->setLastHitPlanet(it - mPlanets.begin());
			}
			
		}

		// check satellites collisions
		for (vector<Satellite*>::iterator itSat = (*it)->getSatellites().begin(); itSat != (*it)->getSatellites().end(); ++itSat)
		{
			if (mI->_entity2dManager->isCollision((*itSat)->getEntity2d(), "satellite", mShip->getEntity2d(), "ship_vertice")
				|| mI->_entity2dManager->isCollision((*itSat)->getEntity2d(), "satellite", mShip->getEntity2d(), "ship_body"))
			{
				mShip->setHealth(mShip->getHealth() - 5);
				mShip->setScore(mShip->getScore() + 10);
				mShip->getExplodeSound()->drop();
				mShip->setExplodeSound(mShip->getSoundEngine()->play2D(mShip->getExplodeSoundSource(), false, false, true));
				(*itSat)->destroy();
			}
		}
	}
}

void checkBulletsPlanetsCollisions(CIndieLib* const mI, vector<Planet*> mPlanets, vector<Bullet*>& shotBullets)
{

	for (vector<Planet*>::iterator itPlanet = mPlanets.begin(); itPlanet != mPlanets.end(); ++itPlanet)
	{
		for (vector<Bullet*>::iterator itBullet = shotBullets.begin(); itBullet != shotBullets.end(); ++itBullet)
		{
			if (mI->_entity2dManager->isCollision((*itPlanet)->getEntity2d(), "planet", (*itBullet)->getEntity2d(), "bullet"))
			{
				delete *itBullet;
				shotBullets.erase(itBullet);
				--itBullet;
			}
			//for ()
		}
	}
}