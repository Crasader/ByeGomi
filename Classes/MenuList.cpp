﻿#include "MenuList.h"
#include "PlayScene.h"
#include "SelectScene.h"
#include "AudioEngine.h"
using namespace cocos2d;
const float speed = 0.2f;
// 概要：初期化
bool MenuList::init()
{
	if (!Node::init())
		return false;
	// 色の設定
	m_color = LayerColor::create();
	this->addChild(m_color);
	// リストを開くボタン
	m_button = ui::Button::create("menu/menu.png");
	// タッチ時の処理
	m_button->addTouchEventListener(
		CC_CALLBACK_2(MenuList::touchEvent, this));
	this->addChild(m_button);
	return true;
}
// 概要：目標位置
// 引数：初期位置false、中心位置true
cocos2d::Vec2 MenuList::getTargetPosition(bool target)const
{
	// 画面サイズ取得
	Size screen_size = Director::getInstance()->getVisibleSize() / 2;
	// リストサイズ取得
	Size list_size = m_listView->getContentSize() / 2;
	// 中心位置
	Vec2 position = screen_size - list_size;
	// 初期位置なら
	if (target == false) position.x *= -1;
	return position;
}
// 概要：リスト生成
void MenuList::createListView()
{
	m_listView = ui::ListView::create();
	// ボタン同士の間隔
	m_listView->setItemsMargin(20.0f);
	this->addChild(m_listView);
	Size size;
	float margin = m_listView->getItemsMargin();
	for (int i = 0; i < ALL; i++)
	{
		char s[32];
		if (i == SELECT)sprintf(s, "menu/select.png");
		else if (i == RESET)sprintf(s, "menu/reset.png");
		ui::Button* button = ui::Button::create(s);
		Vec2 pos = button->getContentSize() / 2 + Size(margin, -margin);
		button->setPosition(pos);
		auto layout = ui::Layout::create();
		layout->setContentSize(button->getContentSize());
		layout->addChild(button);
		m_listView->addChild(layout);
		size.width = button->getContentSize().width + margin * 2;
		size.height += button->getContentSize().height + margin;
	}
	// リストサイズ設定
	m_listView->setContentSize(size);
	//初期地点
	m_listView->setPosition(getTargetPosition(false));
	m_listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_listView->setBounceEnabled(false);
	m_listView->setScrollBarEnabled(false);
	m_listView->addEventListener((ui::ListView::ccListViewCallback)
		CC_CALLBACK_2(MenuList::selectedItemEvent, this));
}
// 概要：リスト移動開始処理
void MenuList::BeginListView()
{
	// 画面を暗くする
	m_color->initWithColor(
		Color4B(0x00, 0x00, 0x00, 0xff));
	m_color->setOpacity(100);
	cocos2d::Node* parent = this->getParent();
	this->pauseRecursive(parent, true);
	this->pauseRecursive(this, false);
	// リスト生成
	createListView();
	// 中心に持ってく
	MoveTo* move = MoveTo::create(speed, getTargetPosition(true));
	m_listView->runAction(move);
}
// 概要：リスト移動終了処理
void MenuList::EndListView()
{
	// 画面を明るくする
	m_color->initWithColor(
		Color4B(0x00, 0x00, 0x00, 0x00));
	cocos2d::Node* parent = this->getParent();
	this->pauseRecursive(parent, false);
	// 初期位置に戻す
	MoveTo* move = MoveTo::create(speed, getTargetPosition(false));
	RemoveSelf* re = RemoveSelf::create();
	CallFunc* func = CallFunc::create([this](){m_listView = nullptr;});
	Sequence* seq = Sequence::create(move, re, func, nullptr);
	m_listView->runAction(seq);
}
// 概要：親子関係全てを　pause,resume
void MenuList::pauseRecursive(Node* node, bool pause)
{
	if (pause)
		node->pause();
	else
		node->resume();
	Ref* child = nullptr;
	auto& children = node->getChildren();
	for (size_t i = 0; i < children.size(); i++)
		pauseRecursive(children.at(i), pause);	
}
// 概要：ボタンの処理
void MenuList::touchEvent(
	Ref * pSender, ui::Widget::TouchEventType type)
{
	// タッチ終了時じゃないならreturn
	if (type != ui::Widget::TouchEventType::ENDED)
		return;	
	experimental::AudioEngine::play2d("button.mp3");
	// リスト移動処理
	if (m_listView)
	{
		// アクションしてないなら
		if (m_listView->numberOfRunningActions() == 0)
			EndListView();		
	}	
	else BeginListView();	
}
// 概要：リストのボタン処理
void MenuList::selectedItemEvent(
	Ref * pSender, ui::ListView::EventType type)
{
	// タッチ終了時じゃないならreturn
	if (type != ui::ListView::EventType::ON_SELECTED_ITEM_END)
		return;
	experimental::AudioEngine::stopAll();
	experimental::AudioEngine::play2d("button.mp3");
	auto listView = static_cast<ui::ListView*>(pSender);
	switch (listView->getCurSelectedIndex())
	{
	case SELECT:
		Director::getInstance()->replaceScene(
			SelectScene::createScene());
		break;
	case RESET:
		Director::getInstance()->replaceScene(
			PlayScene::createScene());
		break;
	default: assert(false); break;
	}
}
