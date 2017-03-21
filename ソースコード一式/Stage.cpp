#include "Stage.h"
#include "AudioEngine.h"
#include "StartDialogLayer.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
// 概要：Stage生成
Stage* Stage::create(Mission mission, int stageNum)
{
	Stage* pRet = new(std::nothrow)Stage();
	if (pRet && pRet->init(mission, stageNum))
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
// 概要：初期化
bool Stage::init(Mission mission, int stageNum)
{
	if (!Node::init())
		return false;
	m_stage = stageNum;
	m_mission = mission;
	// ステージボタン
	ui::Button* button = ui::Button::create("select/button/stage.png");
	button->addTouchEventListener(
		[&](Ref * pRef, ui::Widget::TouchEventType type) 
	{
		if (type != ui::Widget::TouchEventType::ENDED)return;
		popupStartDialog();
		experimental::AudioEngine::play2d("select/button.mp3");		
	});
	this->addChild(button);
	/*for (int i = 0; i < 10; i++)
	{
		char s[64];
		sprintf(s, "STAGES%d", i);
		UserDefault::getInstance()->setBoolForKey(s, false);
	}*/
	// クリア済み画像表示
	UserDefault* userDefault = UserDefault::getInstance();
	char s[64];
	sprintf(s, "STAGES%d", stageNum);
	bool isClear = userDefault->getBoolForKey(s, false);
	if (!isClear)return true;
	Sprite* clear = Sprite::create("select/clear.png");	
	clear->setPosition(50, 50);
	button->addChild(clear);
	scheduleUpdate();
	return true;
}
// 概要：確認する画面
void Stage::popupStartDialog()
{	
	Director::getInstance()->pause();
	StartDialog* startDialog = StartDialog::create(m_mission,m_stage);
	this->getParent()->addChild(startDialog, 10000);
}