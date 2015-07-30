#include "Ship.h"

Ship::Ship(int health, float acceleration, float jolt, float maxSpeed) : health(health), speed(0.0f), acceleration(acceleration), jolt(jolt), maxSpeed(maxSpeed), mAnimationStill(IND_Animation::newAnimation()), mAnimationShip(IND_Animation::newAnimation()), mAnimationLeft(IND_Animation::newAnimation()), mAnimationRight(IND_Animation::newAnimation()), mAnim2dShip(IND_Entity2d::newEntity2d()), soundEngine(NULL)
{
}

int Ship::getHealth() const
{
	return health;;
}

void Ship::setHealth(int health)
{
	this->health = health;
}

float Ship::getSpeed() const
{
	return speed;
}

void Ship::setSpeed(float speed)
{
	this->speed = speed;
}

float Ship::getAcceleration() const
{
	return acceleration;
}

void Ship::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}

float Ship::getJolt() const
{
	return jolt;
}

void Ship::setJolt(float  jolt)
{
	this->jolt = jolt;
}

float Ship::getMaxSpeed() const
{
	return maxSpeed;
}

void Ship::setMaxSpeed(float maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

IND_Animation * Ship::getAnimationStill() const
{
	return mAnimationStill;
}

void Ship::setAnimationStill(IND_Animation * mAnimationStill)
{
	this->mAnimationStill = mAnimationStill;
}

IND_Animation * Ship::getAnimationShip() const
{
	return mAnimationShip;
}

void Ship::setAnimationShip(IND_Animation * mAnimationShip)
{
	this->mAnimationShip = mAnimationShip;
}

IND_Animation * Ship::getAnimationLeft() const
{
	return mAnimationLeft;
}

void Ship::setAnimationLeft(IND_Animation * mAnimationLeft)
{
	this->mAnimationLeft = mAnimationLeft;
}

IND_Animation * Ship::getAnimationRight() const
{
	return mAnimationRight;
}

void Ship::setAnimationRight(IND_Animation * mAnimationRight)
{
	this->mAnimationRight = mAnimationRight;
}

IND_Entity2d * Ship::getAnim2dShip() const
{
	return mAnim2dShip;
}

void Ship::setAnim2dShip(IND_Entity2d * mAnim2dShip)
{
	this->mAnim2dShip = mAnim2dShip;
}

irrklang::ISoundEngine * Ship::getSoundEngine() const
{
	return soundEngine;
}

void Ship::setSoundEngine(irrklang::ISoundEngine* soundEngine)
{
	this->soundEngine = soundEngine;
}

irrklang::ISound* Ship::getRocketSound() const
{
	return rocketSound;
}

void Ship::setRocketSound(irrklang::ISound* rocketSound)
{
	this->rocketSound = rocketSound;
	this->rocketSound->setVolume(0.1f);
}

irrklang::ISoundSource * Ship::getBlasterSoundSource() const
{
	return blasterSoundSource;
}

void Ship::setBlasterSoundSource(irrklang::ISoundSource* blasterSoundSource)
{
	this->blasterSoundSource = blasterSoundSource;
	this->blasterSoundSource->setDefaultVolume(0.1f);
}

irrklang::ISound* Ship::getBlasterSound() const
{
	return blasterSound;
}

void Ship::setBlasterSound(irrklang::ISound* blasterSound)
{
	this->blasterSound = blasterSound;
}

vector<Bullet*>& Ship::getBullets()
{
	return mBullets;
}

void Ship::setBullets(vector<Bullet*> mBullets)
{
	this->mBullets = mBullets;
}

void Ship::loadPropsAnim2d()
{
	getAnim2dShip()->setHotSpot(0.5f, 0.5f);
	getAnim2dShip()->setPosition(getPosX(), getPosY(), 0);
	getAnim2dShip()->setScale(getScaleX(), getScaleY());
	getAnim2dShip()->setAngleXYZ(0, 0, getAngleZ());
}

void Ship::createShip(CIndieLib * const mI,const char * path, const float posX, const float posY)
{
	setMI(mI);

	// Sound:
	setSoundEngine(irrklang::createIrrKlangDevice());

	if (!getSoundEngine())
	{
		writeError(1000, 100, "SoundEngine", "can't create device.");
	}

	setRocketSound(getSoundEngine()->play2D("irrKlang/media/v-start.wav", true, true, true));
	setBlasterSoundSource(getSoundEngine()->addSoundSourceFromFile("irrKlang/media/blaster.wav"));
	setBlasterSound(getSoundEngine()->play2D("irrKlang/media/blaster.wav", false, true, true));
	getSoundEngine()->setSoundVolume(0.1f);

	// Load Surface + Animations
	setPathSurface(path);

	getMI()->_animationManager->addToSurface(getAnimationShip(), "resources/Spaceship with motor new/Ship_advance.xml", IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationStill(), "resources/Spaceship with motor new/Ship_still.xml", IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationLeft(), "resources/Spaceship rotation smoke/LEFT/Rotate_left.xml", IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationRight(), "resources/Spaceship rotation smoke/RIGHT/Rotate_right.xml", IND_ALPHA, IND_32);

	// Manage the 2D entities - both entity2d and mAnim2dShip

	getMI()->_entity2dManager->add(getEntity2d());
	getMI()->_entity2dManager->add(getAnim2dShip());

	getEntity2d()->setHotSpot(0.5f, 0.5f); // comment out to see both entity2d and mAnim2dShip
	getEntity2d()->setPosition(0, 0, 1);
	setPosition(posX, posY);

	getAnim2dShip()->setAnimation(getAnimationStill());
	getAnim2dShip()->setHotSpot(0.5f, 0.5f);
	getAnim2dShip()->setPosition(posX, posY, 0);

	// the scale is set so that the ship takes at maximum 5% of the width of the screen and 15% of the height
	// if taking 5% of the width requires taking more than 15% of the height,
	// the ship would take less than 5% of the width and exactly 15% of the height and vice versa

	int winWidth = getMI()->_window->getWidth();
	int winHeight = getMI()->_window->getHeight();
	float entWidth = getSurface()->getWidth();
	float entHeight = getSurface()->getHeight();
	float scale = ((0.05*(winWidth / entWidth)) < (0.15*(winHeight / entHeight)) ?
																				(0.05*(winWidth / entWidth)) :
																				(0.15*(winHeight / entHeight)));

	// scaleX and scaleY are always equal
	setScale(scale, scale);
	getAnim2dShip()->setScale(scale, scale);

}

void Ship::updateShip(Controls* controls, float mDelta)
{
	// IMPORTANT: everytime the animation is reset, the properties of the mAnim2dShip are reset
	// Use loadPropsAnim2d() at the end of the function

	// Check if shoot was pressed
	if (getMI()->_input->onKeyPress(controls->getShoot()))
	{
		getBlasterSound()->setIsPaused(false);
		if (getBlasterSound()->isFinished())
		{
			getBlasterSound()->drop();
			setBlasterSound(getSoundEngine()->play2D(getBlasterSoundSource(), false, false, true));
		}
		else
		{
			getBlasterSound()->setPlayPosition(0);
		}
		getBullets().push_back(new Bullet());
		getBullets().back()->createBullet(getMI(), "resources/green_beam.png", getPosX() + getWidth()*cos(getAngleZRadian() - M_PI / 6.0f) / 3.0f, getPosY() - getWidth()*sin(getAngleZRadian() - M_PI / 6.0f) / 3.0f, getAngleZ());
		getBullets().push_back(new Bullet());
		getBullets().back()->createBullet(getMI(), "resources/green_beam.png", getPosX() + getWidth()*cos(getAngleZRadian() + M_PI / 6.0f) / 3.0f, getPosY() - getWidth()*sin(getAngleZRadian() + M_PI / 6.0f) / 3.0f, getAngleZ());
	}

	// Move or Deaccelerate
	if (getMI()->_input->isKeyPressed(controls->getAdvance()))
	{
		getAnim2dShip()->setAnimation(getAnimationShip());

		// restart the animation on key press
		if (getMI()->_input->onKeyPress(controls->getAdvance()))
		{
			getAnim2dShip()->setSequence(0);

			getRocketSound()->setIsPaused(false);
		}

		

		setPosX(getPosX() + getSpeed() * cos(getAngleZRadian()) * mDelta);
		setPosY(getPosY() - getSpeed() * sin(getAngleZRadian()) * mDelta);
		this->checkCoords();

		setAcceleration(getAcceleration() + ((getSpeed() + getAcceleration()*mDelta) < getMaxSpeed() ?
																									getJolt()*mDelta :
																									0));

		setSpeed(getSpeed() + ((getSpeed() + getAcceleration()*mDelta) < getMaxSpeed() ?
																						getAcceleration()*mDelta :
																						0));
	}
	else
	{
		getAnim2dShip()->setAnimation(getAnimationStill());

		getRocketSound()->setIsPaused(true);
		// Deaccelerate

		setAcceleration(getAcceleration() - (getAcceleration() > 0 ?
																		getJolt()*mDelta :
																		0));

		setSpeed(getSpeed() - ((getSpeed() - getAcceleration()*mDelta) > 0 ?
																			getAcceleration()*mDelta :
																			0));
	}
	
	// In case Both the Up and either Left or Right keys are pressed, the later should overwrite the standard animation
	// Also perform the actual rotation on the ship

	if (getMI()->_input->isKeyPressed(controls->getRotateLeft()) || getMI()->_input->isKeyPressed(controls->getRotateRight()))
	{
		// Set Animation

		getAnim2dShip()->setAnimation(getMI()->_input->isKeyPressed(controls->getRotateLeft()) ?
																								getAnimationLeft() : 
																								getAnimationRight());

		getRocketSound()->setIsPaused(false);

		// restart the animation on key press
		if (getMI()->_input->onKeyPress(controls->getRotateLeft()) || getMI()->_input->onKeyPress(controls->getRotateRight()))
		{
			getAnim2dShip()->setSequence(0);
		}

		// Rotate with 180 degrees per second

		setAngleZ(getAngleZ() + (getMI()->_input->isKeyPressed(controls->getRotateLeft()) ? -1 : 1) * 180 * mDelta);
	}

	// Manage Bullets:
	for (vector<Bullet*>::iterator it = getBullets().begin(); it < getBullets().end(); ++it)
	{
			if ((*it)->updateBullet(mDelta))
			{
				vector<Bullet*>::iterator it2 = it;
				delete (*it);
				getBullets().erase(it);
			}
	}
	// load back the properties into mAnim2dShip
	this->loadPropsAnim2d();
}

Ship::~Ship()
{
	getMI()->_animationManager->remove(getAnimationStill());
	getMI()->_animationManager->remove(getAnimationShip());
	getMI()->_animationManager->remove(getAnimationLeft());
	getMI()->_animationManager->remove(getAnimationRight());
	getMI()->_entity2dManager->remove(getAnim2dShip());
	getSoundEngine()->drop();
	for (vector<Bullet*>::iterator it = getBullets().begin(); it != getBullets().end(); ++it)
	{
			delete (*it);
	}
	getBullets().clear();
	/*getAnimationShip()->destroy();
	getAnimationStill()->destroy();
	getAnimationLeft()->destroy();
	getAnimationRight()->destroy();
	getAnim2dShip()->destroy();*/
}