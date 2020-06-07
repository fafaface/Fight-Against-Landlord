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

    int		_state;	// 当前状态，1出牌，2等待
    bool	 _gameover;
    bool	 _begin;

    std::vector<PokerInfo> _pokeInfo;
   __Array* _arrPlayerOut;	// 上家当前出的牌

public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    //游戏进行时的菜单, 菜单回调
    void menuReadyCallback(cocos2d::Ref* pSender);//准备
    void menuQiangCallback(Ref* pSender);//抢地主
    void menuBuQiangCallback(Ref* pSender);//不抢地主
    void menuBuChuCallback(Ref* pSender);//不出牌
    void menuChuPaiCallback(Ref* pSender);//出牌

    //另外两个玩家
    void ChuPai2Callback(Node* node);//出牌
    void ChuPai3Callback(Node* node);//出牌
    void Qiang2Callback(Node* node);//抢地主
    void Qiang3Callback(Node* node);//抢地主

    //游戏画面控制 
    void DealCards();//发牌
    void DealCardsCallback(Node* node);//响应发牌
    void initCards();//初始化牌
    void DealLandlordCards(Player* player);//发给地主的底牌
    void Update(float delta);//更新状态
    void OutCard(float delta);//出的牌
    void SettleAccountsCallback(Node* node);//结算游戏结果回调
    void SettleAccounts(int winID);//结算游戏结果
    void GameOver(int winID);//判定游戏结束了
    void setOutCardsMenuEnabled(bool isChuPai, bool isBuChu);//判定是否出现出牌菜单
    __Array* getArrPlayerOut();
    void GoToGameOverScene(Ref* pSender);//游戏结束画面
    CREATE_FUNC(GameScene);
    
};

#endif // __GAME_SCENE_H__