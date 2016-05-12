#ifndef _CARD_H_
#define _CARD_H_
#include"cocos2d.h"

USING_NS_CC;

typedef enum{
	//黑桃
	Pades,
	//红桃
	Hearts,
	//梅花
	Clubs ,
	//方块(红)
	Diamonds, 
} Suit;

class Card :public cocos2d::Sprite
{
public:

	Card() :_num(0), _suit(Suit::Clubs), _isFront(false), _forChange(nullptr), _x(-1), _y(-1){};
	static Card* create(const std::string& filename, int num, Suit suit, bool isFront);
	Vec2 getRealPos();
	void change();//翻牌
	void change2();

	CC_SYNTHESIZE(int, _num, Num);//数字
	CC_SYNTHESIZE(Suit, _suit, Suit);//花色
	CC_SYNTHESIZE(bool, _isFront, IsFront);//正反面
	CC_SYNTHESIZE(cocos2d::SpriteFrame*, _forChange, ForChange);//存储另一面的图像
	CC_SYNTHESIZE(int, _x, X);		//记录位置
	CC_SYNTHESIZE(int, _y, Y);
};

#endif