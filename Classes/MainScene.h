
#include"cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"
#include"Audio.h"

using namespace cocos2d;
using namespace ui;

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();		//初始化动作

	void Start(Ref *pSender, Widget::TouchEventType type);	//开始游戏
	void Setting(Ref *pSender, Widget::TouchEventType type);	//设置

	CREATE_FUNC(MainScene);
};
