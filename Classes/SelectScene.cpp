#include "SelectScene.h"
#include "ui/CocosGUI.h"
#include "Stage.h"
#include "AudioEngine.h"
using namespace cocos2d;
Scene* SelectScene::createScene()
{	
	auto scene = Scene::create();
	auto layer = SelectScene::create();	
	scene->addChild(layer);
	return scene;
}
// 概要：初期化
bool SelectScene::init()
{	
	if (!Layer::init())
		return false;
	experimental::AudioEngine::play2d("BGM/Select.mp3", true);	
	Size size = Director::getInstance()->getVisibleSize()/2;
	Sprite *backGround = Sprite::create("select/background.png");
	backGround->setPosition(size);
	this->addChild(backGround);
	// ステージ
	int k = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Stage *stage = Stage::create(m_missionTable[k], k + 1);
			stage->setPosition(Vec2(120 + 120 * j, 400 - 120 * i));
			this->addChild(stage);
			k++;
		}
	}
	std::string string[2] = { "select/Stage.png","select/Select.png" };
	Vec2 pos[2] = { { 200, 700 },{ 320, 600 } };
	Vec2 scale[2] = { {0.8f,0.4f}, {0.4f,0.8f} };
	for (int i = 0; i < 2; i++)
	{
		//stageの文字
		Sprite* spr = Sprite::create(string[i]);
		spr->setScale(0.6f);
		spr->setPosition(pos[i]);
		this->addChild(spr);

		ScaleTo* scale1 = ScaleTo::create(1.f, scale[i].x);
		ScaleTo* scale2 = ScaleTo::create(1.f, scale[i].y);
		Sequence* seq = Sequence::create(scale1, scale2, nullptr);
		RepeatForever* re = RepeatForever::create(seq);
		spr->runAction(re);
	}
	// キャラクター
	m_gomi = Sprite::create("select/animation/gomi0.png");
	m_gomi->setPosition(50, 50);
	this->addChild(m_gomi, 100);
	const float time = 1.0f;
	MoveBy* right = MoveBy::create(time, Vec2(400, 0));
	MoveBy* up = MoveBy::create(time, Vec2(0, 450));
	MoveBy* left = MoveBy::create(time, Vec2(-400, 0));
	MoveBy* down = MoveBy::create(time, Vec2(0, -450));
	Sequence* seq = Sequence::create(right, up, left, down, nullptr);
	m_gomi->runAction(RepeatForever::create(seq));

	Animation *animation = Animation::create();
	for (int i = 0; i < 6; i++)
	{
		char c[64];
		sprintf(c, "select/animation/gomi%d.png", i);
		animation->addSpriteFrameWithFileName(c);
	}
	// 0.6秒間の間に 5フレーム切替表示を行う
	animation->setDelayPerUnit(0.6f / 5.0f);
	RepeatForever* action = RepeatForever::create(Animate::create(animation));
	// アニメーションを実行
	m_gomi->runAction(action);
	scheduleUpdate();
	return true;
}
// 概要：更新処理
void SelectScene::update(float delta)
{	
	//ゴミが画面周りを走る
	float x = m_gomi->getPosition().x;
	float y = m_gomi->getPosition().y;

	if (x >= 440 && y <= 60)
	{
		m_gomi->setRotation(-90);
	}
	else if (x >= 440 && y >= 400)
	{
		m_gomi->setRotation(-180);
	}
	else if (x <= 60 && y >= 390)
	{
		m_gomi->setRotation(-270);
	}
	else if (x <= 60 && y <= 60)
	{
		m_gomi->setRotation(0);
	}
}