#include"Audio.h"

Audio* Audio::_instance = nullptr;

Audio* Audio::getInstance(){
	if (_instance == nullptr)
		_instance = new Audio;
	return _instance;
}

Audio::Audio(){
	SimpleAudioEngine::getInstance()->preloadEffect("specok.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("specselectcard.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("specsysreturnok.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("special_menu.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("special_mutiply.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("special_star.mp3");
}
void Audio::play(int num){
	if (!_back)	return;
	switch (num){
	case 0:SimpleAudioEngine::getInstance()->playEffect("specok.mp3"); break;
	case 1:SimpleAudioEngine::getInstance()->playEffect("specselectcard.mp3"); break;
	case 2:SimpleAudioEngine::getInstance()->playEffect("specsysreturnok.mp3"); break;
	case 3:SimpleAudioEngine::getInstance()->playEffect("special_menu.mp3"); break;
	case 4:SimpleAudioEngine::getInstance()->playEffect("special_mutiply.mp3"); break;
	case 5:SimpleAudioEngine::getInstance()->playEffect("special_star.mp3"); break;
	}
}

void Audio::changeSoundVol(double percent){
	CCLOG("sound::%f",percent);

	SimpleAudioEngine::getInstance()->setEffectsVolume(percent);
}
void Audio::changeMusicVol(double percent){
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent);
}
void Audio::setSound(bool b){
	_back = b;
}
void Audio::setMusic(bool b){
	_music = b;
}