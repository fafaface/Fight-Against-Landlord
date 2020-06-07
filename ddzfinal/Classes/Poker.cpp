#include "Poker.h"
#include "Array.h"
#include "GameScene.h"

#define SCALE_FACTOR 0.625f //位置规模因子
USING_NS_CC;

char* poker_text[] = {
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"J",
	"Q",
	"K",
	"A",
	"2",
	"w",
	"W",
};

Poker* Poker::create(PokerInfo info, bool isSmall)
{
	Poker* sprite = new(std::nothrow)Poker();
	if (sprite && sprite->init(info, isSmall))
	{
		sprite->autorelease();
		return sprite;
	}
	return nullptr;
}

bool Poker::init(PokerInfo info, bool isSmall)
{
	if (!Sprite::init())
	{
		return false;
	}

	_info = info;
	//设置扑克牌图案
	if (isSmall)
	{
		auto cardFront = Label::createWithSystemFont(poker_text[_info._num], "arial", 24);
		this->addChild(cardFront);
		_size = cardFront->getContentSize();
	}
	else
	{
		auto cardFront = Sprite::createWithSpriteFrameName("b/bg.png");
		this->addChild(cardFront);
		_size = cardFront->getContentSize();

		if (_info._num == PokerNum::NUM_XW)
		{
			auto cardNum = Sprite::createWithSpriteFrameName("b/smalltag_4.png");
			cardNum->setPosition(-50 / SCALE_FACTOR, 10 / SCALE_FACTOR);
			this->addChild(cardNum);

			auto cardTag = Sprite::createWithSpriteFrameName("b/bigtag_6.png");
			cardTag->setPosition(20 / SCALE_FACTOR, -30 / SCALE_FACTOR);
			this->addChild(cardTag);
		}
		else if (_info._num == PokerNum::NUM_DW)
		{
			auto cardNum = Sprite::createWithSpriteFrameName("b/smalltag_5.png");
			cardNum->setPosition(-50 / SCALE_FACTOR, 10 / SCALE_FACTOR);
			this->addChild(cardNum);

			auto cardTag = Sprite::createWithSpriteFrameName("b/bigtag_6.png");
			cardTag->setPosition(20 / SCALE_FACTOR, -30 / SCALE_FACTOR);
			this->addChild(cardTag);
		}
		else
		{
			unsigned int num = 0;
			if (_info._num == NUM_A)
				num = 0;
			else if (_info._num == NUM_2)
				num = 1;
			else
				num = _info._num + 2;

			std::stringstream strTag;
			strTag << "b/bigtag_" << _info._tag << ".png";
			auto cardSmallTag = Sprite::createWithSpriteFrameName(strTag.str());
			cardSmallTag->setScale(0.5);
			cardSmallTag->setPosition(-50 / SCALE_FACTOR, 20 / SCALE_FACTOR);
			this->addChild(cardSmallTag);

			if (_info._tag == HEITAO || _info._tag == MEIHUA)
			{
				std::stringstream strNum;
				strNum << "b/black_" << num << ".png";
				auto cardNum = Sprite::createWithSpriteFrameName(strNum.str());
				cardNum->setPosition(-50 / SCALE_FACTOR, 70 / SCALE_FACTOR);
				this->addChild(cardNum);
			}
			else
			{
				std::stringstream strNum;
				strNum << "b/red_" << num << ".png";
				auto cardNum = Sprite::createWithSpriteFrameName(strNum.str());
				cardNum->setPosition(-50 / SCALE_FACTOR, 70 / SCALE_FACTOR);
				this->addChild(cardNum);
			}
			auto cardTag = Sprite::createWithSpriteFrameName(strTag.str());
			cardTag->setPosition(20 / SCALE_FACTOR, -30 / SCALE_FACTOR);
			this->addChild(cardTag);
		}
	}

	_isSelected = false;
	return true;
}

void Poker::SetTouchEnabled()// 触摸事件
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Poker::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Poker::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Poker::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Poker::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void Poker::click()
{
	__Array* arr = _gameMain->getArrPlayerOut();
	//arr->retain();
	if (_isSelected)
	{
		setPosition(_position.x, _position.y - 20 / SCALE_FACTOR);
		arr->removeObject(this);
	}
	else
	{
		setPosition(_position.x, _position.y + 20 / SCALE_FACTOR);
		arr->addObject(this);
	}
	_isSelected = !_isSelected;
}

bool Poker::isSelected()
{
	return _isSelected;
}

void Poker::unSelect()
{
	__Array* arr = _gameMain->getArrPlayerOut();
	//arr->retain();
	if (_isSelected)
	{
		setPosition(_position.x, _position.y - 20 / SCALE_FACTOR);
		arr->removeObject(this);
	}
	_isSelected = false;
}

void Poker::Select()
{
	__Array* arr = _gameMain->getArrPlayerOut();
//	arr->retain();
	if (!_isSelected)
	{
		setPosition(_position.x, _position.y + 20 / SCALE_FACTOR);
		arr->addObject(this);
	}
	_isSelected = true;
}

PokerInfo Poker::getInfo()
{
	return _info;
}

Rect Poker::getRect()
{
	return Rect(-_size.width / 2, -_size.height / 2, _size.width, _size.height);
}

//触摸监听
bool Poker::onTouchBegan(Touch* touch, Event* event)
{
	if (getRect().containsPoint(convertTouchToNodeSpaceAR(touch)))
	{
		click();
		return true;
	}
	return false;
}

void Poker::onTouchEnded(Touch* touch, Event* event)
{
}

void Poker::onTouchCancelled(Touch* touch, Event* event)
{
}

void Poker::onTouchMoved(Touch* touch,Event* event)
{
}

