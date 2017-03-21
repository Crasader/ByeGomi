#include "GameLayer.h"
#include "DebugDrawNode.h"
#include "Background.h"
#include "GameManager.h"
#include "ResultDialog.h"

#define PTM_RATIO 32
using namespace cocos2d;
// 概要：初期化
bool GameLayer::init()
{
	if (!Layer::init())
		return false;
	// 物理空間
	initPhysics();	
	// 背景
	Background* ground = Background::create(m_world);
	this->addChild(ground);
	// ゴミ生成
	m_gomiSystem = GomiSystem::create(m_world);
	this->addChild(m_gomiSystem);
	// カラス
	m_crow = Crow::create(m_world);
	this->addChild(m_crow);
	// センサー
	m_sensor = Sensor::create(m_world);
	this->addChild(m_sensor);
	// 更新処理開始
	scheduleUpdate();
	return true;
}
// 概要：更新処理
void GameLayer::update(float delta)
{
	m_world->Step(delta, 8, 3);
	// BodyのSprite更新
	for (b2Body* body = m_world->GetBodyList();
	body != nullptr; body = body->GetNext())
	{
		Sprite* spr = (Sprite*)body->GetUserData();
		if (spr == nullptr) continue;
		b2Vec2 pos = body->GetPosition();
		spr->setPosition(pos.x*PTM_RATIO, pos.y*PTM_RATIO);
		float rot = body->GetAngle();
		spr->setRotation(-CC_RADIANS_TO_DEGREES(rot));
		// bodyの削除
		checkFinish(body);
	}
}
// 概要：物理空間初期処理
void GameLayer::initPhysics()
{
	// ワールド作成
	b2Vec2 gravity(0.0f, -9.8f);
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(true);
	// デバック表示
	DebugDrawNode* debugDraw = DebugDrawNode::create(m_world);
	this->addChild(debugDraw, 10000);
	m_world->SetContactListener(this);
}
// 概要：Body同士の接触開始処理
void GameLayer::BeginContact(b2Contact * contact)
{
	if (m_gomiSystem->BeginContact(contact))return;
	if (m_sensor->BeginContact(contact))return;
	if (m_crow->BeginContact(contact))return;
}
// 概要：Bodyの終了処理
void GameLayer::checkFinish(b2Body* body)
{
	if (!m_label)return;
	Sprite* spr = (Sprite*)body->GetUserData();
	b2Vec2 pos = body->GetPosition();
	float y = -spr->getContentSize().height / 2 * spr->getScale() / PTM_RATIO;
	// 画面上ならreturn
	if (pos.y > y)return;
	// kinematicBody以外ならreturn
	if (body->GetType() != b2_kinematicBody)return;
	// ミッションと違う条件の種類のゴミならreturn
	if (m_mission.getType() != spr->getTag())return;
	// Body、Spriteの削除
	spr->removeFromParent();
	m_world->DestroyBody(body);
	// 文字の更新
	int count = m_mission.getCount() - 1;
	m_mission.setCount(count);
	char s[64];
	std::string string = m_mission.getString();
	sprintf(s, "%sを残り%d個処理", string.c_str(), count);
	m_label->setString(s);
	// クリア判定
	setGameClear();
}
// 概要：ゲームクリア判定
void GameLayer::setGameClear()
{
	// ミッションの条件の数に達してないならreturn
	if (m_mission.getCount() > 0)return;
	int stage = GameManager::getInstance()->getStage();	
	char s[32];
	sprintf(s, "STAGES%d", stage);
	UserDefault::getInstance()->setBoolForKey(s, true);
	Director::getInstance()->pause();
	ResultDialog* resultDialog = ResultDialog::create(true);
	this->getParent()->addChild(resultDialog, 20000);
}
// 概要：ミッション設定
void GameLayer::setMission()
{
	m_mission = GameManager::getInstance()->getMission();	
	int count = m_mission.getCount();
	std::string string = m_mission.getString();	
	char s[64];
	sprintf(s, "%sを残り%d個処理", string.c_str(), count);
	m_label = Label::create(s, "Arial", 30);
	//m_label->setColor(Color3B::BLACK);
	m_label->setPosition(250, 750);
	this->getParent()->addChild(m_label,100);
}