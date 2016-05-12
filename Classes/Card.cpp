#include"Card.h"

Card* Card::create(const std::string& filename, int num, Suit suit, bool isFront){

	Card* card = new Card();
	if (card&&card->initWithSpriteFrameName(filename)){
		card->_num = num;
		card->_suit = suit;
		if (!isFront){
			card->_forChange = card->getDisplayFrame();
			card->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bk2.png"));
			card->_isFront = false;
		}
		else{
			card->_isFront = true;
			card->_forChange = SpriteFrameCache::getInstance()->getSpriteFrameByName("bk2.png");
		}
		card->setAnchorPoint(Vec2(0.5, 1));
		card->autorelease();
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
}
Vec2 Card::getRealPos(){
	return Vec2((this->getY() + 1) * 87, 580 - this->getX() * 20);
}
void Card::change(){
	auto se = Sequence::create(ScaleTo::create(0.15, 0,0.7), 
			 CCCallFunc::create([&](){this->setFlippedX(true);this->change2();}),
			 ScaleTo::create(0.15, -.7,0.7),
			NULL);
	this->runAction(se);
}
void Card::change2(){
	auto save = this->getDisplayFrame();
	this->setDisplayFrame(_forChange);
	this->_forChange = save;
	_isFront = !_isFront;
	return;
}

