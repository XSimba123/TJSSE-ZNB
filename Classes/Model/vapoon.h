#pragma once
#define Vapoon_h

#include <iostream>
#include <string>
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;
enum vapoonID 
{
	crossbow,
	gun,
	fireball
};
class Vapoon : public cocos2d::Sprite
{
public:
	int           ATT;
	int           Range;
	int           frontnum=1;
	int           besidenum=0;
	int           backnum=0;
	float           shootSPD;
	float           waitTime;
	bool          crossAdmitted;
	bool          goback;
	bool          scalebig;
	Sprite*       VapoonUI;
	
public:
	static Vapoon* create()//使用图片创建人物
	{
		Vapoon *sprite = new Vapoon();
		sprite->VapoonUI = Sprite::create();
		return sprite;
	}
	void setproperties(vapoonID ID )
	{
		if (ID == crossbow) 
		{
			ATT = 0.4;
			Range = 100;
			shootSPD = 0.3;
			waitTime = 1.4;
		}
		if (ID == gun)
		{
			ATT = 0.1;
			Range = 200;
			shootSPD = 0.15;
			waitTime = 0.8;
		}
		if (ID == fireball)
		{
			ATT = 1.5;
			Range = 75;
			shootSPD = 0.6;
			waitTime = 2.;
		}
	}
};

