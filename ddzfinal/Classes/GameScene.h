/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "../cocos2d/cocos/cocos2d.h"
#include "Poker.h"
#include "BottomCardZone.h"
#include "Player.h"
#include "Common.h"

class GameScene : public cocos2d::Layer
{
public:

    Player* _player1;
    Player* _player2;
    Player* _player3;

protected:

    std::vector<PokerInfo> _vecDiPai;

    BottomCardZone* _bottomCardZone;

    cocos2d::Menu* _menuBack;
    cocos2d::Menu* _menuChuPai;
    cocos2d::Menu* _menuQiangDiZhu;
    cocos2d::Menu* _menuReady;

    int		_state;	// ��ǰ״̬��1���ƣ�2�ȴ�
    bool	 _gameover;
    bool	 _begin;

    std::vector<PokerInfo> _pokeInfo;
   __Array* _arrPlayerOut;	// �ϼҵ�ǰ������

public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    //��Ϸ����ʱ�Ĳ˵�, �˵��ص�
    void menuReadyCallback(cocos2d::Ref* pSender);//׼��
    void menuQiangCallback(Ref* pSender);//������
    void menuBuQiangCallback(Ref* pSender);//��������
    void menuBuChuCallback(Ref* pSender);//������
    void menuChuPaiCallback(Ref* pSender);//����

    //�����������
    void ChuPai2Callback(Node* node);//����
    void ChuPai3Callback(Node* node);//����
    void Qiang2Callback(Node* node);//������
    void Qiang3Callback(Node* node);//������

    //��Ϸ������� 
    void DealCards();//����
    void DealCardsCallback(Node* node);//��Ӧ����
    void initCards();//��ʼ����
    void DealLandlordCards(Player* player);//���������ĵ���
    void Update(float delta);//����״̬
    void OutCard(float delta);//������
    void SettleAccountsCallback(Node* node);//������Ϸ����ص�
    void SettleAccounts(int winID);//������Ϸ���
    void GameOver(int winID);//�ж���Ϸ������
    void setOutCardsMenuEnabled(bool isChuPai, bool isBuChu);//�ж��Ƿ���ֳ��Ʋ˵�
    __Array* getArrPlayerOut();
    void GoToGameOverScene(Ref* pSender);//��Ϸ��������
    CREATE_FUNC(GameScene);
    
};

#endif // __GAME_SCENE_H__