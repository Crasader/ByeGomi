#include "Gomi.h"
#include "GomiSystem.h"
#include "QueryCallback.h"
#include "AudioEngine.h"
#define PTM_RATIO 32.0f
using namespace cocos2d;
// 概要：初期化
bool GomiSystem::init(b2World* world)
{
	if (!Node::init())
		return false;
	// Worldを覚える
	m_world = world;
	// 10秒ごとに粗大ゴミ生成
	DelayTime* time = DelayTime::create(10.0f);
	CallFunc* func = CallFunc::create([this]()
	{
		Gomi* gomi = Gomi::create(m_world);
		gomi->setEntity(3);
		b2Vec2 position(random(100, 400) / PTM_RATIO, 750 / PTM_RATIO);
		gomi->setTransform(position, 0);
		this->addChild(gomi);
	});	
	Sequence* seq = Sequence::create(time, func, nullptr);
	this->runAction(RepeatForever::create(seq));
	// タッチ有効化
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	// 更新処理開始
	scheduleUpdate();
	return true;
}
// 概要；更新処理
void GomiSystem::update(float delta)
{
	// ゴミ生成
	createGomi();
	// ゴミ融合
	fusionGomi();
}
// 概要：ランダムでゴミ生成
void GomiSystem::createGomi()
{
	static float time;
	static float rand = random(0.5f, 2.0f);
	time += 1.0f / 60;
	if (time < rand)return;
	rand = random(0.5f, 2.0f);
	time = 0.0f;
	Gomi* gomi = Gomi::create(m_world);
	gomi->setEntity(random(0, 2));
	b2Vec2 position(random(100, 400) / PTM_RATIO, 700 / PTM_RATIO);
	gomi->setTransform(position, 0);
	this->addChild(gomi);
}
// 概要：同じゴミ同士の融合
void GomiSystem::fusionGomi()
{
	if (!m_bodyA || !m_bodyB)return;
	experimental::AudioEngine::play2d("gomi/fusion.mp3");
	Sprite* sprA = (Sprite*)m_bodyA->GetUserData();
	Sprite* sprB = (Sprite*)m_bodyB->GetUserData();
	// マウスジョイントを削除
	if (m_mousejoint != nullptr)
	{
		if (m_bodyA == m_mousejoint->GetBodyB() ||
			m_bodyB == m_mousejoint->GetBodyB())
			m_mousejoint = nullptr;
	}
	// 基準にするBodyを覚える
	b2Body* body;
	if (sprA->getScale() > sprB->getScale())
		body = m_bodyA;
	else if (sprA->getScale() == sprB->getScale())
	{
		if (sprA->getPosition().y < sprB->getPosition().y)
			body = m_bodyA;
		else body = m_bodyB;
	}
	else body = m_bodyB;
	// ゴミの種類
	int tag = sprA->getTag();
	// ゴミが融合した時の倍率
	float scaleA = sprA->getScale()*sprA->getScale();
	float scaleB = sprB->getScale()*sprB->getScale();
	float scale = sqrtf(scaleA + scaleB);
	// 新しいゴミ生成
	Gomi* gomi = Gomi::create(m_world);
	gomi->setEntity(tag, scale);
	gomi->setTransform(body->GetPosition(), body->GetAngle());
	gomi->getBody()->SetAngularVelocity(body->GetAngularVelocity());
	this->addChild(gomi);
	// 2つのゴミ削除
	destroyGomi(m_bodyA);
	destroyGomi(m_bodyB);
}
// 概要：Bodyの削除
void GomiSystem::destroyGomi(b2Body* &body)
{
	Sprite* spr = (Sprite*)body->GetUserData();
	spr->removeFromParent();
	m_world->DestroyBody(body);
	body = nullptr;
}
// 概要：融合できるゴミ同士が接触したか調べる
bool GomiSystem::BeginContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	// dynamicBody以外はreturn
	if (bodyA->GetType() != b2_dynamicBody ||
		bodyB->GetType() != b2_dynamicBody)
		return false;
	Sprite* sprA = (Sprite*)bodyA->GetUserData();
	Sprite* sprB = (Sprite*)bodyB->GetUserData();
	// Spriteがnullならreturn
	if (!sprA || !sprB)return false;
	// 融合できないゴミならreturn
	if (sprA->getTag() > 2 || sprB->getTag() > 2)return false;
	if (sprA->getTag() != sprB->getTag())return false;
	// 接触した同士のBodyを覚える
	m_bodyA = bodyA;
	m_bodyB = bodyB;
	return true;
}
// 概要：タッチ開始処理
bool GomiSystem::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 location(touch->getLocation());
	// 物理空間座標に変換	
	b2Vec2 p(location.x / PTM_RATIO, location.y / PTM_RATIO);
	// 当たり判定
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;
	QueryCallback callback(p);
	m_world->QueryAABB(&callback, aabb);
	if (!callback.m_body)return false;
	// 小さいゴミ以外触れない
	Sprite* spr = (Sprite*)callback.m_body->GetUserData();
	if (spr->getTag() > 2) return false;
	// マウスジョイント
	b2Body *backGroundBody = m_world->CreateBody(new b2BodyDef());
	b2MouseJointDef md;
	md.bodyA = backGroundBody;
	md.bodyB = callback.m_body;
	md.target = p;
	md.maxForce = 1000.0f*callback.m_body->GetMass();
	m_mousejoint = (b2MouseJoint*)m_world->CreateJoint(&md);
	return true;
}
// 概要：タッチ移動処理
void GomiSystem::onTouchMoved(Touch* touch, Event* event)
{
	Vec2 location(touch->getLocation());
	if (!m_mousejoint)return;
	b2Vec2 p(location.x / PTM_RATIO, location.y / PTM_RATIO);
	m_mousejoint->SetTarget(p);
}
// 概要：タッチ終了処理
void GomiSystem::onTouchEnded(Touch* touch, Event* event)
{
	// マウスジョイント削除
	if (!m_mousejoint)return;
	m_world->DestroyJoint(m_mousejoint);
	m_mousejoint = nullptr;
}
// 概要：GomiSystemの生成
GomiSystem* GomiSystem::create(b2World* world)
{
	GomiSystem* pRet = new(std::nothrow)GomiSystem();
	if (pRet && pRet->init(world))
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