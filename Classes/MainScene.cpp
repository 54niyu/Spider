#include"cocos2d.h"
#include"MainScene.h"
#include "Matrix.h"
#include"SettingLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"

USING_NS_CC;


Scene* MainScene::createScene(){

	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}


bool MainScene::init()
{
	if (!Layer::init()){
		return false;
	}
	auto GUI = CSLoader::createNode("MainScene.csb");
	this->addChild(GUI, -1);

	static_cast<Button*>(GUI->getChildByName("Button_Start"))->addTouchEventListener(CC_CALLBACK_2(MainScene::Start, this));
	static_cast<Button*>(GUI->getChildByName("Button_Setting"))->addTouchEventListener(CC_CALLBACK_2(MainScene::Setting, this));

	return true;
}

void MainScene::Start(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN){
		Audio::getInstance()->play(0);
	}
	else if (type == Widget::TouchEventType::ENDED){
	//	Audio::getInstance()->play(0);
		if (Director::getInstance()->getRunningScene()->getTag() == 10)
			Director::getInstance()->popScene();
		else{
			auto scene = Matrix::createScene();
			//Director::getInstance()->pushScene(scene);
			Director::getInstance()->replaceScene(scene);
		}
	}
}
void MainScene::Setting(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN){
		Audio::getInstance()->play(0);
	}
	else if (type == Widget::TouchEventType::ENDED){
		Audio::getInstance()->play(0);
		auto layer = SettingLayer::create();
		this->addChild(layer);
	}
}