#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"
#include "Player.h"
#include"Array.h"


//创建菜单
cocos2d::MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);
cocos2d::MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback callback);
//判断是否连续
bool IsContinuous(__Array* vecPoker);
bool IsContinuous(std::vector<int>& vecPoker);
//判断是否都小于2
bool IsLess2(__Array* vecPoker);
bool IsLess2(std::vector<int>& vecPoker);
//判断牌型
CARDS_DATA JudgePaiXing(__Array* cards);
CARDS_DATA JudgePaiXing(std::vector<PokerInfo>& cards);
CARDS_DATA JudgePaiXing(std::vector<int>& cards);
#endif //__COMMON_H__