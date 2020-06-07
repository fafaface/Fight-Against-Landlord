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

	// 手牌管理
	_cardsManager = Sprite::create();
	_cardsManager->setPosition(0, -190 / SCALE_FACTOR);
	this->addChild(_cardsManager, 1);

	// 玩家名称
	_labelName = Label::createWithTTF(_name, "fonts/FZCuYuan-M03S.ttf", 18 / SCALE_FACTOR);
	_labelName->setColor(Color3B(255, 255, 0));
	_labelName->setPosition(0, -70 / SCALE_FACTOR);
	this->addChild(_labelName, 1);

	// 玩家头像
	auto avatorBg = Sprite::create("avator_bg.png");
	avatorBg->setScale(0.8);
	this->addChild(avatorBg, 0);

	char str_avator_image[255] = { 0 };
	sprintf(str_avator_image, "head/vtouxiang_%02d.png", rand() % 6 + 1);
	auto avator = Sprite::create(str_avator_image);
	this->addChild(avator, 0);

	// 玩家牌数
	auto back = Sprite::create();
	back->setScale(0.3);
	back->setPosition(100 / SCALE_FACTOR, 0);
	_labelPokerCount = Label::createWithTTF("0", "fonts/FZCuYuan-M03S.ttf", 130 / SCALE_FACTOR);
	_labelPokerCount->setColor(Color3B(138, 43, 226));
	_labelPokerCount->setPosition(70 / SCALE_FACTOR, 110 / SCALE_FACTOR);
	back->addChild(_labelPokerCount, 1);
	this->addChild(back, 1);
	// 出牌区
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
	text << _vecPokerInfo.size();//将扑克牌数量转化为字符数字
	_labelPokerCount->setString(text.str());//设置扑克牌数量

	int count = _cardsManager->getChildren().size();
	int zeroPoint = count / 2;
	for (int i = 0; i < count; i++)
	{
		Poker* card = static_cast<Poker*>(_cardsManager->getChildren().at(i));
		//dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换
		if (card != NULL)
		{
			card->setPosition(550/ SCALE_FACTOR + (i - zeroPoint) * 50 / SCALE_FACTOR, card->getPosition().y);
		}
	}
}

void Player::clearCards()//清除所有显示的牌
{
	_showZone->removeAllChildrenWithCleanup(true);
}

void Player::setLandlord()//设置该玩家为地主
{
	_isLandlord = true;
	if (_isLandlord)	// 玩家身份
	{
		auto lord = Sprite::create("flag_lord.png");//添加地主表示
		lord->setPosition(160 / SCALE_FACTOR, 0 / SCALE_FACTOR);
		this->addChild(lord, 0);
	}
}

bool Player::getIsLandlord()//判断这个玩家的身份
{
	return _isLandlord;
}

void Player::DealCards(GameScene* scene, PokerInfo info)
{
	_vecPokerInfo.push_back(info);

	if (_isHero)
	{
		//添加精灵
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

		// 从手牌中删除要出的牌
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
				info._tag = (PokerTag)0;	// 因为花色不影响牌值的大小，所以暂时空着，下面找到牌再赋值
				_vecOutCards.push_back(info);
			}

			// 从手牌中删除要出的牌
			for (int j = 0; j < _vecOutCards.size(); j++)
			{
				for (int i = 0; i != _vecPokerInfo.size(); i++)
				{
					if (_vecOutCards.at(j)._num == _vecPokerInfo.at(i)._num)
					{
						_vecOutCards.at(j)._tag = _vecPokerInfo.at(i)._tag;	// 给牌的花色赋值
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

	if (_vecPokerInfo.empty())	// 胜利
	{
		scene->GameOver(_id);
	}
}

void Player::BuChu()// 不出
{
	clearCards();
	_vecOutCards.clear();//清除了左下角上家出的牌

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
	// 牌值

//			最小牌基础值	每大一点加一
// 单张		0	+	1
// 对子		20	+	1
// 三带		40	+	1
// 单顺		60	+	1
// 双顺		80	+	1
// 炸弹		100	+	1
// 火箭		120


	_allCardGroups.clear();

	std::vector<int> vec_poke;//扑克数组
	for (int i = 0; i < _vecPokerInfo.size(); i++)
	{
		vec_poke.push_back(_vecPokerInfo[i]._num);//将当前手牌中的所有扑克的数值放入该数组中
	}

	std::sort(vec_poke.begin(), vec_poke.end());//排序

	// 拆牌算法

	// 4个辅助数组
	std::vector<int> card_i;
	std::vector<int> card_ii;
	std::vector<int> card_iii;
	std::vector<int> card_iiii;

	// 1.将所有手牌分到四个辅助数组
	for (int i = 0; i < vec_poke.size(); i++)
	{
		// 第一个数组
		auto itor_find_i = std::find(card_i.begin(), card_i.end(), vec_poke[i]);
		// 找到了
		if (itor_find_i != card_i.end())
		{
			// 第二个数组
			auto itor_find_ii = std::find(card_ii.begin(), card_ii.end(), vec_poke[i]);
			// 找到了
			if (itor_find_ii != card_ii.end())
			{
				// 第三个数组
				auto itor_find_iii = std::find(card_iii.begin(), card_iii.end(), vec_poke[i]);
				// 找到了
				if (itor_find_iii != card_iii.end())
				{
					// 第四个数组
					card_iiii.push_back(vec_poke[i]);
				}
				// 没找到
				else
				{
					card_iii.push_back(vec_poke[i]);
				}
			}
			// 没找到
			else
			{
				card_ii.push_back(vec_poke[i]);
			}
		}
		// 没找到
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

	// 2.从四个辅助数组中判断牌型

	// 火箭
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

	// 第四个数组中都是炸弹
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

	// 第三个数组中都是三张相同的，三不带，三带一，三带对
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

	// 对子
	for (int i = 0; i < card_ii.size(); i++)
	{
		CARDS_DATA temp;
		temp._cards.push_back(card_ii[i]);
		temp._cards.push_back(card_ii[i]);
		temp._type = DOUBLE_CARD;
		temp._value = 20 + card_ii[i];

		_allCardGroups.push_back(temp);
	}

	// 找单牌(在第一个数组，而不在第二个数组，并不能组成单顺)

	// 单顺
	for (int i = 0; i < card_i.size(); i++)
	{
		bool is_in_link = false;
		for (int j = 4; j < card_i.size(); j++)
		{
			if (i + j < card_i.size() && card_i[i + j] - card_i[i] == j && card_i[i + j] <= NUM_A/*单顺不能打过A*/)
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

	// 双顺
	for (int i = 0; i < card_ii.size(); i++)
	{
		for (int j = 2; j < card_ii.size(); j++)
		{
			if (i + j < card_ii.size() && card_ii[i + j] - card_ii[i] == j && card_ii[i + j] <= NUM_A/*单顺不能打过A*/)
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

bool Player::IsQiangLandlord()//机器选择是否抢地主
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
	// TODO: 在此处插入 return 语句
	_vecFindFollowCards.clear();

	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		// 先判断牌值，如果牌值小直接下一轮
		if (_allCardGroups[i]._value <= value)
			continue;

		// 单张/对子/三不带/炸弹/单顺/双顺/飞机/火箭
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
		// 三带一/三带一对
		else
		{
			// 如果上家出牌为3带一
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
			// 如果上家出牌为3带一对
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

	// 没找到对应的牌型并且牌值大于上家的，使用炸弹
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		// 先判断牌值，如果牌值小直接下一轮
		if (_allCardGroups[i]._value <= value)
			continue;

		if (BOMB_CARD == _allCardGroups[i]._type)
			return _allCardGroups[i]._cards;
	}

	// 没找到对应的牌型并且牌值大于上家的，使用火箭（王炸）
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
	// TODO: 在此处插入 return 语句
	_vecFindFollowCards.clear();

	// 不同牌组优先级: 双顺 > 单顺 > 三带 > 对子 > 单牌 > 炸弹 > 火箭

	//双顺
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

	//单顺
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

	//三不带/三带一/三带一对
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

	//对子
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == DOUBLE_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	//单牌
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == SINGLE_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	//炸弹
	for (int i = 0; i < _allCardGroups.size(); i++)
	{
		if (_allCardGroups[i]._type == BOMB_CARD)
		{
			return _allCardGroups[i]._cards;
		}
	}

	//火箭
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
	// TODO: 在此处插入 return 语句
	return _vecOutCards;
}
