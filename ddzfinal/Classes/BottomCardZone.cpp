#include "BottomCardZone.h"

USING_NS_CC;
//底牌区域
BottomCardZone* BottomCardZone::create()//创建底牌区域类
{
	BottomCardZone* sprite = new (std::nothrow) BottomCardZone();//创建类指针
	if (sprite && sprite->init())
	{
		sprite->autorelease();//放入自动释放资源池中
		return sprite;//返回底牌精灵类指针
	}
	CC_SAFE_DELETE(sprite);//创建失败
	return nullptr;
}

bool BottomCardZone::init()	// 初始化底牌区域类
{
	if (!Sprite::init())
	{
		return false;//创建失败
	}
	setScale(0.5);//设置大小
	auto bg = Sprite::create("bottomCardZone.png");//利用底牌区域背景图片创建精灵
	this->addChild(bg);//添加到当前场景中
	return true;
}

void BottomCardZone::Show(PokerInfo cardInfo)//选定地主后,加入底牌
{
	auto card = Poker::create(cardInfo);//添加精灵
	card->setScale(0.7);
	this->addChild(card, 100 - cardInfo._num);//添加底牌
	sortAllChildren();//排序
	int count = _children.size();
	int zeroPoint = count / 2;
	for (int i = 0; i < _children.size(); i++)
	{
		Poker* card = dynamic_cast<Poker*>(_children.at(i));//转化为扑克牌类型
		if (card != NULL)
		{
			card->setPosition((i - zeroPoint) * 110, 0);//设置底牌的位置,每次都要移动一段距离
		}
	}
}