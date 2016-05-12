#ifndef _GAMEDATA_H_
#define _GAMEDATA_H_

#include"cocos2d.h"

class GameData{
public:
	static GameData* getInstance();

	int getHard();// { return _hard; };
	void setHard(int hard){ _hard = hard; };
	void saveHard();
	int getScores(){ return _scores; };
	void setScores(int sc){ _scores = sc; };
	void addScores(int more){ _scores += more; }


private:
	GameData();
	static GameData* _instance;
	int _hard;
	int _scores;
};

#endif