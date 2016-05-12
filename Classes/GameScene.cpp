#include"GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

//判断次点击位置可否移动
bool canBeMoved(Card* c){
	if (!c->getIsFront())
		return false;
	while (c->getChildrenCount() != 0){
		c = static_cast<Card*>(c->getChildren().at(0));
		if (c->getNum() == (static_cast<Card*>(c->getParent())->getNum ()- 1)&&(c->getSuit()%2)==((static_cast<Card*>(c->getParent()))->getSuit()%2)){
			continue;
		}
		else{
			return false;
		}
	}
	return true;
}
//设置当前结点以及子节点透明度
void setOp(Card* c, int op){
	c->setOpacity(op);
	while (c->getChildrenCount() != 0){
		c = static_cast<Card*>(c->getChildren().at(0));
		c->setOpacity(op);
	}
}


//设置globalZorder  用来顶置
void setZorder(Card* c, int z){
	c->setGlobalZOrder(z++);
	while (c->getChildrenCount() != 0){
		c = static_cast<Card*>(c->getChildren().at(0));
		c->setGlobalZOrder(z++);
	}
}

//恢复
void reserveZorder(Card* c){
	c->setGlobalZOrder(0);
	while (c->getChildrenCount() != 0){
		c = static_cast<Card*>(c->getChildren().at(0));
		c->setGlobalZOrder(0);
	}
}

//判断一列是否完成
Card* GameScene::judge(int pos){

	auto temp = static_cast<Card*>(tails.at(pos));
	if (static_cast<Card*>(tails.at(pos))->getNum() == 0){
		int count = 1;
		auto ptr = temp->getParent();
		while (ptr != slots.at(pos) && static_cast<Card*>(ptr)->getIsFront() && static_cast<Card*>(ptr)->getNum() == temp->getNum() + 1&&static_cast<Card*>(ptr)->getSuit()%2==temp->getSuit()%2){
			temp = static_cast<Card*>(temp->getParent());
			count++;
			ptr = temp->getParent();
		}
		if (count == 13)
			return temp;
		else
			return NULL;
	}
	else{
		return NULL;
	}
}

//移动函数
void GameScene::moveTail(Card* target, int tail,bool isRecorde){
	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	//isRecorde 用于区别是否需要记录 移动 撤销操作不需要记录 
	Vec2 or(target->getX(), target->getY());
	int orign = target->getX();
	tails.at(orign) = static_cast<Sprite*>(target->getParent());

	target->retain();
	target->removeFromParentAndCleanup(false);
	if (!isRecorde){
		if (change.back()){
			static_cast<Card*>(tails.at(tail))->change();
			change.pop_back();
		}
		else{
			change.pop_back();
		}
	}
	tails.at(tail)->addChild(target);
	target->release();


	//空槽
	if (tails.at(tail) == slots.at(tail)){
		target->setX(tail);
		target->setY(0);
		target->setPosition(Vec2(0, 0));
		target->setAnchorPoint(Vec2::ZERO);
	}
	else{
		auto p = static_cast<Card*>(target->getParent());
		target->setX(tail);
		target->setY(p->getY()+1);
		target->setAnchorPoint(Vec2(0, 0.2));
		target->setPosition(Vec2::ZERO);
	}

	//更新尾节点
	auto temp = target;
	while (temp->getChildrenCount() != 0){
		auto child = static_cast<Card*>(temp->getChildren().at(0));
		child->setX(temp->getX());
		child->setY(temp->getY() + 1);
		temp = child;
	}
	tails.at(tail)=static_cast<Sprite*>( temp);
	
	//翻开原来位置的下一张牌
	if (isRecorde){
	if (tails.at(orign) != slots.at(orign)){
		if (!static_cast<Card*>(tails.at(orign))->getIsFront()){
			static_cast<Card*>(tails.at(orign))->change();
			change.push_back(true);
		}
		else{
			change.push_back(false);
		}
	}
	else{
		change.push_back(false);
	}
	}
	Vec2 last(target->getX(), target->getY());

	if (isRecorde)
		this->records.push_back(std::pair<Vec2,Vec2>(or, last));
	isChange = true;
	return;
}

void GameScene::moveToWin(Card* tar){

	//经完成队列移动到完成槽
	tails.at(tar->getX())=static_cast<Sprite*>(tar->getParent());

	tar->retain();
	tar->removeFromParentAndCleanup(false);
	this->finishSlots.at(finishNum++)->addChild(tar);
	tar->release();

	//修改锚点使其在同一位置
	while (tar->getChildrenCount() != 0){
		tar->getChildren().at(0)->setAnchorPoint(Vec2::ZERO);
		tar = static_cast<Card*>(tar->getChildren().at(0));
	}

	bool ch = false;
	//如果翻牌
	if (tails.at(tar->getX()) != slots.at(tar->getX())){
		if (!static_cast<Card*>(tails.at(tar->getX()))->getIsFront()){
			static_cast<Card*>(tails.at(tar->getX()))->change();
			ch = true;
		}
		else{
			ch = false;
		}
	}
	else{
		ch = false;
	}

	change.push_back(ch);
	records.push_back(std::pair<Vec2, Vec2>(Vec2(-1, 1), Vec2(-1, 1)));

	isChange = true;
}

Scene* GameScene::createScene(){

	auto scene = Scene::create();
	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}


bool GameScene::init()
{
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	UserDefault::getInstance()->setIntegerForKey("test", 10);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pukeImage.plist");

	SimpleAudioEngine::getInstance()->preloadEffect("click.mp3");
	//添加背景图
	Sprite* _bg = Sprite::create("bk.png");
	_bg->setAnchorPoint(Vec2::ZERO);
	_bg->setOpacity(180);
	this->addChild(_bg,-1);

	//按钮层
	auto layer = Layer::create();
	Sprite* _bar = Sprite::create("bar.png");
	_bar->setAnchorPoint(Vec2(0, 1));
	_bar->setPosition(Vec2(0, visibleSize.height));

	layer->addChild(_bar, 0);

	auto _ret = MenuItemImage::create("canNormal.png", "canClicked.png", [](Ref* pSender){Director::getInstance()->popScene(); });
	auto _pro = MenuItemImage::create("proNormal.png", "proClicked.png", CC_CALLBACK_1(GameScene::prompt, this));
	auto _can = MenuItemImage::create("cheNormal.png", "cheClicked.png", CC_CALLBACK_1(GameScene::cancel, this));

	auto menu = Menu::create(_ret, _pro, _can, NULL);
	menu->setAnchorPoint(Vec2(0,1));
	menu->setPosition(Vec2(0, 595));
	_ret->setPosition(Vec2(50, 0));
	_ret->setAnchorPoint(Vec2(0,0));
	_pro->setPosition(Vec2(750, 0));
	_pro->setAnchorPoint(Vec2(0,0));
	_can->setPosition(Vec2(850, 0));
	_can->setAnchorPoint(Vec2(0,0));

	layer->addChild(menu,1);

	layer->setAnchorPoint(Vec2(0, 0));
	layer->setPosition(Vec2(0, 0));
	this->addChild(layer,1);


	gInfo = GameData::getInstance();
	log("%d  %d", gInfo->getHard(), gInfo->getScores());
	//计算每张牌的大小
	int each = 20;
	int boarder = 50;
	int eachcard =( visibleSize.width - (2 * boarder) - (9 * each)) / 10;

	//放置十个卡槽
	for (int i = 0; i < 11; i++){
	//	const auto p =Sprite::create("k.jpg");
		const auto p = Sprite::createWithSpriteFrameName("k.jpg");
		p->setPosition(Vec2(boarder + eachcard*i + i*each, visibleSize.height-80));
		p->setAnchorPoint(Vec2(0,1));
		p->setScale(0.7);
		this->addChild(p,1);
		this->slots.push_back(p);
		this->tails.push_back(p);
	}
	this->slots.at(10)->setVisible(false);;

	//放置5个发牌卡槽
	for (int i = 0; i < 5; i++){
		//const auto p = Sprite::create("bk2.png");
		const auto p = Sprite::createWithSpriteFrameName("bk2.png");
		p->setScale(0.7);
		p->setPosition(Vec2(slots.at(9)->getPosition().x-i*10, 100));
		this->dealCards.push_back(p);
		this->addChild(p);
	}

	//放置8个完成卡槽
	for (int i = 0; i < 8; i++){
		const auto p = Node::create();
		p->setScale(0.7);
		p->setPosition(Vec2(slots.at(1)->getPosition().x+i*10, 100));
		this->finishSlots.push_back(p);
		this->addChild(p);
	}



	//注册发牌事件监听
	auto dealListener = EventListenerTouchOneByOne::create();
	dealListener->setSwallowTouches(true);
	dealListener->onTouchBegan = [&](Touch* touch, Event* event){

		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Vec2 location = target->convertToNodeSpace(touch->getLocation());

		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(location)){
			//卡槽全不为空
			bool emp = false;
			for (int i = 0; i < 10; i++){
				if (tails.at(i) == slots.at(i)){
					emp = true;
					break;
				}
			}
			if (!emp){

				//是否在区域内
				if (this->resNum >= 0){

					//获取一列牌
					auto dealQueue = this->dealCards.at(resNum--);

					for (int i = 0; i < 10; i++){
						auto ptr = dealQueue->getChildren().back();
						static_cast<Card*>(ptr)->setX(i);
						static_cast<Card*>(ptr)->setY(static_cast<Card*>(tails.at(i))->getY() + 1);

						Vec2 v = tails.at(i)->convertToNodeSpace(dealQueue->getPosition());

						ptr->retain();
						ptr->removeFromParentAndCleanup(false);
						tails.at(i)->addChild(ptr);
						ptr->setPosition(v);
						ptr->setAnchorPoint(Vec2(0, 0.2));
						ptr->release();

					//移动到牌尾
						auto que = Sequence::create(DelayTime::create(0.2f*i),MoveTo::create(0.5, Vec2::ZERO),DelayTime::create(0.1f),CCCallFunc::create(ptr,callfunc_selector(Card::change)),nullptr);
						ptr->runAction(que);
						SimpleAudioEngine::getInstance()->playEffect("click.mp3");
						tails.at(i) = static_cast<Card*>(ptr);
					//	(static_cast<Card*>(ptr))->change();
						isChange = true;

					}
					dealQueue->setVisible(false);
					this->records.push_back(std::pair<Vec2, Vec2>(Vec2(-1, -1), Vec2(-1, -1)));
				}
			}
		}
		return true;
	};

	for (int i = 0; i < 5; i++){
		//注册发牌事件
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(dealListener->clone(), this->dealCards.at(i));
	}

	//注册触摸事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event){

		auto target = static_cast<Card*>(event->getCurrentTarget());
		Vec2 location = target->convertToNodeSpace(touch->getLocation());


		Size s = target->getContentSize();
		Rect rect = Rect(0,0, s.width, s.height);

		if (rect.containsPoint(location)){
			//可以移动		
			SimpleAudioEngine::getInstance()->playEffect("click.mp3");
			if (canBeMoved(target)){
				//用于顶置和透明
				setZorder(target, 2000);
				setOp(target, 200);
				return true;
			}
		}
		return false;
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event){
		//跟随鼠标移动
		auto target = static_cast<Card*>(event->getCurrentTarget());
		target->setPosition(target->getPosition() + touch->getDelta()/0.7);
	};

	listener->onTouchEnded = [&](Touch* touch, Event* event){
		SimpleAudioEngine::getInstance()->playEffect("click.mp3");
		auto target = static_cast<Card*>(event->getCurrentTarget());
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());

		bool isOk = false;
		for (int i = 1; i <11; i++){
			if (pos.x > slots.at(i - 1)->getPosition().x&&pos.x <slots.at(i)->getPosition().x){
				//找到结点，移动
				if (tails.at(i-1) == slots.at(i-1) || static_cast<Card*>(tails.at(i-1))->getNum() == target->getNum() + 1){
					moveTail(target, i-1,true);
					isOk = true;
				}
				//判断i-1列是否完成
				Card* p;
				if ((p=judge(i - 1))!=NULL){
					moveToWin(p);
					if (finishNum > 8)
						log("You Win!!");
				}
				break;
			}
		}
		if (!isOk) target->runAction(MoveTo::create(0.2,Vec2::ZERO));
		
		//恢复顶置，透明状态
		reserveZorder(target);
		setOp(target, 255);
	};

	//读入所有牌组
	Vector<Card*> cards;
	int start = 3;
	char str[100] = { 0 };
	for (int i = 0; i <4 ; i++){
		for (int j = 0; j < 13; j++){
			//将int转化为string
			
			sprintf(str,"%d.jpg", start++);
			auto cc = Card::create(str,(j + 2) % 13,(Suit)i, false);
			auto cc2 = Card::create(str, (j + 2) % 13, (Suit)i, false);

			cards.pushBack(cc);
			cards.pushBack(cc2);
		}
	}


	//洗牌
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
	isChange = true;

	//发牌
	int index = 0;
	int count = 0;
	int dindex = 0;
	for (auto card : cards){

		if (count < 54){
			if (count < 10){
				this->tails.at(index)->addChild(card);
				card->setX(index);
				card->setY(0);
				card->setAnchorPoint(Vec2(0, 0));
			//	card->setPosition(Vec2(0, card->getContentSize().height*0.2));
			}
			else{
				this->tails.at(index)->addChild(card);
				card->setX(index);
				card->setY(static_cast<Card*>(card->getParent())->getY() + 1);
				card->setAnchorPoint(Vec2(0,.2));
			}
			//更新尾位置
			this->tails.at(index) = card;
			//注册卡牌事件
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), card);
			//翻牌最后行的牌
			if (count > 43)
				card->change();
			index = (index + 1) % 10;
		}
		else{
			//剩余放入发牌槽
				card->setAnchorPoint(Vec2(0,0));
				this->dealCards.at(dindex)->addChild(card);
				Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), card);
				dindex = (dindex + 1) % 5;
		}
		count++;
	}
	return true;
}
//撤销
void GameScene::cancel(Ref* psender){
	/*
	三种移动方式
	移动队列    计算下移动前后的位置，并记录是否翻开下一张
	发牌    位置为（-1，-1) 
	赢牌    位置为（1，-1）,并记录是否翻开下一张
	由于撤销
	*/
	if (!records.empty()){

		auto record = this->records.back();
		this->records.pop_back();
		int x = record.second.x;
		int y = record.second.y;
		if (x < 0 && y < 0){
			//取消发牌
			cancelDeal();
			log("cancelDeal");
		}
		else if(x>=0&&y>=0){
			//取消移动
			auto temp = slots.at(x)->getChildren().at(0);
			while (y){
				temp = temp->getChildren().at(0);
				y--;
			}
			moveTail(static_cast<Card*>(temp), record.first.x, false);
			log("cancelMove");
		}
		else{
			//取消赢牌
			cancelWind();
			log("cancelWind");
		}
		isChange = true;
	}
}
//提示
void GameScene::prompt(Ref* psender){

	//如果有更改 清空队列重新计算提示队列
	if (isChange){

		promptQueue.clear();
		//选择最长连续序列，并在剩余的九列寻找匹配位置
		for (int i = 0; i < 10; i++){
			auto ptr = static_cast<Card*>(tails.at(i));
			while (ptr->getParent() != slots.at(i) && static_cast<Card*>(ptr->getParent())->getIsFront() && static_cast<Card*>(ptr->getParent())->getNum() == ptr->getNum() + 1 && static_cast<Card*>(ptr->getParent())->getSuit()%2 == ptr->getSuit()%2){
				ptr = static_cast<Card*>(ptr->getParent());
			}
			for (int j = 0; j < 10; j++){
				if (j != i){
					//并没有考虑空列的情况 有待改进
					if ((ptr->getNum() == static_cast<Card*>(tails.at(j))->getNum() - 1)){
						promptQueue.push_back(std::pair<Card*, Card*>(ptr, static_cast<Card*>(tails.at(j))));
					}
				}
			}
		}
		isChange = false;
	}

	//提示一次弹出一个
	if (!promptQueue.empty()){
		//提示闪烁
		auto item = promptQueue.back();
		auto blin = Blink::create(0.5, 2);
		item.first->runAction(blin);
		item.second->runAction(blin->clone());
		promptQueue.pop_back();
	}
	else{
		if (resNum >= 0){
			auto blin = Blink::create(0.5, 2);
			dealCards.at(resNum)->runAction(blin);
		}
	}
	log("prompt");
}
//取消发牌
void GameScene::cancelDeal(){
	if (resNum < 4){
		resNum++;
		//遍历tails移动回代发牌槽中
		for (int i = 0; i < 10; i++){
			auto ptr = tails.at(i);
			tails.at(i) = static_cast<Sprite*>(tails.at(i)->getParent());

			Vec2 del = ptr->convertToNodeSpace(this->dealCards.at(resNum)->getPosition());
			static_cast<Card*>(ptr)->change();

			ptr->retain();
			ptr->removeFromParentAndCleanup(false);
			dealCards.at(resNum)->addChild(ptr);
			ptr->release();
			ptr->setPosition(-del);
			ptr->setAnchorPoint(Vec2::ZERO);
			ptr->runAction(MoveTo::create(0.5, Vec2::ZERO));

		}
		//重新设置为可见
		this->dealCards.at(resNum)->setVisible(true);
		isChange = true;
	}

}

//取消赢牌操作
void GameScene::cancelWind(){
	//获取最近赢牌队列
	auto tar = static_cast<Card*>(finishSlots.at(--finishNum)->getChildren().at(0));
	bool ch = change.back();
	change.pop_back();

	//如果有翻牌
	if (ch)
		static_cast<Card*>(tails.at(tar->getX()))->change();

	//移动回原来位置
	tar->retain();
	tar->removeFromParentAndCleanup(false);
	tails.at(tar->getX())->addChild(tar);
	tar -> release();

	//重新设置锚点
	while (tar->getChildrenCount() != 0){
		tar->getChildren().at(0)->setAnchorPoint(Vec2(0,.2));
		tar = static_cast<Card*>(tar->getChildren().at(0));
	}

	//更新tails
	tails.at(tar->getX()) = tar;

	isChange = true;
}
void GameScene::dealCard(int hard){
	if (hard == 0){

	}
	else if (hard == 1){

	}
	else{

	}
}