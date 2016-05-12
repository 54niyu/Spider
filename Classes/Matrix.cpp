#include"Matrix.h"
#include"Card.h"
#include"GameData.h"
#include"SimpleAudioEngine.h"
#include"Audio.h"
#include"MainScene.h"
#include"ScoreLayer.h"
using namespace CocosDenshion;
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;


void loadCards(vector<Card*>& cards, int hard){
	static string post[4] = { "_of_spades.png", "_of_hearts.png", "_of_clubs.png", "_of_diamonds.png" };
	int repeat = 0;
	int suit = 0;
	if (hard == 0){
		//单花色
		repeat = 4; suit = 1;
	}
	else if (hard == 1){
		//双花色
		repeat = 2; suit = 2;
	}
	else{
		//四花色
		repeat = 1; suit = 4;
	}

	char file[100] = "";
	for (int i = 0; i < repeat; i++){
		for (int j = 0; j < suit; j++){
			for (int k = 1; k <= 13; k++){
				sprintf(file, "%d", k);
				string temp(file + post[j]);
				//	auto c = Card::create(file, (k + 2) % 13, (Suit)j, false);
				//	auto cc=Card::create(file, (k + 2) % 13, (Suit)j, false); 
				cards.push_back(Card::create(temp, k, (Suit)j, false));
				//	cards.push_back(c);
				//	cards.push_back(cc);
				cards.back()->retain();
				cards.push_back(Card::create(temp, k, (Suit)j, false));
				cards.back()->retain();
			}
		}
	}
}
void shuffleCards(vector<Card*>& cards){

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));

}
void Matrix::dealCards(int num, bool needChange, bool allowEmpty){
	if (!allowEmpty){
		for (int i = 0; i < num; i++){
			if (_matrix.at(i).empty())
				return;
		}
	}
	for (int i = 0; i < num; i++){
		Card* temp = _res.back();
		this->addChild(_res.back());
		_res.back()->setAnchorPoint(Vec2(0.5, 1));
		_res.back()->setY(i);
		_res.back()->setScale(0.7);
		_res.back()->setX(_matrix.at(i).size());
		_res.back()->setZOrder(_res.back()->getX());
		_res.back()->setPosition(deal->getPosition());
		_matrix.at(i).push_back(_res.back());
		auto mt = MoveTo::create(0.1/*0.2 / (i + 1)*/, Vec2((_res.back()->getY() + 1) * 87, 580 - _res.back()->getX() * 20));

		if (needChange){
			auto sq = Sequence::create(DelayTime::create(i*0.05), mt, CCCallFunc::create([temp](){	temp->change(); }), CCCallFunc::create([&,i](){this->isFinish(i); }), NULL);
			_res.back()->runAction(sq);
		}
		else{
			auto sq = Sequence::create(DelayTime::create(i*0.05), mt, NULL);
			_res.back()->runAction(sq);
		}
		_res.pop_back();
	}
}
void Matrix::cancelDeal(){
	for (int i = 9; i >= 0; i--){
		_res.push_back(_matrix[i].back());
		auto c = _res.back();
		_matrix[i].pop_back();
		auto sq = Sequence::create(CCCallFunc::create([c](){c->change(); }), DelayTime::create(0.05*(10 - i)), MoveTo::create(0.1/*0.05*(10 - i)*/, deal->getPosition()), CCCallFunc::create([c](){c->removeFromParentAndCleanup(false); }), NULL);
		_res.back()->runAction(sq);
	}
}

Scene* Matrix::createScene(){

	auto scene = Scene::create();
	auto layer = Matrix::create();
	scene->addChild(layer, 0);
	return scene;
}
bool Matrix::init(){

	if (!Layer::init()){
		return false;
	}

	auto GUI = CSLoader::createNode("MatrixLayer.csb");
	this->addChild(GUI, -1);

	time = static_cast<Text*>(GUI->getChildByName("Text_Time"));
	scores = static_cast<Text*>(GUI->getChildByName("Text_Scores"));

	deal = static_cast<Button*>(GUI->getChildByName("Button_Deal"));
	deal->addTouchEventListener(CC_CALLBACK_2(Matrix::dealEvent, this));

	promot = static_cast<Button*>(GUI->getChildByName("Button_Promot"));
	promot->addTouchEventListener(CC_CALLBACK_2(Matrix::promotEvent, this));

	leave = static_cast<Button*>(GUI->getChildByName("Button_Leave"));
	leave->addTouchEventListener(CC_CALLBACK_2(Matrix::leaveEvent, this));

	cancel = static_cast<Button*>(GUI->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener(CC_CALLBACK_2(Matrix::cancelEvent, this));

	back = static_cast<Button*>(GUI->getChildByName("Button_Return"));
	back->addTouchEventListener(CC_CALLBACK_2(Matrix::backEvent, this));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("poker.plist");

	GameData* gd = GameData::getInstance();

	this->scheduleUpdate();

	for (int i = 0; i < 10; i++){
		vector<Card*> temp;
		_matrix.push_back(temp);
	}

	//加载牌
	loadCards(_res, gd->getHard());
	//洗牌
	shuffleCards(_res);
	shuffleCards(_res);
	//发牌
	for (int i = 0; i < 5; i++){
		dealCards(10, false, true);
	}

	dealCards(4, false, true);
	for (int i = 0; i < 10; i++){
		_matrix.at(i).back()->change();
	}

	this->setTouchEnabled(true);
	return true;
}

void Matrix::update(float delta){
	static float all = 0;
	all += delta;
	if (all > 1){
		_time += 1;
		time->setText(String::createWithFormat("%2d:%2d", _time / 60, _time % 60)->getCString());
		all = 0;
	}
}
void Matrix::onEnter(){
	Layer::onEnter();
	SimpleAudioEngine::getInstance()->preloadEffect("click.mp3");
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Matrix::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Matrix::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Matrix::onTouchEnded, this);
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}
void Matrix::onExit(){
	Layer::onExit();
}

bool Matrix::isValidTouch(int& x, int y, Vec2 location){

	if (y >= 0 && y < 10){
		if (_matrix.at(y).size() == 0){
			Vec2 p((y + 1) * 87, 580);
			Rect r(p.x - 52.5*0.7, p.y - 150 * 0.7, 105 * .7, 150 * .7);
			if (r.containsPoint(location)){
				return true;
			}
			else{
				return false;
			}
		}
		if (x < _matrix.at(y).size()){
			Vec2 p = _matrix.at(y).at(x)->getPosition();
			Rect r(p.x - 52.5*0.7, p.y - 150 * 0.7, 105 * .7, 150 * .7);
			if (r.containsPoint(location)){
				if (_matrix[y][x]->getIsFront())
					return true;
			}
		}
		else{
			if (!_matrix.at(y).empty()){
				Vec2 p = _matrix.at(y).back()->getPosition();
				Rect r(p.x - 52.5*0.7, p.y - 150 * 0.7, 105 * .7, 150 * .7);
				if (r.containsPoint(location)){
					x = _matrix.at(y).size() - 1;
					if (_matrix[y][x]->getIsFront())
						return true;
				}
			}
		}
	}
	return false;
}
bool Matrix::isValidMotion(int row, int col){

	for (int i = row + 1; i < _matrix.at(col).size(); i++){
		if (_matrix[col][i]->getSuit() == _matrix[col][i - 1]->getSuit() && _matrix[col][i]->getNum() == _matrix[col][i - 1]->getNum() - 1){
			continue;
		}
		else{
			return false;
		}
	}
	return true;
}
bool Matrix::isFinish(int col){
	int count = 1;
	for (int i = _matrix.at(col).size() - 2; i >= 0; i--){
		if (_matrix.at(col).at(i)->getIsFront() && _matrix.at(col).at(i)->getSuit() == _matrix.at(col).at(i + 1)->getSuit() && _matrix.at(col).at(i)->getNum() == _matrix.at(col).at(i + 1)->getNum() + 1){
			count++;
		}
		else{
			break;
		}
	}
	if (count == 13){
		_finish.push_back(vector<Card*>());
		for (int i = 0; i < 13; i++){
			Card* temp = _matrix[col].back();
			_finish.back().push_back(temp);
			_matrix[col].back()->runAction(Sequence::create(DelayTime::create(0.05*i), MoveTo::create(0.2f, Vec2(100 + 20 * _finish.size(), 118)),  NULL));
			_matrix[col].pop_back();
		}
		_score += 100;
		scores->setText(String::createWithFormat("%3d", _score)->getCString());
		isWin();
		return true;
	}
	return false;
}
bool Matrix::isWin(){
	if (_finish.size() == 8){
		ScoreLayer *sc = ScoreLayer::create();
		sc->_score->setText("得分："+scores->getString());
		sc->_time->setText("用时："+time->getString());
		this->getParent()->addChild(sc, 10);
	}
	return true;
}
void Matrix::setOp(int row, int col, int op, int z){
	for (int i = row; i < _matrix[col].size(); i++){
		if (_matrix[col][i]->getChildrenCount() == 0){
			auto shadow = Sprite::create("card_shadow.png");
			shadow->setPosition(Vec2(30, 50));
			shadow->setGlobalZOrder(z - 1);
			_matrix[col][i]->addChild(shadow);
		}
		else{
			_matrix[col][i]->removeAllChildren();
		}
		//	_matrix[col][i]->setOpacity(op);
		_matrix[col][i]->setGlobalZOrder(z);
	}
}

bool Matrix::onTouchBegan(Touch *touch, Event *unused_event){
	//获取当前的位置，并将其转换为对应的x，y行列信息。
	Vec2 location = touch->getLocation();
	int row = (580 - location.y) / 20;
	int col = location.x / 87 - 0.5;

	if (isValidTouch(row, col, location)){
		//合法的点击位置
		Audio::getInstance()->play(1);
		SimpleAudioEngine::getInstance()->playEffect("SpecSelectCard.ogg");
		if (isValidMotion(row, col)){
			//点击的牌可以移动
			setOp(row, col, 180, 100);
			//设置选中效果。
			Selected = _matrix.at(col).at(row);
			return true;
		}
	}
	return false;
}
void Matrix::onTouchMoved(Touch *touch, Event *unused_event){
	if (Selected != nullptr){
		for (int i = Selected->getX(); i < _matrix.at(Selected->getY()).size(); i++){
			//根据每次位移的信息，不停的改变选择牌组的位置，跟随触摸点移动。
			_matrix[Selected->getY()][i]->setPosition(_matrix[Selected->getY()][i]->getPosition() + touch->getDelta());
			
		}
	}
}
void Matrix::moveFromTo(int row1, int col1, int row2, int col2, bool isSave){
	//SimpleAudioEngine::getInstance()->playEffect("click.mp3");
	for (int i = row1; i < _matrix.at(col1).size(); i++){
		_matrix.at(col2).push_back(_matrix[col1][i]);
		_matrix.at(col2).back()->setX(_matrix.at(col2).size() - 1);
		_matrix.at(col2).back()->setY(col2);
		auto ac = MoveTo::create(0.2f, Vec2((col2 + 1) * 87, 580 - (_matrix.at(col2).back()->getX()) * 20));
		Audio::getInstance()->play(1);
		//	SimpleAudioEngine::getInstance()->playEffect("click.mp3");
		_matrix.at(col2).back()->runAction(ac);
		_matrix.at(col2).back()->setZOrder(_matrix.at(col2).back()->getX());
	}
	_matrix.at(col1).erase(_matrix.at(col1).begin() + row1, _matrix.at(col1).end());
	isChange = true;

}
void Matrix::onTouchEnded(Touch *touch, Event *unused_event)
{
	Vec2 location = touch->getLocation();
	int row = (580 - location.y) / 20;
	int col = location.x / 87 - 0.5;
	//转换位置到x，y

	bool isMoved = false;
	bool isSet = false;

	if (isValidTouch(row, col, location)){
		if (_matrix[col].empty() || _matrix[col].back()->getNum() == Selected->getNum() + 1){
			//移动动作
			int pre = Selected->getY();
			int pre2 = Selected->getX();

			//纪录操作
			Record rc; rc.type = Move;
			rc.data = new int(4);
			rc.data[0] = Selected->getX(); rc.data[1] = Selected->getY();
			//移动牌组，更新信息。
			moveFromTo(Selected->getX(), Selected->getY(), row, col, false);
			rc.data[2] = Selected->getX(); rc.data[3] = Selected->getY();

			if (!_matrix[pre].empty() && !_matrix[pre].back()->getIsFront()){
				_matrix[pre].back()->change();
				rc.isTurn = true;
			}
			_historyRecord.push(rc);//添加历史纪录，用于回退

			setOp(Selected->getX(), Selected->getY(), 250, 0);
			isSet = true;

			//完成动作
			if (_matrix[col].back()->getNum() == 1){
				if (isFinish(col)){
					Record rc2;
					rc2.type = Finish;
					rc2.data = new int(1);
					rc2.data[0] = col;

					if (!_matrix[col].empty() && !_matrix[col].back()->getIsFront()){
						_matrix[col].back()->change();
						rc2.isTurn = true;
					}
					_historyRecord.push(rc2);
				}
			}

			_score--;
			scores->setText(String::createWithFormat("%3d", _score)->getCString());
			isMoved = true;
		}
	}

	if (!isMoved){
		//非法移动，去除选中效果，返回原来的位置
		int j = Selected->getY();
		for (int i = Selected->getX(); i < _matrix[j].size(); i++){
			_matrix[j][i]->setPosition(j * 87 + 87, 580 - i * 20);
		}
	}

	if (!isSet)
		setOp(Selected->getX(), Selected->getY(), 250, 0);
	isWin();
	Audio::getInstance()->play(3);
}

void Matrix::promotEvent(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type == Widget::TouchEventType::ENDED){
		if (isChange){
			while (!_promot.empty()){
				_promot.pop();
			}
			//计算提示队列
			for (int i = 0; i < 10; i++){
				int j = _matrix[i].size() - 1;
				if (!_matrix[i].empty()){
					//从末尾向上寻找连续的最长序列
					while (j - 1 >= 0 && _matrix[i][j - 1]->getIsFront() && _matrix[i][j]->getSuit() == _matrix[i][j - 1]->getSuit() && _matrix[i][j]->getNum() + 1 == _matrix[i][j - 1]->getNum()){
						j--;
					}
					//在剩余的列当中测试是否可以连成一列，可以加入_promot队列中
					for (int k = 0; k < 10; k++){
						if (!_matrix[k].empty()){
							if (_matrix[k].back()->getNum() == _matrix[i][j]->getNum() + 1){
								_promot.push(std::pair<Card*, Card*>(_matrix[k].back(), _matrix[i][j]));
							}
						}
					}
				}
			}
			//提示发牌
			if (!_res.empty())
				_promot.push(std::pair<Card*, Card*>(nullptr, nullptr));
			isChange = false;
		}
		if (!_promot.empty()){
			//闪烁的方式显示提示
			auto temp = _promot.front();
			_promot.push(_promot.front());
			_promot.pop();

			auto bl = Blink::create(1, 2);

			if (temp.first != nullptr){
				temp.first->setVisible(true);
				temp.second->setVisible(true);
				temp.first->runAction(bl);
				temp.second->runAction(bl->clone());
			}
			else{
				deal->setVisible(true);
				deal->runAction(bl);
			}
		}
		else {
			if (_res.empty());
				//you lose
		}
	}
}
void Matrix::cancelEvent(Ref *pSender, Widget::TouchEventType type){
	if (_historyRecord.empty())	return;
	switch (type){
	case Widget::TouchEventType::BEGAN:
		Audio::getInstance()->play(0);
		break;
	case Widget::TouchEventType::ENDED:
	{
		_score--;
		scores->setText(String::createWithFormat("%3d", _score)->getCString());
		isChange = true;
		switch (_historyRecord.top().type){
		case Move:{
			if (_historyRecord.top().isTurn){
				_matrix.at(_historyRecord.top().data[1]).back()->change();
			}
			moveFromTo(_historyRecord.top().data[2], _historyRecord.top().data[3], _historyRecord.top().data[0], _historyRecord.top().data[1], false);
			_historyRecord.pop();
		}; break;
		case Deal:{
			cancelDeal();
			deal->setVisible(true);
			_historyRecord.pop();
		}; break;
		case Finish:{
			int col = _historyRecord.top().data[0];
			if (_historyRecord.top().isTurn)
					_matrix[col].back()->change();

				for (int i = 12; i >= 0; i--){
					auto ac = MoveTo::create(0.2f, Vec2((_finish.back().at(i)->getY() + 1) * 87, 580 - _finish.back().at(i)->getX() * 20));
					_finish.back().at(i)->runAction(ac);
					_matrix.at(col).push_back(_finish.back().at(i));
				}
				_finish.pop_back();
				_historyRecord.pop();
				cancelEvent(pSender,Widget::TouchEventType::ENDED);
		}; break;
		}
	}
	break;
	}
}
void Matrix::backEvent(Ref *pSender, Widget::TouchEventType type){

	switch (type){
	case Widget::TouchEventType::BEGAN:
		Audio::getInstance()->play(0);
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:{
		auto scene = MainScene::createScene();
		scene->setTag(10);
		Director::getInstance()->pushScene(scene);
	}
					   break;

	case Widget::TouchEventType::CANCELED:
		break;
	}

}
void Matrix::dealEvent(Ref *pSender, Widget::TouchEventType type){
	switch (type){
	case Widget::TouchEventType::BEGAN:
		Audio::getInstance()->play(0);
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:{
		if (_res.size() >= 10){
			isChange = true;
			Record rc;
			rc.type = Deal;
			dealCards(10, true, false);
			_historyRecord.push(rc);
			//_histroy.push(record(pair<int, int>(-1, -1), pair<int, int>(-1, -1)));
		}
		if (_res.empty()){
			isChange = true;
			deal->setVisible(false);
		}
	}
					   break;

	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void Matrix::leaveEvent(Ref *pSender, Widget::TouchEventType type){
	switch (type){
	case Widget::TouchEventType::BEGAN:
		Audio::getInstance()->play(0);
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:{
		auto scene = Matrix::create();
		scene->setTag(10);
		this->getParent()->addChild(scene);
		this->removeFromParentAndCleanup(true);
	}; break;

	case Widget::TouchEventType::CANCELED:
		break;
	}

}