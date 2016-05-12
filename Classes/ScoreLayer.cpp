#include"ScoreLayer.h"
#include"Matrix.h"
#include"GameData.h"
#include"Audio.h"
#include"MainScene.h"

bool ScoreLayer::init(){
	if (!Layer::init())
		return false;

	auto GUI = CSLoader::createNode("ScoreLayer.csb");
	this->addChild(GUI, -1);

	_state = static_cast<Text*>(GUI->getChildByName("Text_State"));
	_score = static_cast<Text*>(GUI->getChildByName("Text_Score"));
	_time = static_cast<Text*>(GUI->getChildByName("Text_Time"));

	static_cast<Button*>(GUI->getChildByName("Button_New"))->addTouchEventListener(CC_CALLBACK_2(ScoreLayer::Restart,this));
	static_cast<Button*>(GUI->getChildByName("Button_Leave"))->addTouchEventListener(CC_CALLBACK_2(ScoreLayer::Leave,this));

}

void ScoreLayer::Restart(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type == Widget::TouchEventType::ENDED){
		auto pr = this->getParent();
		pr->removeAllChildren();
		pr->addChild(Matrix::create());
	}
}

void ScoreLayer::Leave(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type == Widget::TouchEventType::ENDED){
		Director::getInstance()->replaceScene(MainScene::createScene());
	}
}