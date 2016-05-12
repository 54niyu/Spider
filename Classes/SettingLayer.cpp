#include"SettingLayer.h"
#include"GameData.h"
#include"Audio.h"

bool SettingLayer::init(){

	if (!Layer::init()){
		return false;
	}

	auto GUI = CSLoader::createNode("SettingLayer.csb");
	this->addChild(GUI, -1);

	static_cast<Button*>(GUI->getChildByName("Button_Close"))->addTouchEventListener(CC_CALLBACK_2(SettingLayer::Close, this));
	bnt[0] = static_cast<Button*>(GUI->getChildByName("Button_0"));
	bnt[0]->addTouchEventListener(CC_CALLBACK_2(SettingLayer::Single, this));
	bnt[1] = static_cast<Button*>(GUI->getChildByName("Button_1"));
	bnt[1]->addTouchEventListener(CC_CALLBACK_2(SettingLayer::Double, this));
	bnt[2] = static_cast<Button*>(GUI->getChildByName("Button_2"));
	bnt[2]->addTouchEventListener(CC_CALLBACK_2(SettingLayer::Four, this));

	slds = static_cast<Slider*>(GUI->getChildByName("Slider_Sound"));
	slds->addEventListener(CC_CALLBACK_2(SettingLayer::SoundChange, this));
	log("soud  %f",SimpleAudioEngine::getInstance()->getEffectsVolume());
	slds->setPercent(SimpleAudioEngine::getInstance()->getEffectsVolume() * 100);

	sldm = static_cast<Slider*>(GUI->getChildByName("Slider_Music"));
	sldm->addEventListener(CC_CALLBACK_2(SettingLayer::MusicChange, this));
	bnt[GameData::getInstance()->getHard()]->setEnabled(false);

	return true;
}

void SettingLayer::Close(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type==Widget::TouchEventType::ENDED)
		this->removeFromParentAndCleanup(true);
}
void SettingLayer::Single(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type == Widget::TouchEventType::ENDED){
		bnt[GameData::getInstance()->getHard()]->setEnabled(true);
		GameData::getInstance()->setHard(0);
		GameData::getInstance()->saveHard();
		bnt[GameData::getInstance()->getHard()]->setEnabled(false);
	}
}
void SettingLayer::Double(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type == Widget::TouchEventType::ENDED){
		bnt[GameData::getInstance()->getHard()]->setEnabled(true);
		GameData::getInstance()->setHard(1);
		GameData::getInstance()->saveHard();
		bnt[GameData::getInstance()->getHard()]->setEnabled(false);
	}
}
void SettingLayer::Four(Ref *pSender, Widget::TouchEventType type){
	if (type == Widget::TouchEventType::BEGAN)
		Audio::getInstance()->play(0);
	else if (type == Widget::TouchEventType::ENDED){
		bnt[GameData::getInstance()->getHard()]->setEnabled(true);
		GameData::getInstance()->setHard(2);
		GameData::getInstance()->saveHard();
		bnt[GameData::getInstance()->getHard()]->setEnabled(false);
	}
}

void SettingLayer::SoundChange(Ref* pSender, Slider::EventType type){
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		int percent = slds->getPercent();
		int maxPercent = slds->getMaxPercent();
		log("%d %d", percent, maxPercent);
		//if (percent == 0)	Audio::getInstance()->setSound(false);
		Audio::getInstance()->changeSoundVol((double)percent/maxPercent);
	}
}
void SettingLayer::MusicChange(Ref* pSender, Slider::EventType type){
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		int percent = sldm->getPercent();
		Audio::getInstance()->changeMusicVol(percent);
	}
}