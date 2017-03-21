#pragma once
#include "Box2D\Box2D.h"
class QueryCallback :public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_body = nullptr;
	}
	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_body = fixture->GetBody();
				return false;
			}
		}
		return true;
	}
	b2Vec2 m_point;
	b2Body* m_body;
};