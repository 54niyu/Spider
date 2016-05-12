#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "cocos2d.h"
#include "Card.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"

typedef std::pair<std::pair<int, int>, std::pair<int, int>> record;

using namespace cocos2d;
using namespace ui;

enum RecordType{
	Move,//move from one point to another point
	Deal,//deal cards
	Finish,//finish one col
};
struct Record{
	RecordType type;	//纪录的类型
	int* data;	
	bool isTurn;
	Record():type(Move),data(nullptr),isTurn(false){};
	~Record(){ if(!data)	delete[]data; }
};

class Matrix:public cocos2d::Layer{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Matrix);


	void dealCards(int num,bool needChange,bool allowEmpty);	//发牌 
	void cancelDeal();
	bool isValidTouch(int &x, int y, Vec2 pos);
	bool isValidMotion(int row, int col);
	bool isFinish(int col);
	bool isWin();
	void setOp(int row, int col,int op,int z);
	void moveFromTo(int row1, int col1, int row2, int col2,bool isSave);

	void onEnter();
	void onExit();

	//用来处理触摸事件
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);


	void promotEvent(Ref *pSender, Widget::TouchEventType type);	//提示
	void cancelEvent(Ref *pSender, Widget::TouchEventType type);	//撤销
	void backEvent(Ref *pSender, Widget::TouchEventType type);	//返回
	void dealEvent(Ref *pSender, Widget::TouchEventType type);	//发牌
	void leaveEvent(Ref *pSender, Widget::TouchEventType type);	//新游戏

	void update(float delta);		

private:
	std::vector<std::vector<Card*>> _matrix;		//存放十组牌
	std::vector<Card*> _res;		//代发牌数组
	std::vector<std::vector<Card*>> _finish;		//已完成数组
	std::queue<std::pair<Card*, Card*>> _promot;		//提示信息队列
	std::stack<Record> _historyRecord;		//历史纪录栈

	Card	*preSelected = nullptr,*Selected = nullptr;
	int cols = 10;
	bool idDealing = false, isChange = true;
	long  _time = 0;int _score = 500;
	Text *time, *scores;
	Button *promot, *back, *cancel, *deal, *leave;
};


#endif