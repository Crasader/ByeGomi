#include "Background.h"
#define PTM_RATIO 32.0f
using namespace cocos2d;
// 概要：初期化
bool Background::init(b2World * world)
{
	if (!Node::init())
		return false;
	m_world = world;
	Vec2 size = Director::getInstance()->getVisibleSize()/2;
	Sprite* spr = Sprite::create("background/background.png");
	spr->setPosition(size);
	this->addChild(spr);
	float x = size.x; float w = 20.0f;
	float y = 0.0f; float h = size.y;	
	b2PolygonShape shape;
	// 右壁
	shape = getShape(x, y, w, h);
	addBody(shape);
	// 左壁
	shape = getShape(-x, y, w, h);
	addBody(shape);
	// 天井
	shape = getShape(y, h, x, w*5);
	addBody(shape);	
	//// 床
	//shape = getShape(y, -h, x, w);
	//addBody(shape);
	return true;
}
// 概要：形状取得
b2PolygonShape Background::getShape(float x, float y, float w, float h)
{
	b2PolygonShape shape;
	b2Vec2 points[] =
	{
		{ (x + w) / PTM_RATIO,(y + h) / PTM_RATIO },
		{ (x + w) / PTM_RATIO,(y - h) / PTM_RATIO },
		{ (x - w) / PTM_RATIO,(y - h) / PTM_RATIO },
		{ (x - w) / PTM_RATIO,(y + h) / PTM_RATIO },
	};
	int count = sizeof(points) / sizeof(b2Vec2);
	shape.Set(points, count);
	return shape;
}
// 概要：Bodyの追加
void Background::addBody(b2Shape & shape)
{
	if (m_body == nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		Vec2 position = Director::getInstance()->getVisibleSize() / 2;		
		bodyDef.position = b2Vec2(position.x/PTM_RATIO, position.y / PTM_RATIO);
		m_body = m_world->CreateBody(&bodyDef);
	}
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.filter.groupIndex = -1;
	m_body->CreateFixture(&fixtureDef);
}
// 概要：BackGroundの生成
Background * Background::create(b2World * world)
{
	Background* pRet = new(std::nothrow)Background();
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
