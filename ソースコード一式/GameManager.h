#pragma once

#include "Mission.h"

class GameManager
{
public:
	static GameManager* getInstance();	
	Mission getMission()const {	return m_mission;}
	void setMission(const Mission mission) { m_mission = mission; }
	int getStage()const { return m_stage; }
	void setStage(const int stage) { m_stage = stage; }
protected:
	GameManager() {}
private:
	GameManager(const GameManager&) {};
	GameManager& operator = (const GameManager&) {};
	static GameManager* _instance;	
	Mission m_mission;
	int m_stage;
};