#ifndef SHIP_H
#define SHIP_H

#include "irrKlang.h"
#include "IND_Timer.h"
#include "Object.h"
#include "Bullet.h"
#include "Controls.h"
#include "IND_Animation.h"

class Ship : public Object {
private:
	//action/gameplay related
	int health;
	int numFiredBullets;
	int score;
	short lastHitPlanet;
	//movement related
	float speed; //to remove
	float acceleration;
	float jolt;
	float maxSpeed;

	IND_Animation* mAnimationStill;
	IND_Animation* mAnimationShip;
	IND_Animation* mAnimationLeft;
	IND_Animation* mAnimationRight;
	IND_Animation* mAnimationExplode;

	IND_Entity2d* mAnim2dShip;

	IND_Timer* timer;

	// irrKlang Sound Engine
	irrklang::ISoundEngine* soundEngine;

	// movement sound
	irrklang::ISound* rocketSound;
	// blaster sound
	irrklang::ISoundSource* blasterSoundSource;
	irrklang::ISound* blasterSound;
	// explosion sound
	irrklang::ISoundSource* explodeSoundSource; // add to load
	irrklang::ISound* explodeSound; // add to load

	vector<Bullet*> mBullets;
public:
	Ship(int health = 100, int numFiredBullets = 0, int score = 0, float acceleration = 0.0f, float jolt = 50.0f, float maxSpeed = 2000.0f);

	// @overwrite
	virtual float getAngleZRadian() const;

	int getHealth() const;
	void setHealth(int);
	int getNumFiredBullets() const;
	void setNumFiredBullets(int);
	int getScore()const;
	void setScore(int);
	short getLastHitPlanet() const;
	void setLastHitPlanet(short);

	float getSpeed() const;
	void setSpeed(float);
	float getAcceleration() const;
	void setAcceleration(float);
	float getJolt() const;
	void setJolt(float);
	float getMaxSpeed() const;
	void setMaxSpeed(float);

	IND_Animation* getAnimationStill() const;
	void setAnimationStill(IND_Animation*);
	IND_Animation* getAnimationShip() const;
	void setAnimationShip(IND_Animation*);
	IND_Animation* getAnimationLeft() const;
	void setAnimationLeft(IND_Animation*);
	IND_Animation* getAnimationRight() const;
	void setAnimationRight(IND_Animation*);
	IND_Animation* getAnimationExplode() const;
	void setAnimationExplode(IND_Animation*);

	IND_Entity2d* getAnim2dShip() const;
	void setAnim2dShip(IND_Entity2d*);

	IND_Timer* getTimer() const;
	void setTimer(IND_Timer*);

	irrklang::ISoundEngine* getSoundEngine() const;
	void setSoundEngine(irrklang::ISoundEngine*);

	irrklang::ISound* getRocketSound() const;
	void setRocketSound(irrklang::ISound*);

	irrklang::ISoundSource* getBlasterSoundSource() const;
	void setBlasterSoundSource(irrklang::ISoundSource*);
	irrklang::ISound* getBlasterSound() const;
	void setBlasterSound(irrklang::ISound*);

	irrklang::ISoundSource* getExplodeSoundSource() const;
	void setExplodeSoundSource(irrklang::ISoundSource*);
	irrklang::ISound* getExplodeSound() const;
	void setExplodeSound(irrklang::ISound*);

	vector<Bullet*>& getBullets();
	void setBullets(vector<Bullet*>);

	// Reseting the animation, also resets the properties of Anim2d
	// Use this function to load them from the object entity that holds the surface which never changes
	void loadPropsAnim2d();

	void createShip(CIndieLib* const, const char*, const float, const float);
	void updateShip(Controls*, float);

	~Ship();
};
#endif