#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "cocos2d.h"
#include "../Classes/Poker.h"
#include "../editor-support/cocostudio/SimpleAudioEngine.h"   //包含声音引擎头文件
#include <vector>
#include<string>
#define SCALE_FACTOR 0.625f //位置规模因子
USING_NS_CC;

class GameScene;

// 牌型
enum  CARD_TYPE{
	SINGLE_CARD = 1,			//单牌
	DOUBLE_CARD,			//对子
	THREE_CARD,				//3不带
	BOMB_CARD,				//炸弹
	MISSILE_CARD,			//火箭
	THREE_ONE_CARD,			//3带1
	THREE_TWO_CARD,			//3带2
	BOMB_TWO_CARD,			//四个带2张单牌
	BOMB_TWOOO_CARD,		//四个带2对
	CONNECT_CARD,			//连牌
	COMPANY_CARD,			//连对
	AIRCRAFT_CARD,			//飞机不带
	AIRCRAFT_SINGLE_CARD,	//飞机带单牌
	AIRCRAFT_DOUBLE_CARD,	//飞机带对子
	ERROR_CARD				//错误的牌型
};

// 每种牌的信息
struct CARDS_DATA{
	std::vector<int>	_cards;//牌的内容
	CARD_TYPE			_type;//牌型
	unsigned int		_value;//值
};

//出牌展示区域
class PokerShowZone :public cocos2d::Sprite{
public:
	static  PokerShowZone* create()
	{
		static PokerShowZone* sprite = new(std::nothrow) PokerShowZone();
		if (sprite && sprite->init())
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	virtual bool init() {
		if (!Sprite::init())
		{
			return false;
		}
		return true;
	}

	void chupai(std::vector<PokerInfo> arrayIndexes)//出牌
	{
		if (arrayIndexes.empty())//没有选中手牌
		{
			auto label_buchu = cocos2d::Sprite::create("buchu.png");
			this->addChild(label_buchu);
		}

		for (int i = 0; i < arrayIndexes.size(); ++i)//选中了手牌
		{
			auto card = Poker::create(arrayIndexes.at(i));
			card->setScale(0.5);//更改大小
			this->addChild(card, 100 - arrayIndexes.at(i)._num);
		}
		sortAllChildren();//排序

		int count = _children.size();
		for (int i = 0; i < _children.size(); ++i)
		{
			Poker* card = dynamic_cast<Poker*>(_children.at(i));
			if (card != NULL)
			{
				card->setPosition(i * 30 /SCALE_FACTOR,0);//更改位置
			}
		}
	}
};


//玩家类

class Player :public cocos2d::Sprite {
public:
	static Player* create(std::string name,bool isHero);//创建玩家 
	virtual bool init(std::string name, bool isHero);//初始化
	void updateCards();//更新手牌
	void clearCards();//清空手牌

	//是否是地主
	void setLandlord();//设置是否是地主
	bool getIsLandlord();//返回true 就是地主

	void DealCards(GameScene* scene, PokerInfo info);	//发牌
	void OutCards(GameScene* scene, bool isFollow, CARD_TYPE cardtype, unsigned int count, unsigned int value);//出牌
	void BuChu();//不出
	void ChaiPai();//拆牌
	void ShowTipInfo(bool isFollow, CARD_TYPE cardType, unsigned int count, unsigned int value);//展示提示信息
	bool IsQiangLandlord();//是否抢地主
	std::string GetName();//获得名字
	void SetPlayerID(size_t id);//设置玩家ID

	size_t GetPlayerID();//获得玩家ID

	//找出跟牌的牌型
	std::vector<int>& FindFollowCards(CARD_TYPE cardType, unsigned int count, unsigned int value);
	//找出牌的牌型
	std::vector<int>& FindOutCards();
	std::vector<PokerInfo>& GetOutCards();//获得出牌的牌型

protected:
	size_t _id;//玩家ID
	bool _isHero;//是否是第一个出完牌的
	bool _isLandlord;//是否是地主
	
	std::string _name;//姓名
	//int _score;//成绩
	cocos2d::Sprite* _cardsManager;//卡牌管理
	cocos2d::Label* _labelName;//名称
	cocos2d::Label* _labelPokerCount;//计数

	std::vector<PokerInfo> _vecPokerInfo;//扑克信息数组
	std::vector<int> _vecFindFollowCards;//跟牌
	std::vector<PokerInfo> _vecOutCards;//出牌

	PokerShowZone* _showZone;//展示区
	std::vector<CARDS_DATA> _allCardGroups;//卡组
	cocos2d::Label* _labelTipInfo;//提示信息

	int m_intArray[15];//记录每张牌出现的次数

};

#endif // __PLAYER_H__
