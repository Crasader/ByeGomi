#include "CountDown.h"
using namespace cocos2d;
// 概要：初期化
// 引数：制限時間
bool CountDown::init(const float LimitTime, 
	const std::string& filename1, const std::string& filename2)
{
	if (!Node::init())
		return false;
	// 制限時間を覚える
	m_time = LimitTime;
	m_LimitTime = LimitTime;
	m_changeTime = 10.0f;
	// パーセントに変換
	m_percent = 100.0f / (m_time-1);
	// 裏の画像
	Sprite* back = Sprite::create(filename1);
	this->addChild(back);
	Vec2 center = back->getContentSize() / 2;
	// 表の画像
	Sprite* front = Sprite::create(filename2);
	// タイマー作成
	m_timer = ProgressTimer::create(front);
	m_timer->setType(kCCProgressTimerTypeRadial);
	m_timer->setPosition(center);
	back->addChild(m_timer);
	// 時間の表示
	char s[12];
	sprintf(s, "%.f", m_time);
	m_label = Label::create(s, "Arial", 30);
	m_label->setPosition(center);
	back->addChild(m_label);
	scheduleUpdate();
	return true;
}
// 概要：毎秒カウントダウン
void CountDown::update(float delta)
{	
	// 一度のみflagをtrue
	if (m_FinishFlag) 
	{
		m_FinishFlag = false;
		unscheduleUpdate();
		return;
	}
	// パーセント更新
	float percent = m_timer->getPercentage();
	percent += m_percent*delta;
	m_timer->setPercentage(percent);
	// 時間の更新
	m_time -= delta;
	// 10秒切ったら赤文字にする
	if (m_time <= m_changeTime)
		m_label->setColor(Color3B::RED);	
	// 表示更新
	char s[12];
	sprintf(s, "%.f", m_time);
	m_label->setString(s);
	// 終了
	if (m_time <= delta)
		m_FinishFlag = true;
}
// 概要：CountDownの生成
CountDown * CountDown::create(const float LimitTime,
	const std::string& filename1, const std::string& filename2)
{
	CountDown* pRet = new(std::nothrow)CountDown();
	if (pRet && pRet->init(LimitTime, filename1, filename2))
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