#include "PlayScene.h"
#include "GameLayer.h"
#include "MenuList.h"
#include "AudioEngine.h"
#include "ResultDialog.h"
using namespace cocos2d;
// 概要：初期化
bool PlayScene::init()
{
	if (!Layer::init())
		return false;
	// BGM
	experimental::AudioEngine::play2d("BGM/Play.mp3", true);
	Sprite* spr = Sprite::create("missionback.png");
	spr->setPosition(250, 750);
	this->addChild(spr, 10);
	// カウントダウン
	m_countDown = CountDown::create(
		60,"timer/back.png","timer/front.png");
	m_countDown->setPosition(50, 750);
	this->addChild(m_countDown,100);
	// メニュー
	MenuList* menu = MenuList::create();
	menu->setPosition(450, 750);
	this->addChild(menu, 1000);
	// ゲーム
	GameLayer* game = GameLayer::create();
	this->addChild(game);
	game->setMission();
	scheduleUpdate();
	return true;
}
// 概要：更新処理
void PlayScene::update(float delta)
{
	// ゲームオーバー判定
	if (m_countDown->checkFinish())
	{
		Director::getInstance()->pause();
		ResultDialog* resultDialog = ResultDialog::create(false);
		this->addChild(resultDialog, 20000);
	}
}
// 概要：シーンの生成
cocos2d::Scene * PlayScene::createScene()
{	
	auto scene = Scene::create();
	auto layer = PlayScene::create();
	scene->addChild(layer);
	return scene;
}