#include "Common.h"
#include "Poker.h"
USING_NS_CC;

MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	auto image = Sprite::create(normalImage);//利用图片创建精灵
	image->setColor(Color3B(100, 100, 100));//设置颜色
	auto item = MenuItemSprite::create(Sprite::create(normalImage),image,Sprite::create(disabledImage),callback);//设置是否点击时两种情况的图片
	return item;
}

cocos2d::MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback callback)
{
	auto image = Sprite::createWithSpriteFrameName(normalImage);
	image->setColor(Color3B(100, 100, 100));
	auto item = MenuItemSprite::create(Sprite::createWithSpriteFrameName(normalImage),image,Sprite::createWithSpriteFrameName(disabledImage),callback);
	return item;
}


bool IsContinuous(std::vector<int>& vecPoker)//判断扑克牌是否连续
{
	bool ret = true;
	// 排序
	std::sort(vecPoker.begin(), vecPoker.end());
	// 排序完成后比较相邻的两个数字的差值
	for (int i = 0; i < vecPoker.size() - 1; i++){
		if (vecPoker[i + 1] - vecPoker[i] != 1){//如果不为1，则不连续
			ret = false;
			break;
		}
	}
	return ret;
}
bool IsContinuous(__Array* vecPoker)//判断扑克牌是否连续
{
	unsigned int length = vecPoker->count();//获取扑克牌数量
	std::vector<int> vec;

	for (int i = 0; i < length; i++)
	{
		Poker* poker = dynamic_cast<Poker*>(vecPoker->objectAtIndex(i));
		if (poker != NULL)
		{
			vec.push_back(poker->_info._num);//得到当前扑克牌的数值
		}
	}

	return IsContinuous(vec);
}

bool IsLess2(__Array* vecPoker) // 判断扑克牌是否都小于2
{
	bool ret =true;
	unsigned int length = vecPoker->count();//获取扑克牌数量
	for (int i = 0; i < length; i++)
	{
		Poker* poker = dynamic_cast<Poker*>(vecPoker->objectAtIndex(i));
		if (poker != NULL)
		{
			ret = poker->_info._num < 12;//如果当前牌小于2
			if (!ret)
				return ret;//说明这张牌大于2
		}
	}
	return ret;
}

bool IsLess2(std::vector<int>& vecPoker) // 判断扑克牌是否都小于2
{
	bool ret =true;
	for (int i = 0; i < vecPoker.size(); i++)
	{
		if (vecPoker[i] >= 12)//如果当前牌大于等于2
		{
			ret = false;//不都小于2
			break;
		}
	}
	return ret;
}

struct CRAD_INDEX
{
	std::vector<int>	single_index;	//记录单张的牌
	std::vector<int>	double_index;	//记录对子的牌
	std::vector<int>	three_index;	    //记录3张
	std::vector<int>	four_index;		//记录4张
};

CARDS_DATA JudgePaiXing(__Array* cards)//判断牌型
{
	std::vector<int> vec;
	for (int i = 0; i < cards->count(); i++){
		Poker* poker = dynamic_cast<Poker*>(cards->objectAtIndex(i));
		if (poker != NULL){
			vec.push_back(poker->_info._num);//获取扑克牌数值
		}
	}
	return JudgePaiXing(vec);
}

CARDS_DATA JudgePaiXing(std::vector<PokerInfo>& cards)
{
	std::vector<int> vec;
	for (int i = 0; i < cards.size(); i++){
		vec.push_back(cards[i]._num);
	}
	return JudgePaiXing(vec);
}


CARDS_DATA JudgePaiXing(std::vector<int>& cards)
{
	CARDS_DATA ret;
	unsigned int length = cards.size();
	std::sort(cards.begin(), cards.end());

	for (int i = 0; i < length; i++){
		ret._cards.push_back(cards[i]);
	}

	//小于5张牌
	//单牌，对子，3不带,炸弹通用算法
	if (length > 0 && length < 5)
	{
		// 单牌/对子/三不带/炸弹
		if (cards[0] == cards[length - 1])
		{
			ret._type = (CARD_TYPE)length;
			if (length == 4){
				ret._value = 100 + cards[0];
			}else{
				ret._value = (length - 1) * 20 + cards[0];
			}
			return ret;
		}

		// 火箭
		if (cards[0] == NUM_XW && cards[1] == NUM_DW && length == 2){
			ret._type = MISSILE_CARD;
			ret._value = 120;
			return ret;
		}

		// 三带一
		if ((cards[0] == cards[length - 2] && length == 4)|| (cards[1] == cards[length - 1] && length == 4)){
			ret._type = THREE_ONE_CARD;
			if ((cards[0] == cards[length - 2] && length == 4)){
				ret._value = 40 + cards[0];
			}else{
				ret._value = 40 + cards[1];
			}
			return ret;
		}
	}

	// 大于等于5张牌
	else if (length >= 5)
	{
		// 连牌
		if (IsContinuous(cards) && IsLess2(cards)){
			ret._type = CONNECT_CARD;
			ret._value = 60 + cards[0];
			return ret;
		}

		// 连对
		if (length >= 6 && length % 2 == 0) // 判断是否大于六张，且为双数
		{
			bool is_all_double = true;// 判断是否都是对子
			for (int i = 0; i < length; i += 2){
				if (cards[i] != cards[i + 1]){
					is_all_double = false;
					break;
				}
			}

			// 判断对子是否连续
			if (is_all_double){
				std::vector<int> vec_single;
				for (int i = 0; i < length; i += 2)
					vec_single.push_back(cards[i]);
				if (IsContinuous(vec_single))
				{
					ret._type = COMPANY_CARD;
					ret._value = 80 + cards[0];
					return ret;
				}
			}
		}

		// 将数组中的牌数分到结构体CRAD_INDEX中
		CRAD_INDEX card_index;

		for (int i = 0; i < length; ){
			if (i + 1 < length && cards[i] == cards[i + 1]){
				if (i + 2 < length && cards[i + 1] == cards[i + 2]){
					if (i + 3 < length && cards[i + 2] == cards[i + 3]){
						card_index.four_index.push_back(cards[i]);
						i += 4;
					}else{
						card_index.three_index.push_back(cards[i]);
						i += 3;
					}
				}
				else{
					card_index.double_index.push_back(cards[i]);
					i += 2;
				}
			}
			else{
				card_index.single_index.push_back(cards[i]);
				i++;
			}
		}

		// 3带对
		if (card_index.three_index.size() == 1 && card_index.double_index.size() == 1 && card_index.four_index.empty() && card_index.single_index.empty()){
			ret._type = THREE_TWO_CARD;
			ret._value = 40 + card_index.three_index[0];
			return ret;
		}

		// 飞机   前提：两个连续三张的
		if (card_index.four_index.empty() && card_index.three_index.size() == 2 && card_index.three_index[0] + 1 == card_index.three_index[1])
		{
			if (card_index.single_index.empty() && card_index.double_index.empty())// 333444
			{
				ret._type = AIRCRAFT_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
			if (card_index.double_index.empty() && card_index.single_index.size() == 2)// 33344456
			{
				ret._type = AIRCRAFT_SINGLE_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}	
			if (card_index.single_index.empty() && card_index.double_index.size() == 1)// 33344455
			{
				ret._type = AIRCRAFT_SINGLE_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
			if (card_index.single_index.empty() && card_index.double_index.size() == 2)// 3334445566
			{
				ret._type = AIRCRAFT_DOUBLE_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
		}

		// 4带2
		if (card_index.four_index.size() == 1 && length % 2 == 0 && card_index.three_index.empty())
		{
			if (card_index.single_index.size() == 2 && card_index.double_index.empty())// 444423
			{
				ret._type = BOMB_TWO_CARD;
				ret._value = 100 + cards[0];
				return ret;
			}			
			if (card_index.double_index.size() == 1 && card_index.single_index.empty())// 444422
			{
				ret._type = BOMB_TWO_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
			if (card_index.double_index.size() == 2 && card_index.single_index.empty())// 44442233
			{
				ret._type = BOMB_TWOOO_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
		}
	}

	ret._type = ERROR_CARD;
	ret._value = 0;
	return ret;
}