#ifndef __BOTTOM_CARD_ZONE_H__
#define __BOTTOM_CARD_ZONE_H__
#include "cocos2d.h"
#include "Poker.h"
//��ʾ�������� �̳�cocos2d�ľ�����
class BottomCardZone :public cocos2d::Sprite
{
public:
	static BottomCardZone* create();//�½�������ʾ����
	virtual bool init();//��ʼ��
	void Show(PokerInfo cardInfo);//չʾ������Ϣ
};
#endif // __BOTTOM_CARD_ZONE_H__