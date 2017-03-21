#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Sensor.h"
#include "GomiSystem.h"
#include "Crow.h"
#include "Mission.h"
class GameLayer :public cocos2d::Layer, b2ContactListener
{
private:
	Mission m_mission;
	cocos2d::Label* m_label;
	b2World* m_world;
	Sensor* m_sensor;
	Crow* m_crow;
	GomiSystem* m_gomiSystem;	
	bool init()override;
	void update(float delta)override;
	void initPhysics();
	void BeginContact(b2Contact* contact)override;
	void checkFinish(b2Body* body);	
	void setGameClear();
public:
	void setMission();	
	CREATE_FUNC(GameLayer);
};