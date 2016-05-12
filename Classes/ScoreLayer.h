#ifndef _SCORE_H_
#define _SCORE_H_
#include "cocos2d.h"
#include "Card.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"

using namespace cocos2d;
using namespace ui;


class ScoreLayer:public cocos2d::Layer{
public:
	virtual bool init();
	CREATE_FUNC(ScoreLayer);

	void Leave(Ref *pSender, Widget::TouchEventType type);
	void Restart(Ref *pSender, Widget::TouchEventType type);
	Text *_state, *_score, *_time;
};

#endif
