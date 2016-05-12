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
	//ʣ���������
	int resNum = 4;
	//������
	std::vector<cocos2d::Sprite*> dealCards;
	
	//8�����ʱ���Ʒ��õĿ��ۣ�finishNumΪ��ɵ�����
	int finishNum = 0;
	std::vector<cocos2d::Node*> finishSlots;

	//ά��һ��ָ��ʮ�����е�ͷ��β
	std::vector<cocos2d::Sprite*> slots;
	std::vector<cocos2d::Sprite*> tails;

	//��¼���������ڳ���
	std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>> records;
	std::vector<bool> change;

	//�����жϲ���һ����ʾ��Ϣ���Ƿ������ƶ�
	//���ƶ�����������
	bool isChange = false;
	std::vector<std::pair<Card*, Card*>> promptQueue;

	void moveTail(Card*, int,bool);//�����������ƶ�һ����
	void cancel(cocos2d::Ref* pSender);//����һ��
	void prompt(cocos2d::Ref* pSender);//��ʾ
	void cancelDeal();//��������
	Card* judge(int );//�ж�i���Ƿ����
	void moveToWin(Card*);//Ӯ���ƶ�
	void cancelWind();//����Ӯ��
	void dealCard(int hard);



	GameData* gInfo;

};