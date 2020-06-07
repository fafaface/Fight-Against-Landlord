#ifndef  __POKER_H__
#define __POKER_H__

#include "cocos2d.h"

class GameScene;//声明游戏场景类

enum PokerNum {//扑克牌值,依次增大
	NUM_3 = 0,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_10,
	NUM_J,
	NUM_Q,
	NUM_K,
	NUM_A,
	NUM_2,
	NUM_XW,//小王
	NUM_DW//大王
};

enum PokerTag // 花色
{
	FANGKUAI = 0,//方块♦
	MEIHUA,//梅花♣
	HONGTAO,//红桃♥
	HEITAO//黑桃♠
};

struct PokerInfo//扑克牌信息
{
	PokerNum _num;//牌值
	PokerTag _tag;//花色

	inline bool operator==(const PokerInfo& pi) const//内联布尔函数
	{//如果牌值和花色都一样
		return (this->_num == pi._num && this->_tag == pi._tag);
	}//判定是否完全相同
};

class Poker : public cocos2d::Sprite {//因为扑克牌需要移动, 出现, 消失, 因此扑克牌是一个Sprite

public:	
	CC_SYNTHESIZE(GameScene*, _gameMain, GameMain);
	static Poker* create(PokerInfo info, bool isSmall = false);//新建一组扑克牌
	virtual bool init(PokerInfo info, bool isSmall = false);//初始化
	void SetTouchEnabled();//是否可以点击, 比如没有到出牌阶段就不能点击
	void click();//点击
	bool isSelected();//判断是否选中
	void unSelect();//不选
	void Select();//选择

	PokerInfo getInfo();//获取扑克牌信息
	cocos2d::Rect getRect();
//	bool onTouchBegan(Touch* touch, Event* event);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);//开始
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event); //结束
	virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);//取消
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);//滑动



	PokerInfo	_info;
protected:
	bool		_isSelected;
	cocos2d::Size _size;
};




#endif //  __POKER_H__
