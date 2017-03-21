#pragma once
#include "cocos2d.h"
class CountDown : public cocos2d::Node
{
private:
	float m_time;
	float m_percent;
	float m_LimitTime;
	float m_changeTime;
	bool m_FinishFlag;
	cocos2d::Label* m_label;
	cocos2d::ProgressTimer* m_timer;
	bool init(const float LimitTime,
		const std::string& filename1,
		const std::string& filename2);
	void update(float delta)override;
public:
	// 概要：時間更新
	// 戻り値：０になったらtrueを返す
	bool checkFinish()const { return m_FinishFlag; }
	// 概要：制限時間取得
	float getLimitTime()const { return m_LimitTime; }
	// 概要：現在時間の変更
	void setNowTime(const float time) { m_time = time; }
	// 概要：現在時間取得
	float getNowTime()const { return m_time; }
	// 概要：何秒から数字の色を変えるか設定
	void setChangeTime(const float changeTime) { m_changeTime = changeTime; }
	// 概要：色を変える時間を取得
	float getChangeTime()const { return m_changeTime; }	
	static CountDown* create(const float LimitTime,
		const std::string& filename1, 
		const std::string& filename2);	
};