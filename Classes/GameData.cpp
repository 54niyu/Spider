#include"GameData.h"

GameData* GameData::_instance = nullptr;

GameData* GameData::getInstance(){
	if (_instance == nullptr){
		_instance = new GameData();
	}
	return _instance;
}

GameData::GameData() :_hard(0), _scores(500){
	_hard = cocos2d::UserDefault::getInstance()->getIntegerForKey("hard", 0);
}
void GameData::saveHard(){

	cocos2d::UserDefault::getInstance()->setIntegerForKey("hard", _hard);
}
int GameData::getHard(){
	return cocos2d::UserDefault::getInstance()->getIntegerForKey("hard", _hard);
}