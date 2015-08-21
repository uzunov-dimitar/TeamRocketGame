#include "SoundEngine.h"

// irrKlang Sound Engine
irrklang::ISoundEngine* SoundEngine::soundEngine = NULL;

// movement sound
irrklang::ISound* SoundEngine::rocketSound = NULL;
// blaster sound
irrklang::ISoundSource* SoundEngine::blasterSoundSource = NULL;
irrklang::ISound* SoundEngine::blasterSound = NULL;
// explosion sound
irrklang::ISoundSource* SoundEngine::explodeSoundSource = NULL;
irrklang::ISound* SoundEngine::explodeSound = NULL;

irrklang::ISoundEngine* const SoundEngine::getSoundEngine()
{
	return SoundEngine::soundEngine;
}

void SoundEngine::setSoundEngine(irrklang::ISoundEngine* soundEngine)
{
	SoundEngine::soundEngine = soundEngine;
}

irrklang::ISound * const SoundEngine::getRocketSound()
{
	return rocketSound;
}

void SoundEngine::setRocketSound(irrklang::ISound* rocketSound)
{
	SoundEngine::rocketSound = rocketSound;
}

irrklang::ISoundSource * const SoundEngine::getBlasterSoundSource()
{
	return blasterSoundSource;
}

void SoundEngine::setBlasterSoundSource(irrklang::ISoundSource* blasterSoundSource)
{
	SoundEngine::blasterSoundSource = blasterSoundSource;
}

irrklang::ISound * const SoundEngine::getBlasterSound()
{
	return blasterSound;
}

void SoundEngine::setBlasterSound(irrklang::ISound* blasterSound)
{
	SoundEngine::blasterSound = blasterSound;
}

irrklang::ISoundSource * const SoundEngine::getExplodeSoundSource()
{
	return explodeSoundSource;
}

void SoundEngine::setExplodeSoundSource(irrklang::ISoundSource* explodeSoundSource)
{
	SoundEngine::explodeSoundSource = explodeSoundSource;
}

irrklang::ISound * const SoundEngine::getExplodeSound()
{
	return explodeSound;
}

void SoundEngine::setExplodeSound(irrklang::ISound* explodeSound)
{
	SoundEngine::explodeSound = explodeSound;
}

bool SoundEngine::initialize()
{
	setSoundEngine(irrklang::createIrrKlangDevice());

	if (!getSoundEngine())
	{
		return false;
	}

	setRocketSound(getSoundEngine()->play2D("../SpaceGame/irrKlang/media/v-start.wav", true, true, true));

	setBlasterSoundSource(getSoundEngine()->addSoundSourceFromFile("../SpaceGame/irrKlang/media/blaster.wav"));
	setBlasterSound(getSoundEngine()->play2D("../SpaceGame/irrKlang/media/blaster.wav", false, true, true));

	setExplodeSoundSource(getSoundEngine()->addSoundSourceFromFile("../SpaceGame/irrKlang/media/explosion.wav"));
	setExplodeSound(getSoundEngine()->play2D("../SpaceGame/irrKlang/media/explosion.wav", false, true, true));

	getSoundEngine()->setSoundVolume(0.1f);

	return true;
}
