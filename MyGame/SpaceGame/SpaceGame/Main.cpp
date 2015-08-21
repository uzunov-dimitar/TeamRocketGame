#include <ctime>
#include <vector>
#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "IND_Timer.h"
#include "Controls.h"
#include "Hud.h"
#include "Menu.h"
#include "Save.h"
#include "SoundEngine.h"
#include "Planet.h"
#include "Ship.h"
#include "Satellite.h"
/*
==================
Main
==================
*/

void checkShipPlanetsCollisions(CIndieLib* const, vector<Planet*>, Ship*);
void checkBulletsPlanetsCollisions(CIndieLib* const, vector<Planet*>, Ship*);
void checkBulletsShipCollisions(CIndieLib* const, vector<Planet*>, Ship*);

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

	if (!SoundEngine::initialize())
	{
		error->writeError(200, 100, "Error", "SoundEngine");
	}

	vector<Planet*> mPlanets;
	Ship* mShip = NULL;

	bool loadSave = false;
	float mDelta = 0.0f;

	IND_Timer* mTimer = new IND_Timer;
	mTimer->start();

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit() && !mMenu->isExit())
	{
		// get delta time
		mDelta = mI->_render->getFrameTime() / 1000.0f;

		if (mI->_input->isKeyPressed(controls->getMenu()))
		{
			mMenu->show();
			SoundEngine::getSoundEngine()->setAllSoundsPaused(true);
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
				SoundEngine::getSoundEngine()->setAllSoundsPaused(true);
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
				if (mShip->isDestroyed())
				{
					SoundEngine::getSoundEngine()->setAllSoundsPaused(true);
					mHud->updateGameOverText(mShip->getScore());
					deleteObjects(mHud, mShip, mPlanets);
					mHud->getLoadingText()->setShow(false);
					mHud->getGameOverText()->setShow(true);
					mMenu->show();
				}
				if(mShip!=NULL)
				{
					//----- Collisions -----
					checkShipPlanetsCollisions(mI, mPlanets, mShip);
					checkBulletsPlanetsCollisions(mI, mPlanets, mShip);
					checkBulletsShipCollisions(mI, mPlanets, mShip);

					//----- Movement update -----
					mShip->updateShip(controls, mDelta);
					if ((mTimer->getTicks() / 1000.0f) >= 3.0f)
					{
						mTimer->start();
						mPlanets.at(rand() % mPlanets.size())->addSatellite();
					}
					for (vector<Planet*>::iterator it = mPlanets.begin(); it != mPlanets.end(); ++it)
					{
						(*it)->updatePlanet(mDelta, (mShip->getPosX() + 0.25f * mShip->getHeight() * cos(mShip->getAngleZRadian())), (mShip->getPosY() - 0.25f * mShip->getHeight() * sin(mShip->getAngleZRadian())));
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
			if (mShip->getTimer() > 1.0f || mShip->getLastHitPlanet() != (it - mPlanets.begin()))
			{
				mShip->setHealth(mShip->getHealth() - 10);
				mShip->setTimer(0.0f);
				mShip->setLastHitPlanet(it - mPlanets.begin());
			}
			
		}

		// check satellites collisions
		for (vector<Satellite*>::iterator itSatellite = (*it)->getSatellites().begin(); itSatellite != (*it)->getSatellites().end(); ++itSatellite)
		{
			if (mI->_entity2dManager->isCollision((*itSatellite)->getEntity2d(), "satellite", mShip->getEntity2d(), "ship_vertice")
				|| mI->_entity2dManager->isCollision((*itSatellite)->getEntity2d(), "satellite", mShip->getEntity2d(), "ship_body"))
			{
				mShip->setHealth(mShip->getHealth() - 5);
				mShip->setScore(mShip->getScore() + 10);
				(*itSatellite)->destroy();
				SoundEngine::getExplodeSound()->drop();
				SoundEngine::setExplodeSound(SoundEngine::getSoundEngine()->play2D(SoundEngine::getExplodeSoundSource(), false, false, true));
			}
		}
	}
}

void checkBulletsPlanetsCollisions(CIndieLib* const mI, vector<Planet*> mPlanets, Ship* mShip)
{
	for (vector<Planet*>::iterator itPlanet = mPlanets.begin(); itPlanet != mPlanets.end(); ++itPlanet)
	{
		for (vector<Bullet*>::iterator itBullet = mShip->getBullets().begin(); itBullet != mShip->getBullets().end(); ++itBullet)
		{
			if (mI->_entity2dManager->isCollision((*itPlanet)->getEntity2d(), "planet", (*itBullet)->getEntity2d(), "bullet"))
			{
				delete *itBullet;
				mShip->getBullets().erase(itBullet);
				--itBullet;
				break;
			}
			for (vector<Satellite*>::iterator itSatellite = (*itPlanet)->getSatellites().begin(); itSatellite != (*itPlanet)->getSatellites().end(); ++itSatellite)
			{
				if (mI->_entity2dManager->isCollision((*itSatellite)->getEntity2d(), "satellite", (*itBullet)->getEntity2d(), "bullet"))
				{
					delete *itBullet;
					mShip->getBullets().erase(itBullet);
					--itBullet;
					(*itSatellite)->destroy();

					mShip->setScore(mShip->getScore() + 10);
					SoundEngine::getExplodeSound()->drop();
					SoundEngine::setExplodeSound(SoundEngine::getSoundEngine()->play2D(SoundEngine::getExplodeSoundSource(), false, false, true));
					break;
				}
			}
		}
	}
}

void checkBulletsShipCollisions(CIndieLib* const mI, vector<Planet*> mPlanets, Ship* mShip)
{
	for (vector<Planet*>::iterator itPlanet = mPlanets.begin(); itPlanet != mPlanets.end(); ++itPlanet)
	{
		for (vector<Bullet*>::iterator itBullet = (*itPlanet)->getBullets().begin(); itBullet != (*itPlanet)->getBullets().end(); ++itBullet)
		{
			if (mI->_entity2dManager->isCollision((*itBullet)->getEntity2d(), "bullet", mShip->getEntity2d(), "ship_vertice")
				|| mI->_entity2dManager->isCollision((*itBullet)->getEntity2d(), "bullet", mShip->getEntity2d(), "ship_body"))
			{
				mShip->setHealth(mShip->getHealth() - 5);
				delete *itBullet;
				(*itPlanet)->getBullets().erase(itBullet);
				--itBullet;
				break;
			}
			for (vector<Planet*>::iterator itPlanetCollide = mPlanets.begin(); itPlanetCollide != mPlanets.end(); ++itPlanetCollide)
			{
				if (itPlanet != itPlanetCollide 
					&& mI->_entity2dManager->isCollision((*itBullet)->getEntity2d(), "bullet", (*itPlanetCollide)->getEntity2d(), "planet"))
				{
					delete *itBullet;
					(*itPlanet)->getBullets().erase(itBullet);
					--itBullet;
					break;
				}
				bool satelliteCollision = false;
				for (vector<Satellite*>::iterator itSatellite = (*itPlanetCollide)->getSatellites().begin(); itSatellite != (*itPlanetCollide)->getSatellites().end(); ++itSatellite)
				{
					if (mI->_entity2dManager->isCollision((*itSatellite)->getEntity2d(), "satellite", (*itBullet)->getEntity2d(), "bullet"))
					{
						delete *itBullet;
						(*itPlanet)->getBullets().erase(itBullet);
						--itBullet;
						(*itSatellite)->destroy();

						SoundEngine::getExplodeSound()->drop();
						SoundEngine::setExplodeSound(SoundEngine::getSoundEngine()->play2D(SoundEngine::getExplodeSoundSource(), false, false, true));

						satelliteCollision = true;
						break;
					}
				}

				if (satelliteCollision)
				{
					break;
				}
			}
		}
	}
}