#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "cocos2d.h"
#include "../Classes/Poker.h"
#include "../editor-support/cocostudio/SimpleAudioEngine.h"   //������������ͷ�ļ�
#include <vector>
#include<string>
#define SCALE_FACTOR 0.625f //λ�ù�ģ����
USING_NS_CC;

class GameScene;

// ����
enum  CARD_TYPE{
	SINGLE_CARD = 1,			//����
	DOUBLE_CARD,			//����
	THREE_CARD,				//3����
	BOMB_CARD,				//ը��
	MISSILE_CARD,			//���
	THREE_ONE_CARD,			//3��1
	THREE_TWO_CARD,			//3��2
	BOMB_TWO_CARD,			//�ĸ���2�ŵ���
	BOMB_TWOOO_CARD,		//�ĸ���2��
	CONNECT_CARD,			//����
	COMPANY_CARD,			//����
	AIRCRAFT_CARD,			//�ɻ�����
	AIRCRAFT_SINGLE_CARD,	//�ɻ�������
	AIRCRAFT_DOUBLE_CARD,	//�ɻ�������
	ERROR_CARD				//���������
};

// ÿ���Ƶ���Ϣ
struct CARDS_DATA{
	std::vector<int>	_cards;//�Ƶ�����
	CARD_TYPE			_type;//����
	unsigned int		_value;//ֵ
};

//����չʾ����
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

	void chupai(std::vector<PokerInfo> arrayIndexes)//����
	{
		if (arrayIndexes.empty())//û��ѡ������
		{
			auto label_buchu = cocos2d::Sprite::create("buchu.png");
			this->addChild(label_buchu);
		}

		for (int i = 0; i < arrayIndexes.size(); ++i)//ѡ��������
		{
			auto card = Poker::create(arrayIndexes.at(i));
			card->setScale(0.5);//���Ĵ�С
			this->addChild(card, 100 - arrayIndexes.at(i)._num);
		}
		sortAllChildren();//����

		int count = _children.size();
		for (int i = 0; i < _children.size(); ++i)
		{
			Poker* card = dynamic_cast<Poker*>(_children.at(i));
			if (card != NULL)
			{
				card->setPosition(i * 30 /SCALE_FACTOR,0);//����λ��
			}
		}
	}
};


//�����

class Player :public cocos2d::Sprite {
public:
	static Player* create(std::string name,bool isHero);//������� 
	virtual bool init(std::string name, bool isHero);//��ʼ��
	void updateCards();//��������
	void clearCards();//�������

	//�Ƿ��ǵ���
	void setLandlord();//�����Ƿ��ǵ���
	bool getIsLandlord();//����true ���ǵ���

	void DealCards(GameScene* scene, PokerInfo info);	//����
	void OutCards(GameScene* scene, bool isFollow, CARD_TYPE cardtype, unsigned int count, unsigned int value);//����
	void BuChu();//����
	void ChaiPai();//����
	void ShowTipInfo(bool isFollow, CARD_TYPE cardType, unsigned int count, unsigned int value);//չʾ��ʾ��Ϣ
	bool IsQiangLandlord();//�Ƿ�������
	std::string GetName();//�������
	void SetPlayerID(size_t id);//�������ID

	size_t GetPlayerID();//������ID

	//�ҳ����Ƶ�����
	std::vector<int>& FindFollowCards(CARD_TYPE cardType, unsigned int count, unsigned int value);
	//�ҳ��Ƶ�����
	std::vector<int>& FindOutCards();
	std::vector<PokerInfo>& GetOutCards();//��ó��Ƶ�����

protected:
	size_t _id;//���ID
	bool _isHero;//�Ƿ��ǵ�һ�������Ƶ�
	bool _isLandlord;//�Ƿ��ǵ���
	
	std::string _name;//����
	//int _score;//�ɼ�
	cocos2d::Sprite* _cardsManager;//���ƹ���
	cocos2d::Label* _labelName;//����
	cocos2d::Label* _labelPokerCount;//����

	std::vector<PokerInfo> _vecPokerInfo;//�˿���Ϣ����
	std::vector<int> _vecFindFollowCards;//����
	std::vector<PokerInfo> _vecOutCards;//����

	PokerShowZone* _showZone;//չʾ��
	std::vector<CARDS_DATA> _allCardGroups;//����
	cocos2d::Label* _labelTipInfo;//��ʾ��Ϣ

	int m_intArray[15];//��¼ÿ���Ƴ��ֵĴ���

};

#endif // __PLAYER_H__
