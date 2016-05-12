#ifndef _CARD_H_
#define _CARD_H_
#include"cocos2d.h"

USING_NS_CC;

typedef enum{
	//����
	Pades,
	//����
	Hearts,
	//÷��
	Clubs ,
	//����(��)
	Diamonds, 
} Suit;

class Card :public cocos2d::Sprite
{
public:

	Card() :_num(0), _suit(Suit::Clubs), _isFront(false), _forChange(nullptr), _x(-1), _y(-1){};
	static Card* create(const std::string& filename, int num, Suit suit, bool isFront);
	Vec2 getRealPos();
	void change();//����
	void change2();

	CC_SYNTHESIZE(int, _num, Num);//����
	CC_SYNTHESIZE(Suit, _suit, Suit);//��ɫ
	CC_SYNTHESIZE(bool, _isFront, IsFront);//������
	CC_SYNTHESIZE(cocos2d::SpriteFrame*, _forChange, ForChange);//�洢��һ���ͼ��
	CC_SYNTHESIZE(int, _x, X);		//��¼λ��
	CC_SYNTHESIZE(int, _y, Y);
};

#endif