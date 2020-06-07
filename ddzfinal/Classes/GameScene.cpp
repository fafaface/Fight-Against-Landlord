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

#include "GameScene.h"
#include "GameOverScene.h"

//#include "../editor-support/cocostudio/SimpleAudioEngine.h"
//#include"../cocos2d/cocos/scripting/deprecated/CCArray.h"

#define schedule_selector(_SELECTOR) CC_SCHEDULE_SELECTOR(_SELECTOR)


USING_NS_CC;

using namespace CocosDenshion;//ʹ�ø���������������ռ�

// ����ʱ�����Ϣ�洢

struct PlayerInfo{//�����Ϣ�ṹ��,����֮���������µ������Ϣ
    std::string _name;//����ǳ�
};

struct RunTimeData{//����ʱ������
    RunTimeData() {
        _isReady = false;//û��׼���ÿ�ʼ��Ϸ
    }
    bool		_isReady;
    PlayerInfo	_playerinfo1;//���1 2 3 ����Ϣ
    PlayerInfo	_playerinfo2;
    PlayerInfo	_playerinfo3;
};
static int index_fapai = 0; //����ʱ������
bool isPlayerWin = false;//��û�����ʤ��
RunTimeData s_runtimeData;//��ǰ��Ϸ���е�����
//��Ϸ����
Scene* GameScene::createScene()
{
    auto layer = GameScene::create();
    auto scene = Scene::create();
    scene->addChild(layer);//����ԭ���ĳ���, ��ʾ��Ϸ����
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if (!Layer::init()){
        return false;
    }
    //���ô���Ϸ�������˳�
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto endItem = MenuItemImage::create("CloseSelected.png", "CloseSelected.png",CC_CALLBACK_1(GameScene::GoToGameOverScene,this));
    endItem->setPosition(Point(endItem->getContentSize().width - (endItem->getContentSize().width / 4) + origin.x, visibleSize.height - endItem->getContentSize().height + (endItem->getContentSize().width / 4) + origin.y));
    //����رհ�ť��λ������Ļ�����Ͻ�
    auto menu = Menu::create(endItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,1);
    //���ñ���ͼƬ
    auto bg = Sprite::create("GameBackgroundPic.png");
    bg->setScale(1 / 0.8);
    bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bg, 0);
    // ��ʼ��Ϸ�˵�
    auto itemReady = customMenuItem("item_begin.png", "item_begin.png", CC_CALLBACK_1(GameScene::menuReadyCallback, this));
    _menuReady = Menu::create(itemReady, NULL);
    this->addChild(_menuReady, 1);
    // �������˵�
    auto itemQiang = customMenuItem("item_qiangdizhu.png", "item_qiangdizhu.png", CC_CALLBACK_1(GameScene::menuQiangCallback, this));
    itemQiang->setPosition(-80 / SCALE_FACTOR, -50 / SCALE_FACTOR);
    auto itemBuQiang = customMenuItem("item_buqiang.png", "item_buqiang.png", CC_CALLBACK_1(GameScene::menuBuQiangCallback, this));
    itemBuQiang->setPosition(80 / SCALE_FACTOR, -50 / SCALE_FACTOR);
    _menuQiangDiZhu = Menu::create(itemBuQiang, itemQiang, NULL);
    this->addChild(_menuQiangDiZhu, 1);
    _menuQiangDiZhu->setVisible(false);
    // ���Ʋ˵�
    auto itemBuChu = customMenuItem("item_buchu.png", "item_buchu_d.png", CC_CALLBACK_1(GameScene::menuBuChuCallback, this));
    itemBuChu->setPosition(100 / SCALE_FACTOR - 50 / SCALE_FACTOR, -50 / SCALE_FACTOR);
    auto itemChuPai = customMenuItem("item_chupai.png", "item_chupai_d.png", CC_CALLBACK_1(GameScene::menuChuPaiCallback, this));
    itemChuPai->setPosition(-100 / SCALE_FACTOR - 50 / SCALE_FACTOR, -50 / SCALE_FACTOR);
    _menuChuPai = Menu::create();
    _menuChuPai->addChild(itemBuChu,1,0);
    _menuChuPai->addChild(itemChuPai,1,1);
    this->addChild(_menuChuPai);
    _menuChuPai->setVisible(false);
    // �����Ϣ��ȡ
    srand(time(0));
    auto name_list = FileUtils::getInstance()->getValueMapFromFile("strings.xml").at("name_list").asValueVector();
    // ������״ν���˴�������������Ϣ
    if (!s_runtimeData._isReady)
    {
        int name_index_1 = rand() % name_list.size();
        int name_index_2 = rand() % name_list.size();
        int name_index_3 = rand() % name_list.size();

        while ((name_index_2 == name_index_1 )|| (name_index_2 == name_index_3)|| (name_index_1 == name_index_3))
        {
            name_index_1 = rand() % name_list.size();
            name_index_2 = rand() % name_list.size();
            name_index_3 = rand() % name_list.size();
        }

        s_runtimeData._playerinfo1._name = name_list[name_index_1].asString();
        s_runtimeData._playerinfo2._name = name_list[name_index_2].asString();
        s_runtimeData._playerinfo3._name = name_list[name_index_3].asString();
        s_runtimeData._isReady = true;
    }
    // ������λ��ҵ�ͷ����ǳ�
    _player1 = Player::create(s_runtimeData._playerinfo1._name, true);
    _player1->setPosition(70 / SCALE_FACTOR, 300 / SCALE_FACTOR);
    _player1->SetPlayerID(1);
    this->addChild(_player1);

    _player2 = Player::create(s_runtimeData._playerinfo2._name, false);
    _player2->setPosition(1000 / SCALE_FACTOR, 600 / SCALE_FACTOR);
    _player2->SetPlayerID(2);
    this->addChild(_player2);

    _player3 = Player::create(s_runtimeData._playerinfo3._name,  false);
    _player3->setPosition(70 / SCALE_FACTOR, 600 / SCALE_FACTOR);
    _player3->SetPlayerID(3);
    this->addChild(_player3);

    // ���ֵ�����
    _bottomCardZone = BottomCardZone::create();
    _bottomCardZone->setPosition(600 / SCALE_FACTOR, 610 / SCALE_FACTOR);
    this->addChild(_bottomCardZone);

    // ��ʼ���ƶ�
    initCards();
    _arrPlayerOut = __Array::create();//�ϼҳ�����
    _arrPlayerOut->retain();//ʹ��retain,һֱ����,���ⱻ�ͷŵ�
    _state = 1;
    _gameover = false;
    _begin = false;

    return true;
}

void GameScene::menuReadyCallback(cocos2d::Ref* pSender)
{
    DealCards();//����
    _menuReady->setVisible(false);//���� ��ʼ��Ϸ���ɼ�
}


void GameScene::menuQiangCallback(Ref* pSender)
{
    DealLandlordCards(_player1);    // �ַ�����
    _menuQiangDiZhu->setVisible(false);    // �л��˵��ɼ�
    _menuChuPai->setVisible(true);
}

void GameScene::menuBuQiangCallback(Ref* pSender)
{
    _menuQiangDiZhu->setVisible(false);  // �л��˵��ɼ�
    auto delay = DelayTime::create(2);
    auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::Qiang2Callback, this));
    auto seq = Sequence::createWithTwoActions(delay, callback);
    this->runAction(seq);
}



void GameScene::menuBuChuCallback(Ref* pSender)
{
    _player3->clearCards();
    _player1->BuChu();

    auto delay = DelayTime::create(1);
    auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::ChuPai2Callback, this));
    auto seq = Sequence::createWithTwoActions(delay, callback);
    this->runAction(seq);

    _menuChuPai->setVisible(false);
}

void GameScene::menuChuPaiCallback(Ref* pSender)
{
    if (!_begin)
    {
        _begin = true;
    }
    auto player3_outcards = _player3->GetOutCards();
    CARDS_DATA player3_card_data = JudgePaiXing(player3_outcards); // ����֮ǰ���ж��ϼҵ�����

    if (player3_outcards.empty())
    {
        auto player2_outcards = _player2->GetOutCards();
        CARDS_DATA player2_card_data = JudgePaiXing(player2_outcards);
        if (player2_outcards.empty())
            _player1->OutCards(this, false, ERROR_CARD, 0, 1);// ����
        else
            _player1->OutCards(this, true, player2_card_data._type, player2_outcards.size(), player2_card_data._value);// ����
    }
    else
        _player1->OutCards(this, true, player3_card_data._type, player3_outcards.size(), player3_card_data._value);// ����
    _arrPlayerOut->removeAllObjects();    // ��������ж���

    if (!_gameover)
    {
        auto delay = DelayTime::create(1);
        auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::ChuPai2Callback, this));
        auto seq = Sequence::createWithTwoActions(delay, callback);
        this->runAction(seq);
    }

    _menuChuPai->setVisible(false);
}

void GameScene::ChuPai2Callback(Node* node)
{
    auto player1_outcards = _player1->GetOutCards();
    CARDS_DATA player1_card_data = JudgePaiXing(player1_outcards);  // ����֮ǰ���ж��ϼҵ�����

    if (player1_outcards.empty())
    {
        auto player3_outcards = _player3->GetOutCards();
        CARDS_DATA player3_card_data = JudgePaiXing(player3_outcards);
        if (player3_outcards.empty())
            _player2->OutCards(this, false, ERROR_CARD, 0, 1);// ����
        else
            _player2->OutCards(this, true, player3_card_data._type, player3_outcards.size(), player3_card_data._value);// ����
    }
    else
        _player2->OutCards(this, true, player1_card_data._type, player1_outcards.size(), player1_card_data._value);// ����
    
    if (!_gameover)
    {
        auto delay = DelayTime::create(1);
        auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::ChuPai3Callback, this));
        auto seq = Sequence::createWithTwoActions(delay, callback);
        this->runAction(seq);
    }
}

void GameScene::ChuPai3Callback(Node* node)
{
    auto player2_outcards = _player2->GetOutCards();
    CARDS_DATA player2_card_data = JudgePaiXing(player2_outcards); // ����֮ǰ���ж��ϼҵ�����
    if (player2_outcards.empty())
    {
        auto player1_outcards = _player1->GetOutCards();
        CARDS_DATA player1_card_data = JudgePaiXing(player1_outcards);
        if (player1_outcards.empty())
            _player3->OutCards(this, false, ERROR_CARD, 0, 1);// ����
        else
            _player3->OutCards(this, true, player1_card_data._type, player1_outcards.size(), player1_card_data._value);// ����
    }
    else
        _player3->OutCards(this, true, player2_card_data._type, player2_outcards.size(), player2_card_data._value);// ����
    if (!_gameover)
        _menuChuPai->setVisible(true);
}

void GameScene::Qiang2Callback(Node* node)
{
    if (_player2->IsQiangLandlord())// �����Ҳ��������������¼����
    {
      DealLandlordCards(_player2);
        if (!_gameover)
        {
            auto delay = DelayTime::create(3);
            auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::ChuPai2Callback, this));
            auto seq = Sequence::createWithTwoActions(delay, callback);
            this->runAction(seq);
            if (!_begin)
                _begin = true;
        }
    }
    else
    {
        auto delay = DelayTime::create(2);
        auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::Qiang3Callback, this));
        auto seq = Sequence::createWithTwoActions(delay, callback);
        this->runAction(seq);
    }
}

void GameScene::Qiang3Callback(Node* node)//���3������
{
    DealLandlordCards(_player3);//�������3����
    if (!_gameover)
    {
        auto delay = DelayTime::create(3);
        auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::ChuPai3Callback, this));
        auto seq = Sequence::createWithTwoActions(delay, callback);
        this->runAction(seq);
        if (!_begin)
        {
            _begin = true;
        }
    }
}

void GameScene::DealCards()
{
    // ϴ��
    index_fapai = 0;
    srand(time(0));
    std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());

    // ����
    auto delay = DelayTime::create(0.04);
    auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::DealCardsCallback, this));
    auto seq = Sequence::createWithTwoActions(delay, callback);
    this->runAction(seq);

    schedule(schedule_selector(GameScene::OutCard), 0.1);
}

void GameScene::DealCardsCallback(Node* node)
{
    if (index_fapai < 51) {
        if (index_fapai % 3 == 0)  // ��ɫ1
            _player1->DealCards(this, _pokeInfo.at(index_fapai));
        else if (index_fapai % 3 == 1)// ��ɫ2 
            _player2->DealCards(this, _pokeInfo.at(index_fapai));
        else if (index_fapai % 3 == 2) // ��ɫ3
            _player3->DealCards(this, _pokeInfo.at(index_fapai));
        index_fapai++;

        auto delay = DelayTime::create(0.01);
        auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::DealCardsCallback, this));
        auto seq = Sequence::createWithTwoActions(delay, callback);
        this->runAction(seq);
    }
    else
        _menuQiangDiZhu->setVisible(true);
}

void GameScene::initCards()//��ʼ���˿���
{
    PokerInfo info;
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
        {
            info._num = (PokerNum)i;
            info._tag = (PokerTag)j;
            _pokeInfo.push_back(info);
        }
    info._num = (PokerNum)13;
    info._tag = (PokerTag)0;
    _pokeInfo.push_back(info);
    info._num = (PokerNum)14;
    info._tag = (PokerTag)0;
    _pokeInfo.push_back(info);
}

void GameScene::DealLandlordCards(Player* player)
{
    for (int i = index_fapai; i < 54; i++)
    {
        //����
        _bottomCardZone->Show(_pokeInfo.at(i));//��ʾ����
        _vecDiPai.push_back(_pokeInfo.at(i));//�������
    }

    player->setLandlord();//����Ϊ����
    for (int i = 0; i < _vecDiPai.size(); i++)
    {
        player->DealCards(this, _vecDiPai[i]);//��������ҷ��ŵ���
    }
}

void GameScene::Update(float delta)
{
}

void GameScene::OutCard(float delta)
{
    if (_state == 1)
    {
        // ����
        if (!_begin)
        {
            CARDS_DATA card_data = JudgePaiXing(_arrPlayerOut);
            setOutCardsMenuEnabled(card_data._type != ERROR_CARD, false);
        }
        // �Ѿ�����
        else
        {
            CARDS_DATA card_data = JudgePaiXing(_arrPlayerOut);

            auto player3_outcards = _player3->GetOutCards();
            CARDS_DATA player3_card_data = JudgePaiXing(player3_outcards);

            if (player3_outcards.empty())
            {
                auto player2_outcards = _player2->GetOutCards();
                CARDS_DATA player2_card_data = JudgePaiXing(player2_outcards);
                if (player2_outcards.empty())
                {
                    setOutCardsMenuEnabled(card_data._type != ERROR_CARD, false);
                }
                else// ��ʾ����δ����
                {         
                    if (
                        // ��ͬ����
                        (card_data._type == player2_card_data._type && card_data._value > player2_card_data._value) ||
                        // ��ͬ������ը������
                        (card_data._type != player2_card_data._type && player2_card_data._type != MISSILE_CARD &&
                            (card_data._type == BOMB_CARD || card_data._type == MISSILE_CARD) && card_data._value > player2_card_data._value))
                    {
                        setOutCardsMenuEnabled(true, true);
                    }
                    else
                        setOutCardsMenuEnabled(false, true);
                }
            }
            else
            {
                if (
                    // ��ͬ����
                    (card_data._type == player3_card_data._type && card_data._value > player3_card_data._value) ||
                    // ��ͬ������ը������
                    (card_data._type != player3_card_data._type && player3_card_data._type != MISSILE_CARD &&
                        (card_data._type == BOMB_CARD || card_data._type == MISSILE_CARD) && card_data._value > player3_card_data._value))
                {
                    setOutCardsMenuEnabled(true, true);
                }
                else
                    setOutCardsMenuEnabled(false, true);
            }
        }
    }
}

void GameScene::SettleAccountsCallback(Node* node)//��Ϸ����,������Ӯ���
{
    _menuChuPai->setEnabled(false);
    _menuQiangDiZhu->setEnabled(false);
    _menuReady->setEnabled(false);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto jiesuanBg = Sprite::create("GameOverPic.png");//������Ϸ������ʾ����ͼƬ, ����ͼƬλ��
    jiesuanBg->setPosition(Vec2(visibleSize.width /2  + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(jiesuanBg, 0);

    auto shengli = Sprite::createWithSpriteFrameName(isPlayerWin ? "ddz_result_base_word_win.png" : "ddz_result_base_word_lose.png");
    shengli->setPosition(Vec2(350 / SCALE_FACTOR, 430 / SCALE_FACTOR));
    jiesuanBg->addChild(shengli, 0);

}

void GameScene::SettleAccounts(int winID)
{
    // �������
    auto delay = DelayTime::create(1);
    auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::SettleAccountsCallback, this));
    auto seq = Sequence::createWithTwoActions(delay, callback);
    this->runAction(seq);
}

void GameScene::GameOver(int winID)
{
    _gameover = true;
    if (winID == 1)//������Ӯ��(�ǵ�����ũ��һ��)
        isPlayerWin = 1;
    else if (_player1->getIsLandlord() && winID != 1)//����ǵ���,����
        isPlayerWin = 0;
    else if (!_player2->getIsLandlord() && winID == 2)//��ת����, ˵����һ��ũ��Ӯ��,��Ҳ��ǵ���
        isPlayerWin = 1;
    else if (!_player3->getIsLandlord() && winID == 3)//��ת����, ˵����һ��ũ��Ӯ��,��Ҳ��ǵ���
         isPlayerWin = 1;
    unschedule(schedule_selector(GameScene::OutCard));
    _menuChuPai->setVisible(false);
    _menuQiangDiZhu->setVisible(false);
    _menuReady->setVisible(false);
    SettleAccounts(winID);
}

void GameScene::setOutCardsMenuEnabled(bool isChuPai, bool isBuChu)
{
    ((MenuItemFont*)_menuChuPai->getChildByTag(1))->setEnabled(isChuPai);//����
    ((MenuItemFont*)_menuChuPai->getChildByTag(0))->setEnabled(isBuChu);//����
}

__Array* GameScene::getArrPlayerOut()
{
    return _arrPlayerOut;
}

void GameScene::GoToGameOverScene(Ref* pSender)
{
    auto scene = GameOver::createScene();
    Director::getInstance()->replaceScene(scene);
}

