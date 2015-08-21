#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "irrKlang.h"

class SoundEngine {
private:
	// irrKlang Sound Engine
	static irrklang::ISoundEngine* soundEngine;

	// movement sound
	static irrklang::ISound* rocketSound;
	// blaster sound
	static irrklang::ISoundSource* blasterSoundSource;
	static irrklang::ISound* blasterSound;
	// explosion sound
	static irrklang::ISoundSource* explodeSoundSource;
	static irrklang::ISound* explodeSound;
public:
	static irrklang::ISoundEngine* const getSoundEngine();
	static void setSoundEngine(irrklang::ISoundEngine*);

	static irrklang::ISound* const getRocketSound();
	static void setRocketSound(irrklang::ISound*);

	static irrklang::ISoundSource* const getBlasterSoundSource();
	static void setBlasterSoundSource(irrklang::ISoundSource*);
	static irrklang::ISound* const getBlasterSound();
	static void setBlasterSound(irrklang::ISound*);

	static irrklang::ISoundSource* const getExplodeSoundSource();
	static void setExplodeSoundSource(irrklang::ISoundSource*);
	static irrklang::ISound* const getExplodeSound();
	static void setExplodeSound(irrklang::ISound*);

	static bool initialize();
};
#endif