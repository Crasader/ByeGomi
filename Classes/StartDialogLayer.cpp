#include "StartDialogLayer.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "Stage.h"
#include "PlayScene.h"
#include "AudioEngine.h"
#pragma execution_character_set("utf-8")
using namespace cocos2d;
// 概要：初期化
bool StartDialog::init(Mission mission, int stage)
{
	if (!Node::init())
		return false;	
	m_mission = mission;
	m_stage = stage;
	LayerColor* layerColor = LayerColor::create();
	layerColor->initWithColor(Color4B(0x00, 0x00, 0x00, 0xff));
	layerColor->setOpacity(100);
	this->addChild(layerColor);
	// ダイアログ画像
	Sprite* spr = Sprite::create("select/window.png");
	Vec2 size = Director::getInstance()->getVisibleSize() / 2;
	spr->setPosition(size);
	this->addChild(spr);
	// 空のタッチイベントリスナーで下層のタッチを阻害する
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* touch, Event* event) {return true; };
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spr);
	// 戻りボタン
	ui::Button* back = ui::Button::create("select/button/back.png");
	back->setPosition((Vec2(285, 70)));
	back->addTouchEventListener([&](Ref * pRef, ui::Widget::TouchEventType type)
	{
		if (type != ui::Widget::TouchEventType::ENDED)return;
		experimental::AudioEngine::play2d("select/button.mp3");
		Director::getInstance()->resume();
		this->removeFromParent();
	});
	spr->addChild(back);
	// プレイボタン
	ui::Button* play = ui::Button::create("select/button/play.png");
	play->setPosition((Vec2(115, 70)));
	play->addTouchEventListener([&](Ref * pRef, ui::Widget::TouchEventType type)
	{
		if (type != ui::Widget::TouchEventType::ENDED)return;
		experimental::AudioEngine::stopAll();
		experimental::AudioEngine::play2d("select/button.mp3");
		Director::getInstance()->resume();
		GameManager::getInstance()->setStage(m_stage);
		GameManager::getInstance()->setMission(m_mission);
		Director::getInstance()->replaceScene(PlayScene::createScene());
	});
	spr->addChild(play);
	// ノルマ
	char s[32];
	int count = m_mission.getCount();
	std::string string = m_mission.getString();
	sprintf(s, "%sを%d個処理", string.c_str(), count);
	Label* missionLabel = Label::create(s, "Arial", 40);
	missionLabel->setPosition(spr->getBoundingBox().size / 2);
	missionLabel->setColor(Color3B::BLACK);
	spr->addChild(missionLabel);
	return true;
}
// 概要：StartDialog生成
StartDialog* StartDialog::create(Mission mission, int stage)
{
	StartDialog* pRet = new(std::nothrow)StartDialog();
	if (pRet && pRet->init(mission,stage))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}