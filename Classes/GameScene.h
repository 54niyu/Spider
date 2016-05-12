#include<iostream>
#include"cocos2d.h"
#include"Card.h"
#include"SimpleAudioEngine.h"
#include"GameData.h"


class GameScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);
	//剩余待发牌数
	int resNum = 4;
	//代发牌
	std::vector<cocos2d::Sprite*> dealCards;
	
	//8个完成时卡牌放置的卡槽，finishNum为完成的列数
	int finishNum = 0;
	std::vector<cocos2d::Node*> finishSlots;

	//维护一个指向十个队列的头和尾
	std::vector<cocos2d::Sprite*> slots;
	std::vector<cocos2d::Sprite*> tails;

	//记录操作，用于撤销
	std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>> records;
	std::vector<bool> change;

	//用于判断产生一次提示信息后是否有牌移动
	//如移动则重新生成
	bool isChange = false;
	std::vector<std::pair<Card*, Card*>> promptQueue;

	void moveTail(Card*, int,bool);//辅助函数，移动一列牌
	void cancel(cocos2d::Ref* pSender);//撤销一步
	void prompt(cocos2d::Ref* pSender);//提示
	void cancelDeal();//撤销发牌
	Card* judge(int );//判断i列是否完成
	void moveToWin(Card*);//赢牌移动
	void cancelWind();//撤销赢牌
	void dealCard(int hard);



	GameData* gInfo;

};