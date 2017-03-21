#pragma once
#pragma execution_character_set("utf-8")
#include <string>

enum GomiType{ Burnable, Bin, Raw, Closet, Paper, Debris, Carcass };

class Mission
{
private:
	const std::string name[7] =
	{
		"可燃ゴミ","ビン","生ゴミ",
		"粗大ゴミ","雑紙","破片","死骸"
	};
	std::string m_string;
	int m_type;
	int m_count;	
public:
	Mission()
	{
		m_type = Burnable;
		m_count = 0;
	}

	Mission operator =(const Mission& mission)
	{
		m_string = mission.m_string;
		m_type = mission.m_type;
		m_count = mission.m_count;		
		return(*this);
	}

	Mission(int type,int count)
	{
		m_string = name[type];
		m_type = type;		
		m_count = count;
	}
	void setCount(const int count) { m_count = count; }
	std::string getString()const { return m_string; }
	int getType()const { return m_type; }
	int getCount()const { return m_count; }
};