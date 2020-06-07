#ifndef __BOTTOM_CARD_ZONE_H__
#define __BOTTOM_CARD_ZONE_H__
#include "cocos2d.h"
#include "Poker.h"
//显示底牌区域 继承cocos2d的精灵类
class BottomCardZone :public cocos2d::Sprite
{
public:
	static BottomCardZone* create();//新建底牌显示区域
	virtual bool init();//初始化
	void Show(PokerInfo cardInfo);//展示卡牌信息
};
#endif // __BOTTOM_CARD_ZONE_H__