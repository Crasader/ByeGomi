#include "TitleScene.h"
#include "SelectScene.h"
#include "GameLayer.h"
#include "AudioEngine.h"
using namespace cocos2d;
// 概要：初期化
bool TitleScene::init()
{
	if (!Layer::init())
		return false;
	// BGM再生
	experimental::AudioEngine::play2d("BGM/Title.mp3",true);
	// ゲーム
	GameLayer* game = GameLayer::create();
	this->addChild(game);
	// Sprite
	setSprite();
	// タッチ有効化
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	scheduleUpdate();
	return true;
}
// 概要：更新処理
void TitleScene::update(float delta)
{
	if (m_touch)
		m_time++;
}
// 概要：Sprite
void TitleScene::setSprite()
{
	Sprite* tap = Sprite::create("title/tap.png");
	tap->setPosition(250.0f, 200.0f);
	this->addChild(tap);
	FadeTo* fade1 = FadeTo::create(1, 0);
	DelayTime* time = DelayTime::create(1);
	FadeTo* fade2 = FadeTo::create(1, 255);
	Sequence* seq = Sequence::create(fade1, time, fade2, nullptr);
	tap->runAction(RepeatForever::create(seq));
	Sprite* bye = Sprite::create("title/bye.png");
	bye->setPosition(160.0f, 650.0f);
	this->addChild(bye);
	Sprite* gomi = Sprite::create("title/gomi.png");
	gomi->setPosition(360.0f, 550.0f);
	this->addChild(gomi);
	Sprite* star = Sprite::create("title/star.png");
	star->setPosition(250.0f, 600.0f);
	this->addChild(star);
	RotateBy* rota = RotateBy::create(1, 90);
	star->runAction(RepeatForever::create(rota));
}
// 概要：タッチ開始処理
bool TitleScene::onTouchBegan(Touch* touch, Event* event)
{
	if (m_touch)return false;
	m_time = 0;
	m_touch = true;
	return true;
}
// 概要：タッチ終了処理
void TitleScene::onTouchEnded(Touch* touch, Event* event)
{
	m_touch = false;
	// ５秒以上タッチしてたらreturn
	if (m_time > (1 * 60))return;
	experimental::AudioEngine::stopAll();
	experimental::AudioEngine::play2d("title/button.mp3");
	auto scene = SelectScene::createScene();
	TransitionFade* fade = TransitionFade::create(0.5f, scene, Color3B::WHITE);	
	Director::getInstance()->replaceScene(fade);
}
// 概要：シーン生成
Scene * TitleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene->addChild(layer);
	return scene;
}
