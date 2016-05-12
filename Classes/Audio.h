#ifndef _AUDIO_H_
#define _AUDIO_H_
#include"cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion; 

class Audio{
public:
	Audio();
	static Audio* getInstance();
	void play(int num);
	void changeSoundVol(double percent);
	void changeMusicVol(double percent);
	void setSound(bool b); 
	void setMusic(bool b);
private:
	static Audio* _instance;
	bool _back = true;
	bool _music = true;
};
#endif