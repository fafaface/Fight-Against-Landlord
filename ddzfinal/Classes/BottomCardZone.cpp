#include "BottomCardZone.h"

USING_NS_CC;
//��������
BottomCardZone* BottomCardZone::create()//��������������
{
	BottomCardZone* sprite = new (std::nothrow) BottomCardZone();//������ָ��
	if (sprite && sprite->init())
	{
		sprite->autorelease();//�����Զ��ͷ���Դ����
		return sprite;//���ص��ƾ�����ָ��
	}
	CC_SAFE_DELETE(sprite);//����ʧ��
	return nullptr;
}

bool BottomCardZone::init()	// ��ʼ������������
{
	if (!Sprite::init())
	{
		return false;//����ʧ��
	}
	setScale(0.5);//���ô�С
	auto bg = Sprite::create("bottomCardZone.png");//���õ������򱳾�ͼƬ��������
	this->addChild(bg);//��ӵ���ǰ������
	return true;
}

void BottomCardZone::Show(PokerInfo cardInfo)//ѡ��������,�������
{
	auto card = Poker::create(cardInfo);//��Ӿ���
	card->setScale(0.7);
	this->addChild(card, 100 - cardInfo._num);//��ӵ���
	sortAllChildren();//����
	int count = _children.size();
	int zeroPoint = count / 2;
	for (int i = 0; i < _children.size(); i++)
	{
		Poker* card = dynamic_cast<Poker*>(_children.at(i));//ת��Ϊ�˿�������
		if (card != NULL)
		{
			card->setPosition((i - zeroPoint) * 110, 0);//���õ��Ƶ�λ��,ÿ�ζ�Ҫ�ƶ�һ�ξ���
		}
	}
}