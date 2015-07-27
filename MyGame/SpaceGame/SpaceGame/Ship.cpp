#include "Ship.h"

Ship::Ship(int health, float acceleration, float maxSpeed) : health(health), speed(0.0f), acceleration(acceleration), maxSpeed(maxSpeed), mAnimationStill(IND_Animation::newAnimation()), mAnimationShip(IND_Animation::newAnimation()), mAnimationLeft(IND_Animation::newAnimation()), mAnimationRight(IND_Animation::newAnimation()), mAnim2dShip(IND_Entity2d::newEntity2d())
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
	initialize(mI);

	// Load Surface + Animations
	setPathSurface(path);
	getMI()->_surfaceManager->add(getSurface(), path, IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationShip(), "resources/Spaceship with motor new/Ship_advance.xml", IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationStill(), "resources/Spaceship with motor new/Ship_still.xml", IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationLeft(), "resources/Spaceship rotation smoke/LEFT/Rotate_left.xml", IND_ALPHA, IND_32);

	getMI()->_animationManager->addToSurface(getAnimationRight(), "resources/Spaceship rotation smoke/RIGHT/Rotate_right.xml", IND_ALPHA, IND_32);

	// Manage the 2D entities - both entity2d and mAnim2dShip

	getMI()->_entity2dManager->add(getEntity2d());
	getMI()->_entity2dManager->add(getAnim2dShip());

	getEntity2d()->setSurface(getSurface());
	getEntity2d()->setHotSpot(0.5f, 0.5f); // comment out to see both entity2d and mAnim2dShip
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
	float scale = ((0.05*(winWidth / entWidth)) < (0.15*(winHeight / entHeight)) ? (0.05*(winWidth / entWidth)) : (0.15*(winHeight / entHeight)));

	// scaleX and scaleY are always equal
	setScale(scale, scale);
	getAnim2dShip()->setScale(scale, scale);

}

void Ship::moveShip(Controls* controls)
{
	// get delta time
	float mDelta = getMI()->_render->getFrameTime() / 1000.0f;

	// Set Rotated Animation and Restart the Sequence
	// IMPORTANT: everytime the animation is reset, the properties of the mAnim2dShip are reset
	// Use loadPropsAnim2d() at the end of the function

	// Move or Deaccelerate

	if (getMI()->_input->isKeyPressed(controls->getAdvance()))
	{
		getAnim2dShip()->setAnimation(getAnimationShip());

		// restart the animation on key press
		if (getMI()->_input->onKeyPress(controls->getAdvance()))
		{
			getAnim2dShip()->setSequence(0);
		}

		setPosX(getPosX() + getSpeed() * cos(getAngleZRadian()) * mDelta);
		setPosY(getPosY() - getSpeed() * sin(getAngleZRadian()) * mDelta);
		this->checkCoords();

		setSpeed(getSpeed() + ((getSpeed() + getAcceleration()*mDelta) < getMaxSpeed() ?
																						getAcceleration()*mDelta :
																						0));
	}
	else
	{
		getAnim2dShip()->setAnimation(getAnimationStill());

		// Deaccelerate

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

		// restart the animation on key press
		if (getMI()->_input->onKeyPress(controls->getRotateLeft()) || getMI()->_input->onKeyPress(controls->getRotateRight()))
		{
			getAnim2dShip()->setSequence(0);
		}

		// Rotate with 180 degrees per second

		setAngleZ(getAngleZ() + (getMI()->_input->isKeyPressed(controls->getRotateLeft()) ? -1 : 1) * 180 * mDelta);
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
	/*getAnimationShip()->destroy();
	getAnimationStill()->destroy();
	getAnimationLeft()->destroy();
	getAnimationRight()->destroy();
	getAnim2dShip()->destroy();*/
}