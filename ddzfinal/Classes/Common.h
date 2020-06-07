#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"
#include "Player.h"
#include"Array.h"


//�����˵�
cocos2d::MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);
cocos2d::MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback callback);
//�ж��Ƿ�����
bool IsContinuous(__Array* vecPoker);
bool IsContinuous(std::vector<int>& vecPoker);
//�ж��Ƿ�С��2
bool IsLess2(__Array* vecPoker);
bool IsLess2(std::vector<int>& vecPoker);
//�ж�����
CARDS_DATA JudgePaiXing(__Array* cards);
CARDS_DATA JudgePaiXing(std::vector<PokerInfo>& cards);
CARDS_DATA JudgePaiXing(std::vector<int>& cards);
#endif //__COMMON_H__