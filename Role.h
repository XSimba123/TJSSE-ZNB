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
	std::string     _roleName;                         ///����Ľ�ɫ
	std::string     _playerName;                       ///��ҵ�����
	bool            _alreadyDead = false;              ///����Ƿ�����������
public:
	static Role* create(const std::string& filename)//ʹ��ͼƬ��������
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



	void setAlreadyDead()//���������Ѿ�����
	{
		_alreadyDead = true;
	}


	bool getAlreadyDead()//�õ�����Ƿ��Ѿ���������Ϣ
	{
		return _alreadyDead;
	}


	void setRoleName(std::string name)//��������Ľ�ɫ���� ��name ����
	{
		_roleName = name;
	}

	std::string getRoleName()//�õ�����Ľ�ɫ����
	{
		return _roleName;
	}

	void setPlayerName(std::string playerName)//�õ����������name ����
	{
		_playerName = playerName;
	}

	std::string getPlayerName()//�õ��������
	{
		return _playerName;
	}

	std::string getPlayerPosition()// * @brief �õ��������λ�ã�string��ʽ�õ� ��std::string
	
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