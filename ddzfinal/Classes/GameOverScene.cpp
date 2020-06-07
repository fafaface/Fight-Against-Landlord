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

#include "GameOverScene.h"
#include"MainMenuScene.h"
USING_NS_CC;

Scene* GameOver::createScene()//创建游戏结束场景
{
    auto layer = GameOver::create();
    auto scene = Scene::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto OverTitle = MenuItemImage::create("GameOverPic.png", "GameOverPic.png");//游戏结束图片
    auto mainmenuItem = MenuItemFont::create("Return to Main Menu",CC_CALLBACK_1(GameOver::GoToMainMenuScene,this));//返回注界面按钮
    auto endItem = MenuItemFont::create("Exit", CC_CALLBACK_1(GameOver::GoExitGame, this));//退出游戏按钮
    auto menu = Menu::create(OverTitle, mainmenuItem, endItem,NULL);//创建菜单
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 8);//垂直排列
    this->addChild(menu);//添加到场景中
    return true;
}

void GameOver::GoToMainMenuScene(Ref* pSender)//返回主菜单
{
    auto scene = MainMenu::createScene();
    Director::getInstance()->replaceScene(scene);
}

void GameOver::GoExitGame(Ref* pSender)//退出游戏
{
    Director::getInstance()->end();
}

