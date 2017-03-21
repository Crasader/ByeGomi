#include "ResultDialog.h"
#include "ui\CocosGUI.h"
#include "SelectScene.h"
#include "PlayScene.h"
#include "AudioEngine.h"
using namespace cocos2d;
// 概要：初期化
bool ResultDialog::init(bool result)
{
	if (!Node::init())
		return false;
	// 全ての音を停止
	experimental::AudioEngine::stopAll();
	// 他のレイヤーを暗くする
	LayerColor* layerColor = LayerColor::create();
	layerColor->initWithColor(Color4B(0x00, 0x00, 0x00, 0xff));
	layerColor->setOpacity(100);
	this->addChild(layerColor);
	// 他のレイヤーのタッチを阻害
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch *touch, Event*event) {return true; };
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layerColor);
	// ダイアログ画像
	Vec2 size = Director::getInstance()->getVisibleSize()/2;
	Sprite* spr = Sprite::create();
	spr->setPosition((Vec2(size.x, size.y)));
	this->addChild(spr);
	if (result)
	{
		experimental::AudioEngine::play2d("BGM/NicePopup1.mp3", true);
		experimental::AudioEngine::play2d("BGM/NicePopup2.mp3");
		spr->setTexture("result/nice.png");
	}
	else
	{
		experimental::AudioEngine::play2d("BGM/SorryPopup.mp3", true);
		spr->setTexture("result/sorry.png");
	}
	// 再開ボタン
	ui::Button* replay = ui::Button::create("result/replay.png");
	replay->setPosition(Vec2(115, 70));
	replay->addTouchEventListener([&](Ref* pRef, ui::Widget::TouchEventType type)
	{
		if (type != ui::Widget::TouchEventType::ENDED)return;
		experimental::AudioEngine::stopAll();
		experimental::AudioEngine::play2d("button.mp3");
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(PlayScene::createScene());
	});
	spr->addChild(replay);
	// 戻りボタン
	ui::Button* back = ui::Button::create("result/back.png");
	back->setPosition(Vec2(285, 70));
	back->addTouchEventListener([&](Ref* pRef, ui::Widget::TouchEventType type)
	{
		if (type != ui::Widget::TouchEventType::ENDED)return;
		experimental::AudioEngine::stopAll();
		experimental::AudioEngine::play2d("button.mp3");
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(SelectScene::createScene());
	});
	spr->addChild(back);
	return true;
}
// 概要：ResultDialog生成
ResultDialog* ResultDialog::create(bool result)
{
	ResultDialog* pRet = new(std::nothrow)ResultDialog();
	if (pRet && pRet->init(result))
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