#include "Sensor.h"
#include "AudioEngine.h"
#define PTM_RATIO 32.0f
using namespace cocos2d;
// 概要：初期化
bool Sensor::init(b2World * world)
{
	if (!Node::init())
		return false;
	// 画面サイズ取得
	Size size = Director::getInstance()->getVisibleSize()/2;
	// Body生成（Sensor）
	b2PolygonShape shape;
	shape.SetAsBox(size.width/ PTM_RATIO, 20.0f / PTM_RATIO);
	b2BodyDef bodyDef;
	bodyDef.position.Set(size.width/ PTM_RATIO, 20.0f / PTM_RATIO);
	bodyDef.type = b2_staticBody;
	b2Body* body = world->CreateBody(&bodyDef);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	setAnimation();
	scheduleUpdate();
	return true;
}
// 概要：更新処理
void Sensor::update(float delta)
{
	if (!m_body)return;
	if (m_body->GetType() == b2_kinematicBody)return;
	Sprite* spr = (Sprite*)m_body->GetUserData();
	if (!spr)return;
	experimental::AudioEngine::play2d("sensor/se.mp3");
	float x = m_body->GetPosition().x;
	float height = spr->getContentSize().height / 2 * spr->getScale();
	float angle = m_body->GetAngle();
	if (spr->getTag() == 3)
	{		
		float width = spr->getContentSize().width / 2;
		float rad = fabsf(cosf(CC_DEGREES_TO_RADIANS(angle)));
		float r = (height - width)*rad;
		float y = (width + r + 40.0f) / PTM_RATIO;
		m_body->SetTransform(b2Vec2(x, y), angle);
	}
	else
	{
		float y = (height + 40.0f) / PTM_RATIO;
		m_body->SetTransform(b2Vec2(x, y), angle);
	}
	spr->setColor(Color3B::RED);
	m_body->SetType(b2_kinematicBody);
	m_body->SetAngularVelocity(0);
	m_body->SetLinearVelocity(b2Vec2(0, -15 / PTM_RATIO));
	m_body = nullptr;
}
void Sensor::setAnimation()
{
	Size size = Director::getInstance()->getVisibleSize() / 2;
	Sprite* spr = Sprite::create("sensor/sensor1.png");
	spr->setPosition(size.width, 20.0f);
	this->addChild(spr);
	//CCAnimationの初期化
	auto animation = Animation::create();
	animation->addSpriteFrameWithFileName("sensor/sensor1.png");
	animation->addSpriteFrameWithFileName("sensor/sensor2.png");
	animation->setDelayPerUnit(0.6f / 5.0f);
	Animate*action = Animate::create(animation);
	spr->runAction(RepeatForever::create(action));
}
// 概要：Body接触開始処理
bool Sensor::BeginContact(b2Contact * contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	bool sensorA = contact->GetFixtureA()->IsSensor();
	bool sensorB = contact->GetFixtureB()->IsSensor();
	if (bodyA->GetType() == b2_kinematicBody ||
		bodyB->GetType() == b2_kinematicBody)
		return false;
	// 両方センサーじゃないなら
	if (!(sensorA^sensorB))
		return false;
	// Aがセンサーなら
	if (sensorA)
	{
		// 接触したBodyを覚える
		m_body = bodyB;
		return true;
	}
	// Bがセンサーなら
	if (sensorB)
	{
		// 接触したBodyを覚える
		m_body = bodyA;
		return true;
	}	
}
// 概要：Sensorの生成
Sensor * Sensor::create(b2World * world)
{
	Sensor* pRet = new(std::nothrow)Sensor();
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
