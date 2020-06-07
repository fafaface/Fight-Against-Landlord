#include "Player.h"
#include <Common.h>
#include "GameScene.h"

Player* Player::create(std::string name,  bool isHero)//int score,
{
	Player* sprite = new (std::nothrow) Player();
	if (sprite && sprite->init(name, isHero))//score, 
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Player::init(std::string name, bool isHero)//int score, 
{
	if (!Sprite::init())
	{
		return false;
	}

	_isHero = isHero;
	_name = name;
	_isLandlord= false;

	// ���ƹ���
	_cardsManager = Sprite::create();
	_cardsManager->setPosition(0, -190 / SCALE_FACTOR);
	this->addChild(_cardsManager, 1);

	// �������
	_labelName = Label::createWithTTF(_name, "fonts/FZCuYuan-M03S.ttf", 18 / SCALE_FACTOR);
	_labelName->setColor(Color3B(255, 255, 0));
	_labelName->setPosition(0, -70 / SCALE_FACTOR);
	this->addChild(_labelName, 1);

	// ���ͷ��
	auto avatorBg = Sprite::create("avator_bg.png");
	avatorBg->setScale(0.8);
	this->addChild(avatorBg, 0);

	char str_avator_image[255] = { 0 };
	sprintf(str_avator_image, "head/vtouxiang_%02d.png", rand() % 6 + 1);
	auto avator = Sprite::create(str_avator_image);
	this->addChild(avator, 0);

	// �������
	auto back = Sprite::create();
	back->setScale(0.3);
	back->setPosition(100 / SCALE_FACTOR, 0);
	_labelPokerCount = Label::createWithTTF("0", "fonts/FZCuYuan-M03S.ttf", 130 / SCALE_FACTOR);
	_labelPokerCount->setColor(Color3B(138, 43, 226));
	_labelPokerCount->setPosition(70 / SCALE_FACTOR, 110 / SCALE_FACTOR);
	back->addChild(_labelPokerCount, 1);
	this->addChild(back, 1);
	// ������
	_showZone = PokerShowZone::create();
	if (isHero)
		_showZone->setPosition(550 / SCALE_FACTOR, 130 / SCALE_FACTOR);
	else
		_showZone->setPosition(0 / SCALE_FACTOR, -150 / SCALE_FACTOR);
	this->addChild(_showZone, 1);

	return true;
}

void Player::updateCards()
{
	this->ChaiPai();

	std::stringstream text;
	text << _vecPokerInfo.size();//���˿�������ת��Ϊ�ַ�����
	_labelPokerCount->setString(text.str());//�����˿�������

	int count = _cardsManager->getChildren().size();
	int zeroPoint = count / 2;
	for (int i = 0; i < count; i++)
	{
		Poker* card = static_cast<Poker*>(_cardsManager->getChildren().at(i));
		//dynamic_cast��Ҫ�������μ������ת��������ת����������������֮��Ľ���ת��
		if (card != NULL)
		{
			card->setPosition(550/ SCALE_FACTOR + (i - zeroPoint) * 50 / SCALE_FACTOR, card->getPosition().y);
		}
	}
}

void Player::clearCards()//���������ʾ����
{
	_showZone->removeAllChildrenWithCleanup(true);
}

void Player::setLandlord()//���ø����Ϊ����
{
	_isLandlord = true;
	if (_isLandlord)	// ������
	{
		auto lord = Sprite::create("flag_lord.png");//��ӵ�����ʾ
		lord->setPosition(160 / SCALE_FACTOR, 0 / SCALE_FACTOR);
		this->addChild(lord, 0);
	}
}

bool Player::getIsLandlord()//�ж������ҵ����
{
	return _isLandlord;
}

void Player::DealCards(GameScene* scene, PokerInfo info)
{
	_vecPokerInfo.push_back(info);

	if (_isHero)
	{
		//��Ӿ���
		auto card = Poker::create(info, !_isHero);
		_cardsManager->addChild(card, 100 - info._num);
		card->SetTouchEnabled();
		card->setGameMain(scene);
	}

	_cardsManager->sortAllChildren();

	updateCards();
}

void Player::OutCards(GameScene* scene, bool isFollow, CARD_TYPE cardtype, unsigned int count, unsigned int value)
{
	clearCards();
	_vecOutCards.clear();

	if (_isHero)
	{
		for (auto it = _cardsManager->getChildren().begin(); it != _cardsManager->getChildren().end(); it++)
		{
			Poker* card = dynamic_cast<Poker*>(*it);
			if (card != NULL && card->isSelected())
			{
				_vecOutCards.push_back(card->getInfo());
			}
		}

		// ��������ɾ��Ҫ������
		for (int j = 0; j < _vecOutCards.size(); j++)
		{
			for (auto it = _cardsManager->getChildren().begin(); it != _cardsManager->getChildren().end(); it++)
			{
				Poker* card = dynamic_cast<Poker*>(*it);
				if (card != NULL && card->getInfo() == _vecOutCards.at(j))
				{
					_vecPokerInfo.erase(std::find(_vecPokerInfo.begin(), _vecPokerInfo.end(), card->getInfo()));
					_cardsManager->removeChild(card, true);
					break;
				}
			}
		}
	}
	else
	{
		std::vector<int> vec = isFollow ? FindFollowCards(cardtype, count, value) : FindOutCards();
		if (vec.empty())
		{
			_showZone->chupai(_vecOutCards);
		}
		else
		{
			for (int i = 0; i < vec.size(); i++)
			{
				PokerInfo info;
				info._num = (PokerNum)vec[i];
				info._tag = (PokerTag)0;	// ��Ϊ��ɫ��Ӱ����ֵ�Ĵ�С��������ʱ���ţ������ҵ����ٸ�ֵ
				_vecOutCards.push_back(info);
			}

			// ��������ɾ��Ҫ������
			for (int j = 0; j < _vecOutCards.size(); j++)
			{
				for (int i = 0; i != _vecPokerInfo.size(); i++)
				{
					if (_vecOutCards.at(j)._num == _vecPokerInfo.at(i)._num)
					{
						_vecOutCards.at(j)._tag = _vecPokerInfo.at(i)._tag;	// ���ƵĻ�ɫ��ֵ
						_vecPokerInfo.erase(std::find(_vecPokerInfo.begin(), _vecPokerInfo.end(), _vecOutCards.at(j)));
						break;
					}
				}

				for (auto it = _cardsManager->getChildren().begin(); it != _cardsManager->getChildren().end(); it++)
				{
					Poker* card = dynamic_cast<Poker*>(*it);
					if (card != NULL && card->getInfo()._num == _vecOutCards.at(j)._num)
					{
						_cardsManager->removeChild(card, true);
						break;
					}
				}
			}
		}
	}

	CARDS_DATA vecOutCardsData = JudgePaiXing(_vecOutCards);
	if ((isFollow)&&(vecOutCardsData._type == BOMB_CARD || vecOutCardsData._type == MISSILE_CARD)){
			std::vector<int> vec;
			for (int i = 0; i < _vecOutCards.size(); i++)
			{
				vec.push_back(_vecOutCards[i]._num);
			}
	}
	
	else
	{
		std::vector<int> vec;
		for (int i = 0; i < _vecOutCards.size(); i++)
		{
			vec.push_back(_vecOutCards[i]._num);
		}
	}
	_showZone->chupai(_vecOutCards);

	updateCards();

	if (_vecPokerInfo.empty())	// ʤ��
	{
		scene->GameOver(_id);
	}
}

void Player::BuChu()// ����
{
	clearCards();
	_vecOutCards.clear();//��������½��ϼҳ�����

	for (int i = 0; i < _cardsManager->getChildren().size(); i++)
	{
		Poker* card = dynamic_cast<Poker*>(_cardsManager->getChildren().at(i));
		if (card != NULL)
		{
			card->unSelect();
		}
	}
}

void Player::ChaiPai()
{
	// ��ֵ

//			��С�ƻ���ֵ	ÿ��һ���һ
// ����		0	+	1
// ����		20	+	1
// ����		40	+	1
// ��˳		60	+	1
// ˫˳		80	+	1
// ը��		100	+	1
// ���		120


	_allCardGroups.clear();

	std::vector<int> vec_poke;//�˿�����
	for (int i = 0; i < _vecPokerInfo.size(); i++)
	{
		vec_poke.push_back(_vecPokerInfo[i]._num);//����ǰ�����е������˿˵���ֵ�����������
	}

	std::sort(vec_poke.begin(), vec_poke.end());//����

	// �����㷨

	// 4����������
	std::vector<int> card_i;
	std::vector<int> card_ii;
	std::vector<int> card_iii;
	std::vector<int> card_iiii;

	// 1.���������Ʒֵ��ĸ���������
	for (int i = 0; i < vec_poke.size(); i++)
	{
		// ��һ������
		auto itor_find_i = std::find(card_i.begin(), card_i.end(), vec_poke[i]);
		// �ҵ���
		if (itor_find_i != card_i.end())
		{
			// �ڶ�������
			auto itor_find_ii = std::find(card_ii.begin(), card_ii.end(), vec_poke[i]);
			// �ҵ���
			if (itor_find_ii != card_ii.end())
			{
				// ����������
				auto itor_find_iii = std::find(card_iii.begin(), card_iii.end(), vec_poke[i]);
				// �ҵ���
				if (itor_find_iii != card_iii.end())
				{
					// ���ĸ�����
					card_iiii.push_back(vec_poke[i]);
				}
				// û�ҵ�
				else
				{
					card_iii.push_back(vec_poke[i]);
				}
			}
			// û�ҵ�
			else
			{
				card_ii.push_back(vec_poke[i]);
			}
		}
		// û�ҵ�
		else
		{
			card_i.push_back(vec_poke[i]);
		}
		vec_poke[i];
	}

	std::sort(card_i.begin(), card_i.end());
	std::sort(card_ii.begin(), card_ii.end());
	std::sort(card_iii.begin(), card_iii.end());
	std::sort(card_iiii.begin(), card_iiii.end());

	// 2.���ĸ������������ж�����

	// ���
	auto itor_xw = std::find(card_i.begin(), card_i.end(), NUM_XW);
	auto itor_dw = std::find(card_i.begin(), card_i.end(), NUM_DW);
	if (itor_xw != card_i.end() && itor_dw != card_i.end())
	{
		CARDS_DATA temp;
		temp._cards.push_back(NUM_XW);
		temp._cards.push_back(NUM_DW);
		temp._type = MISSILE_CARD;
		temp._value = 120;

		_allCardGroups.push_back(temp);
	}

	// ���ĸ������ж���ը��
	for (int i = 0; i < card_iiii.size(); i++)
	{
		CARDS_DATA temp;
		temp._cards.push_back(card_iiii[i]);
		temp._cards.push_back(card_iiii[i]);
		temp._cards.push_back(card_iiii[i]);
		temp._cards.push_back(card_iiii[i]);
		temp._type = BOMB_CARD;
		temp._value = 100 + card_iiii[i];

		_allCardGroups.push_back(temp);
	}

	// �����������ж���������ͬ�ģ�������������һ��������
	for (int i = 0; i < card_iii.size(); i++)
	{
		CARDS_DATA temp;
		temp._cards.push_back(card_iii[i]);
		temp._cards.push_back(card_iii[i]);
		temp._cards.push_back(card_iii[i]);
		temp._type = THREE_CARD;
		temp._value = 40 + card_iii[i];

		_allCardGroups.push_back(temp);
	}

	// ����
	for (int i = 0; i < card_ii.size(); i++)
	{
		CARDS_DATA temp;
		temp._cards.push_back(card_ii[i]);
		temp._cards.push_back(card_ii[i]);
		temp._type = DOUBLE_CARD;
		temp._value = 20 + card_ii[i];

		_allCardGroups.push_back(temp);
	}

	// �ҵ���(�ڵ�һ�����飬�����ڵڶ������飬��������ɵ�˳)

	// ��˳
	for (int i = 0; i < card_i.size(); i++)
	{
		bool is_in_link = false;
		for (int j = 4; j < card_i.size(); j++)
		{
			if (i + j < card_i.size() && card_i[i + j] - card_i[i] == j && card_i[i + j] <= NUM_A/*��˳���ܴ��A*/)
			{
				CARDS_DATA temp;
				temp._type = CONNECT_CARD;
				temp._value = 60 + card_i[i];
				for (int k = 0; k <= j; k++)
				{
					temp._cards.push_back(card_i[i + k]);
				}
				_allCardGroups.push_back(temp);

				is_in_link = true;
			}
		}


		CARDS_DATA temp;
		temp._cards.push_back(card_i[i]);
		temp._type = SINGLE_CARD;
		temp._value = 0 + card_i[i];

		_allCardGroups.push_back(temp);
	}

	// ˫˳
	for (int i = 0; i < card_ii.size(); i++)
	{
		for (int j = 2; j < card_ii.size(); j++)
		{
			if (i + j < card_ii.size() && card_ii[i + j] - card_ii[i] == j && card_ii[i + j] <= NUM_A/*��˳���ܴ��A*/)
			{
				CARDS_DATA temp;
				temp._type = COMPANY_CARD;
				temp._value = 80 + card_ii[i];
				for (int k = 0; k <= j; k++)
				{
					temp._cards.push_back(card_ii[i + k]);
					temp._cards.push_back(card_ii[i + k]);
				}
				_allCardGroups.push_back(temp);
			}
		}
	}
}

void Player::ShowTipInfo(bool isFollow, CARD_TYPE cardType, unsigned int count, unsigned int value)
{

}

bool Player::IsQiangLandlord()//����ѡ���Ƿ�������
{
	return rand() % 2;
}

std::string Player::GetName()
{
	return _name;
}

void Player::SetPlayerID(size_t id)
{
	_id = id;
}

size_t Player::GetPlayerID()
{
	return _id;
}

std::vector<int>& Player::FindFollowCards(CARD_TYPE cardType, unsigned int count, unsigned int value)
{
	// TODO: �ڴ˴����� return ���
	_vecFindFollowCards.clear();

	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		// ���ж���ֵ�������ֵСֱ����һ��
		if (_allCardGroups[i]._value <= value)
			continue;

		// ����/����/������/ը��/��˳/˫˳/�ɻ�/���
		if (cardType == _allCardGroups[i]._type)
		{
			if (cardType == CONNECT_CARD || cardType == COMPANY_CARD || cardType == AIRCRAFT_CARD)
			{
				if (count == _allCardGroups[i]._cards.size())
				{
					return _allCardGroups[i]._cards;
				}
			}
			else
			{
				return _allCardGroups[i]._cards;
			}
		}
		// ����һ/����һ��
		else
		{
			// ����ϼҳ���Ϊ3��һ
			if (cardType == THREE_ONE_CARD)
			{
				if (_allCardGroups[i]._type == THREE_CARD)
				{
					for (int j = 0; j < _allCardGroups.size(); j++)
					{
						if (_allCardGroups[j]._type == SINGLE_CARD)
						{
							_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[j]._cards[0]);
							return _vecFindFollowCards;
						}
					}
				}
			}
			// ����ϼҳ���Ϊ3��һ��
			else if (cardType == THREE_TWO_CARD)
			{
				if (_allCardGroups[i]._type == THREE_CARD)
				{
					for (int j = 0; j < _allCardGroups.size(); j++)
					{
						if (_allCardGroups[j]._type == DOUBLE_CARD && _allCardGroups[i]._cards[0] != _allCardGroups[j]._cards[0])
						{
							_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[j]._cards[0]);
							_vecFindFollowCards.push_back(_allCardGroups[j]._cards[1]);
							return _vecFindFollowCards;
						}
					}
				}
			}
		}
	}

	// û�ҵ���Ӧ�����Ͳ�����ֵ�����ϼҵģ�ʹ��ը��
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		// ���ж���ֵ�������ֵСֱ����һ��
		if (_allCardGroups[i]._value <= value)
			continue;

		if (BOMB_CARD == _allCardGroups[i]._type)
			return _allCardGroups[i]._cards;
	}

	// û�ҵ���Ӧ�����Ͳ�����ֵ�����ϼҵģ�ʹ�û������ը��
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (MISSILE_CARD == _allCardGroups[i]._type)
			return _allCardGroups[i]._cards;
	}

	//std::vector<int> tmp;
	return _vecFindFollowCards;
}

std::vector<int>& Player::FindOutCards()
{
	// TODO: �ڴ˴����� return ���
	_vecFindFollowCards.clear();

	// ��ͬ�������ȼ�: ˫˳ > ��˳ > ���� > ���� > ���� > ը�� > ���

	//˫˳
	int index_company_card = -1;
	int count_company_card = 0;
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == COMPANY_CARD && count_company_card < _allCardGroups[i]._cards.size())
		{
			count_company_card = _allCardGroups[i]._cards.size();
			index_company_card = i;
		}
	}
	if (index_company_card != -1)
	{
		return _allCardGroups[index_company_card]._cards;
	}

	//��˳
	int index_connect_card = -1;
	int count_connect_card = 0;
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == CONNECT_CARD && count_connect_card < _allCardGroups[i]._cards.size())
		{
			count_connect_card = _allCardGroups[i]._cards.size();
			index_connect_card = i;
		}
	}
	if (index_connect_card != -1)
	{
		return _allCardGroups[index_connect_card]._cards;
	}

	//������/����һ/����һ��
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == THREE_CARD)
		{
			for (int j = 0; j < _allCardGroups.size(); j++)
			{
				if (_allCardGroups[j]._type == SINGLE_CARD)
				{
					_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
					_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
					_vecFindFollowCards.push_back(_allCardGroups[i]._cards[0]);
					_vecFindFollowCards.push_back(_allCardGroups[j]._cards[0]);
					return _vecFindFollowCards;
				}
			}

			return _allCardGroups[i]._cards;
		}
	}

	//����
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == DOUBLE_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	//����
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == SINGLE_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	//ը��
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == BOMB_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	//���
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == MISSILE_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	std::vector<int> tmp;
	return tmp;
}

std::vector<PokerInfo>& Player::GetOutCards()
{
	// TODO: �ڴ˴����� return ���
	return _vecOutCards;
}
