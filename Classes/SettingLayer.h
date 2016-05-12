#ifndef _SETTINGLAYER_H
#define _SETTINGLAYER_H

#include "cocos2d.h"
#include "Card.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"

using namespace cocos2d;
using namespace ui;


class SettingLayer:public cocos2d::Layer{
public:
	virtual bool init();
	CREATE_FUNC(SettingLayer);

	void Close(Ref *pSender, Widget::TouchEventType type);
	void Single(Ref *pSender, Widget::TouchEventType type);
	void Double(Ref *pSender, Widget::TouchEventType type);
	void Four(Ref *pSender, Widget::TouchEventType type);

	void SoundChange(Ref* pSender, Slider::EventType type);
	void MusicChange(Ref* pSender, Slider::EventType type);

private:
	Button* bnt[3];
	Slider* slds;
	Slider* sldm;
};
#endif