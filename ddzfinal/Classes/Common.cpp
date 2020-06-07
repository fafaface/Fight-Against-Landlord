#include "Common.h"
#include "Poker.h"
USING_NS_CC;

MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	auto image = Sprite::create(normalImage);//����ͼƬ��������
	image->setColor(Color3B(100, 100, 100));//������ɫ
	auto item = MenuItemSprite::create(Sprite::create(normalImage),image,Sprite::create(disabledImage),callback);//�����Ƿ���ʱ���������ͼƬ
	return item;
}

cocos2d::MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback callback)
{
	auto image = Sprite::createWithSpriteFrameName(normalImage);
	image->setColor(Color3B(100, 100, 100));
	auto item = MenuItemSprite::create(Sprite::createWithSpriteFrameName(normalImage),image,Sprite::createWithSpriteFrameName(disabledImage),callback);
	return item;
}


bool IsContinuous(std::vector<int>& vecPoker)//�ж��˿����Ƿ�����
{
	bool ret = true;
	// ����
	std::sort(vecPoker.begin(), vecPoker.end());
	// ������ɺ�Ƚ����ڵ��������ֵĲ�ֵ
	for (int i = 0; i < vecPoker.size() - 1; i++){
		if (vecPoker[i + 1] - vecPoker[i] != 1){//�����Ϊ1��������
			ret = false;
			break;
		}
	}
	return ret;
}
bool IsContinuous(__Array* vecPoker)//�ж��˿����Ƿ�����
{
	unsigned int length = vecPoker->count();//��ȡ�˿�������
	std::vector<int> vec;

	for (int i = 0; i < length; i++)
	{
		Poker* poker = dynamic_cast<Poker*>(vecPoker->objectAtIndex(i));
		if (poker != NULL)
		{
			vec.push_back(poker->_info._num);//�õ���ǰ�˿��Ƶ���ֵ
		}
	}

	return IsContinuous(vec);
}

bool IsLess2(__Array* vecPoker) // �ж��˿����Ƿ�С��2
{
	bool ret =true;
	unsigned int length = vecPoker->count();//��ȡ�˿�������
	for (int i = 0; i < length; i++)
	{
		Poker* poker = dynamic_cast<Poker*>(vecPoker->objectAtIndex(i));
		if (poker != NULL)
		{
			ret = poker->_info._num < 12;//�����ǰ��С��2
			if (!ret)
				return ret;//˵�������ƴ���2
		}
	}
	return ret;
}

bool IsLess2(std::vector<int>& vecPoker) // �ж��˿����Ƿ�С��2
{
	bool ret =true;
	for (int i = 0; i < vecPoker.size(); i++)
	{
		if (vecPoker[i] >= 12)//�����ǰ�ƴ��ڵ���2
		{
			ret = false;//����С��2
			break;
		}
	}
	return ret;
}

struct CRAD_INDEX
{
	std::vector<int>	single_index;	//��¼���ŵ���
	std::vector<int>	double_index;	//��¼���ӵ���
	std::vector<int>	three_index;	    //��¼3��
	std::vector<int>	four_index;		//��¼4��
};

CARDS_DATA JudgePaiXing(__Array* cards)//�ж�����
{
	std::vector<int> vec;
	for (int i = 0; i < cards->count(); i++){
		Poker* poker = dynamic_cast<Poker*>(cards->objectAtIndex(i));
		if (poker != NULL){
			vec.push_back(poker->_info._num);//��ȡ�˿�����ֵ
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

	//С��5����
	//���ƣ����ӣ�3����,ը��ͨ���㷨
	if (length > 0 && length < 5)
	{
		// ����/����/������/ը��
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

		// ���
		if (cards[0] == NUM_XW && cards[1] == NUM_DW && length == 2){
			ret._type = MISSILE_CARD;
			ret._value = 120;
			return ret;
		}

		// ����һ
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

	// ���ڵ���5����
	else if (length >= 5)
	{
		// ����
		if (IsContinuous(cards) && IsLess2(cards)){
			ret._type = CONNECT_CARD;
			ret._value = 60 + cards[0];
			return ret;
		}

		// ����
		if (length >= 6 && length % 2 == 0) // �ж��Ƿ�������ţ���Ϊ˫��
		{
			bool is_all_double = true;// �ж��Ƿ��Ƕ���
			for (int i = 0; i < length; i += 2){
				if (cards[i] != cards[i + 1]){
					is_all_double = false;
					break;
				}
			}

			// �ж϶����Ƿ�����
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

		// �������е������ֵ��ṹ��CRAD_INDEX��
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

		// 3����
		if (card_index.three_index.size() == 1 && card_index.double_index.size() == 1 && card_index.four_index.empty() && card_index.single_index.empty()){
			ret._type = THREE_TWO_CARD;
			ret._value = 40 + card_index.three_index[0];
			return ret;
		}

		// �ɻ�   ǰ�᣺�����������ŵ�
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

		// 4��2
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