#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
namespace sugiyama
{
	class Menu : public cocos2d::Node
	{
	private:
		enum { SELECT,RESET,ALL};
		cocos2d::LayerColor* m_color;
		cocos2d::ui::Button* m_button;
		cocos2d::ui::ListView* m_listView;
		bool init()override;		
		void createListView();
		void BeginListView();
		void EndListView();
		void setNullPtr() { m_listView = nullptr; }
		cocos2d::Vec2 getTargetPosition(bool target);
		void pauseRecursive(cocos2d::Node* node, bool pause);
		void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
		void selectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
	public:
		void setPosition(cocos2d::Vec2 pos) { m_button->setPosition(pos); }
		CREATE_FUNC(Menu);
	};
}