#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
class MenuList: public cocos2d::Node
{
public:
	enum { SELECT, RESET, ALL };
	cocos2d::LayerColor* m_color;
	cocos2d::ui::Button* m_button;
	cocos2d::ui::ListView* m_listView;
	bool init()override;
	void createListView();
	void BeginListView();
	void EndListView();	
	cocos2d::Vec2 getTargetPosition(bool target)const;
	void pauseRecursive(cocos2d::Node* node, bool pause);
	void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void selectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
public:
	void setPosition(float x,float y) { m_button->setPosition(cocos2d::Vec2(x,y)); }
	CREATE_FUNC(MenuList);
};