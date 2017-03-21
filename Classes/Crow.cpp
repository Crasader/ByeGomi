#include "Crow.h"
#include "AudioEngine.h"
#include "Gomi.h"
#define PTM_RATIO 32.0f
using namespace cocos2d;
// 概要：初期化
bool Crow::init(b2World * world)
{
	if (!Node::init())
		return false;
	// Worldを覚える
	m_world = world;
	// 中心軸
	m_center.x = 250.0f / PTM_RATIO;
	// 更新処理開始
	scheduleUpdate();
	return true;
}
// 概要：更新処理
void Crow::update(float delta)
{	
	if (createCrow())return;
	// カラス更新処理
	setMove();
	setEnd();
	setEvent();
}
bool Crow::createCrow()
{
	if (m_body)return false;
	static float time;
	static float rand = random(0.5f, 2.0f);
	time += 1.0f/60;
	// ランダムにカラス生成
	if (time < rand)return true;
	rand = random(0.5f, 2.0f);
	time = 0.0f;
	createBody();
	// 移動の角度
	m_radian = 315 * 3.14f / 180;
	m_center.y =
		random(600, 1000) / PTM_RATIO;
	setSE();
	return false;
}
// 概要：カラスの移動処理
void Crow::setMove()
{
	// カラス移動処理	
	b2Vec2 position;
	//移動の半径
	float radius = 400.0f / PTM_RATIO;
	position.x = radius*cosf(m_radian) + m_center.x;
	position.y = radius*sinf(m_radian) + m_center.y;
	m_body->SetTransform(position, 0);
	// 変更する数値
	float change = 90.0f / 60.0f*3.14 / 180;
	m_radian -= change;
}
// 概要：カラス終了処理
void Crow::setEnd()
{
	// BodyのSprite更新
	Sprite* spr = (Sprite*)m_body->GetUserData();
	// 画面外に出たら削除
	float x = spr->getPosition().x + spr->getContentSize().width;
	if (x > 0)return;	
	if (m_body->GetJointList() != nullptr)
	{
		b2Body* other = m_body->GetJointList()->other;
		Sprite* spr = (Sprite*)other->GetUserData();
		spr->removeFromParent();
		m_world->DestroyBody(other);
	}
	spr->removeFromParent();
	m_world->DestroyBody(m_body);
	m_body = nullptr;
}
// 概要：カラス効果音処理
void Crow::setSE()
{	
	CallFunc* func = CallFunc::create([this]()
	{
		experimental::AudioEngine::play2d("crow/se.mp3",false,0.4f);
	});
	DelayTime* time = DelayTime::create(0.5f);
	Sequence* seq = Sequence::create(func, time, func, nullptr);
	this->runAction(seq);
}
// 概要：カラスのBody作成
void Crow::createBody()
{	
	// Sprite作成
	Sprite* spr = createSprite();
	// 画面サイズ
	Vec2 pos = Director::getInstance()->getVisibleSize();
	// Spriteサイズ
	Size size = spr->getContentSize() / 2;
	pos.x += size.width; pos.y /= 2;
	// Body作成
	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);
	bodyDef.type = b2_kinematicBody;
	bodyDef.userData = spr;
	m_body = m_world->CreateBody(&bodyDef);
	b2FixtureDef fixtureDef;
	b2PolygonShape shape;
	shape.SetAsBox(size.width / PTM_RATIO,
		size.height / 2 / PTM_RATIO);
	fixtureDef.shape = &shape;
	m_body->CreateFixture(&fixtureDef);
}
void Crow::setEvent()
{
	if (m_gomi==nullptr)return;	
	b2MouseJoint* joint = (b2MouseJoint*)m_gomi->GetJointList();
	if (joint)
	{
		m_gomi = nullptr;
		return;
	}
	Sprite* spr = (Sprite*)m_gomi->GetUserData();
	int tag = spr->getTag();
	if (tag <= 2)
		experimental::AudioEngine::play2d("gomi/se.mp3");	
	switch (tag)
	{
	case 0:setSubGomi(4); break;
	case 1:setSubGomi(5); break;
	case 2:setRaw(); break;
	case 3:setCloset(); break;
	default:  break;
	}
	m_gomi = nullptr;	
}
void Crow::setRaw()
{	
	b2Filter filter = m_gomi->GetFixtureList()->GetFilterData();
	filter.maskBits = 0;
	m_gomi->GetFixtureList()->SetFilterData(filter);
	m_gomi->SetSleepingAllowed(false);
	b2WeldJointDef weldjointDef;
	weldjointDef.bodyA = m_body;
	weldjointDef.bodyB = m_gomi;
	////それぞれのボディを繋ぐ
	weldjointDef.localAnchorA = b2Vec2(0, -0.5f);
	weldjointDef.localAnchorB = b2Vec2(0, -0.5f);
	weldjointDef.dampingRatio = 10000.0f;
	//1秒間にふる回数
	weldjointDef.frequencyHz = 2;
	weldjointDef.collideConnected = false;
	b2Joint* joint = m_world->CreateJoint(&weldjointDef);
}
void Crow::setCloset()
{
	Gomi* sub = Gomi::create(m_world);
	sub->setEntity(6, 0.8f);
	sub->setTransform(m_body->GetPosition(), 0);
	this->addChild(sub);
	Sprite* spr = (Sprite*)m_body->GetUserData();
	spr->removeFromParent();
	m_world->DestroyBody(m_body);
	m_body = nullptr;
	experimental::AudioEngine::play2d("crow/death.mp3");
}
void Crow::setSubGomi(int type)
{
	Gomi* sub = Gomi::create(m_world);
	sub->setEntity(type, 0.8f);
	sub->setTransform(m_gomi->GetPosition(), 0);
	this->addChild(sub);
}
// 概要：カラスのSprite作成
Sprite* Crow::createSprite()
{
	Sprite* spr = Sprite::create("crow/crow1.png");
	this->addChild(spr);
	// アニメーション
	Animation* animation = Animation::create();
	animation->addSpriteFrameWithFileName("crow/crow1.png");
	animation->addSpriteFrameWithFileName("crow/crow2.png");
	animation->setDelayPerUnit(0.6f / 5.0f);
	Animate* action = Animate::create(animation);
	spr->runAction(RepeatForever::create(action));
	return spr;
}
// 概要：カラスとゴミの接触か調べる
bool Crow::BeginContact(b2Contact * contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	if (bodyA != m_body && bodyB != m_body)
		return false;
	if (bodyA == m_body)
		m_gomi = bodyB;
	else if(bodyB == m_body)
		m_gomi = bodyA;
	return true;
}
// 概要：Crow生成
Crow * Crow::create(b2World * world)
{
	Crow* pRet = new(std::nothrow)Crow();
	if (pRet && pRet->init(world))
	{
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}