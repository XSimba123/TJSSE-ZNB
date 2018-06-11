#pragma once
#ifndef Role_h
#define Role_h

#include <iostream>
#include <string>
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;
class Role : public cocos2d::Sprite
{
private:
	std::string     _roleName;                         ///人物的角色
	std::string     _playerName;                       ///玩家的姓名
	bool            _alreadyDead = false;              ///玩家是否真正的死亡
public:
	static Role* create(const std::string& filename)//使用图片创建人物
	{
		Role *sprite = new Role();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}



	void setAlreadyDead()//设置人物已经死亡
	{
		_alreadyDead = true;
	}


	bool getAlreadyDead()//得到玩家是否已经死亡的信息
	{
		return _alreadyDead;
	}


	void setRoleName(std::string name)//设置人物的角色名字 ，name 名字
	{
		_roleName = name;
	}

	std::string getRoleName()//得到人物的角色名字
	{
		return _roleName;
	}

	void setPlayerName(std::string playerName)//得到玩家姓名，name 名字
	{
		_playerName = playerName;
	}

	std::string getPlayerName()//得到玩家姓名
	{
		return _playerName;
	}

	std::string getPlayerPosition()// * @brief 得到玩家坐标位置（string方式得到 ，std::string
	
	{
		std::stringstream ss;
		std::stringstream ss2;
		std::string s1 = "(";
		ss << this->getPositionX();
		std::string positionX = ss.str();
		std::string s2 = ",";
		ss2 << this->getPositionY();
		std::string positionY = ss2.str();
		std::string s3 = ")";

		return s1 + positionX + s2 + positionY + s3;

	}
};
#endif /* Role_h */