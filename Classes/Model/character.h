#pragma once
#ifndef Character_h
#define Character_h

#include <iostream>
#include <string>
#include <time.h>
#include "cocos2d.h"
#include "model\vapoon.h"
USING_NS_CC;
using namespace ui;
enum skillID
{
	addfrontnum,
	addbacknum,
	addbesidenum,
	scalevapoon,
	addDEF,
	addATT,
	addSPD,
	addHP,
	addXPRate,
	addRange,
	addAll,
	useCrossBow,
	useGun,
	useFireBall,
	K1A2,
	characterCross,
	vapoonCross

};
class Character : public cocos2d::Sprite
{
private:
	std::string     _playerName;                       ///玩家的姓名
	bool            _alreadyDead = false;              ///玩家是否真正的死亡
	float           NowHP;
	int             MaxHP;
	int             NowXP;
	int             NeededXP;
	int             Level;
	int             MaxLevel = 20;
	float           ATT;
	float           DEF;
	float           SPD;
	int             ATT_Range;
	bool            ThroughWallAdmitted=false;
	bool            reachFullLevel = false;
public:
	int             deadbegintime=0;
	bool            movable=false;
	std::string     _playName;
	Vapoon *        vapoon;
	Sprite *        nowHpUI;
	Sprite*         maxHpUI;
	vapoonID        vapoonID;
	int             canSelect;
	int             select_time=0;
	float           XPrate = 1;
	int             killNum=0;
	bool
		isK1A2, 
		isscale,//
		isCharacterCross,//
		isVapoonCross,//
		isXPrate;
	bool selected[17];
	Sprite* SkillUI[3];
	static Character* create(const std::string& filename)//使用图片创建人物
	{
		Character *sprite = new Character();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			sprite->NowHP = sprite->MaxHP = 3;
			sprite->NowXP = 0;
			sprite->NeededXP =50;
			sprite->Level = 1;
			sprite->ATT_Range = 200;
			sprite->SPD = 6;
			sprite->ATT = 0.5;
			sprite->DEF = 0;
			sprite->vapoonID = crossbow;
			sprite->vapoon = Vapoon::create();
			sprite->vapoon->setproperties(sprite->vapoonID);
			sprite->maxHpUI = Sprite::create("scene/gameUI/maxHP.png", Rect(0, 0, 30 * 3, 20));
			sprite->maxHpUI->setAnchorPoint(Vec2(0.5, 0.5));
			sprite->nowHpUI = Sprite::create("scene/gameUI/lightHP.png", Rect(0, 0, 30 * 3, 20));
			for (int i = 0; i < 3; i++)
			{
				sprite->SkillUI[i] = Sprite::create("model/skill/addfrontnum.png");
				sprite->SkillUI[i]->setVisible(false);
			}
			sprite->nowHpUI->setAnchorPoint(Vec2(0, 0.5));
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void setcomeback(Sprite* baselevelUI, Label* level)
	{
		NowHP =MaxHP = 3;
		NowXP = 0;
		NeededXP = 50;
		Level = 1;
		ATT_Range = 200;
		SPD = 6;
		ATT = 0.5;
		DEF = 0;
		vapoonID = crossbow;
		vapoon->setproperties(vapoonID);
		deadbegintime = 0;
		_alreadyDead = false;
		baselevelUI->setTextureRect(Rect(0, 0, NowXP * 300 / NeededXP, 20));
		level->setString("Level:" + this->getLevel());
		maxHpUI->setTextureRect(Rect(0, 0, 30 * MaxHP, 20));
		nowHpUI->setTextureRect(Rect(0, 0, 30 * NowHP, 20));
		nowHpUI->setPosition(maxHpUI->getPosition() - Vec2(MaxHP * 15, 0));
	}
	void setcomeback()
	{
		NowHP = MaxHP = 3;
		NowXP = 0;
		NeededXP = 50;
		Level = 1;
		ATT_Range = 200;
		SPD = 6;
		ATT = 0.5;
		DEF = 0;
		vapoonID = crossbow;
		vapoon->setproperties(vapoonID);
		maxHpUI->setTextureRect(Rect(0, 0, 30 * 3, 20));
		maxHpUI->setAnchorPoint(Vec2(0.5, 0.5));
		nowHpUI->setTextureRect(Rect(0, 0, 30 * 3, 20));
		nowHpUI->setAnchorPoint(Vec2(0, 0.5));
		setTexture("model/character/character-crossbow.png");
		deadbegintime = 0;
		_alreadyDead = false;
		setColor(Color3B(255, 255, 255));
	}
	std::string getLevel()
	{
		std::stringstream ss;
		ss << this->Level;
		std::string nowlevel = ss.str();
		std::string smid = std::string("            Exp:");
		return nowlevel+smid;
	}
	int get_level()
	{
		return Level;
	}
	int getMaxHP()
	{
		return MaxHP;
	}
	int getXP()
	{
		return NowXP;
	}
	int getNeededXP()
	{
		return NeededXP;
	}
	int getATT_Range()
	{
		return this->ATT_Range;
	}
	void addATT_Range()
	{
		this->ATT_Range+=20;
	}

	float get_ATT()
	{
		return this->ATT;
	}

	void add_ATT()
	{
		this->ATT += 0.2;
	}
	float getDEF()
	{
		return this->DEF;
	}

	void add_DEF()
	{
		this->DEF += 0.1;
	}
	void add_All()
	{
		ATT += 0.05;
		DEF += 0.02;
		ATT_Range += 10;
		SPD += 0.4;
	}
	float get_SPD()
	{
		return this->SPD;
	}

	void add_SPD()
	{
		this->SPD += 1;
	}
	float get_HP()
	{
		return this->NowHP;
	}
	void set_HP(float HP)
	{
		NowHP = HP;
		nowHpUI->setTextureRect(Rect(0, 0, 30 * NowHP, 20));
	}
	void add_HP(int addHP)
	{
		if (this->NowHP + addHP <= this->MaxHP)this->NowHP += addHP;
		else NowHP = MaxHP;
		nowHpUI->setTextureRect(Rect(0, 0, 30 * NowHP, 20));
	}
	void add_MaxHP()
	{
		if (MaxHP < 7)
		{
			MaxHP++;
			NowHP++;
		}
		maxHpUI ->setTextureRect(Rect(0, 0, 30 * MaxHP, 20));
		nowHpUI->setTextureRect(Rect(0, 0, 30 * NowHP , 20));
		nowHpUI->setPosition(maxHpUI->getPosition() - Vec2(MaxHP * 15, 0));
	}
	void addXP(int addXP, Sprite* baselevelUI,Label* level)
	{
		if (this->NowXP + addXP < this->NeededXP)
		{
			this->NowXP += addXP;
			baselevelUI->setTextureRect(Rect(0,0,NowXP*300/NeededXP,20));
		}
		else addLevel(baselevelUI,level);
	}
	void EnemyaddLevel()
	{
		if (this->Level <= 19)
		{
			this->NeededXP *= 1.2;
			this->SPD -= 0.2;
			this->NowXP = 0;
			this->Level++;
			this->setScale(1 + 0.05*(float)Level);
			if (Level == 20)reachFullLevel = true;
		}
	}
	void addLevel( Sprite* baselevelUI,Label* level)
	{
		if (this->Level <= 19)
		{
			this->NeededXP *= 1.2;
			this->SPD -= 0.2;
			this->NowXP = 0;
			this->Level++;
			this->setScale(1 + 0.05*(float)Level);
			level->setString("Lv . " + this->getLevel());
			baselevelUI->setTextureRect(Rect(0, 0,0, 20));

			select_time++;
			
			if (Level == 20)reachFullLevel = true;

		}

	}
	void setSkillUI()
	{
		chooseSkill(this);
		log("%d %d %d", SkillUI[0]->getTag(), SkillUI[1]->getTag(), SkillUI[2]->getTag());
		SkillUI[0]->setPosition(Vec2(400, 200));
		SkillUI[1]->setPosition(Vec2(640, 200));
		SkillUI[2]->setPosition(Vec2(880, 200));
		canSelect = 1;
	}

	bool setDamage(float att)
	{
		float damage = att - this->DEF;
		if (!_alreadyDead)
		{
			if (this->NowHP - damage > 0)
			{
				this->NowHP -= damage;
				nowHpUI->setTextureRect(Rect(0, 0, 30 * NowHP, 20));
				return false;
			}
			else
			{
				this->NowHP = 0;
				nowHpUI->setTextureRect(Rect(0, 0, 30 * NowHP, 20));
				if(this->getTag()==0)setAlreadyDead();
				return true;
			}
		}
		else return false;
	}

	void setAlreadyDead()//设置人物已经死亡
	{
		setColor(Color3B(100, 100, 100));
		_alreadyDead = true;
	}


	bool getAlreadyDead()//得到玩家是否已经死亡的信息
	{
		return _alreadyDead;
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



	void chooseSkill(Character* mycharacter)
	{

		for (int i = 0; i < 3; i++)
		{
			//if (SkillUI[i] == nullptr)SkillUI[i] = Sprite::create("model/skill/addfrontnum.png");
			bool isdecided = false;
			while (!isdecided)
			{
				int selection =  CCRANDOM_0_1() * 16;
				switch (selection)
				{
				case addfrontnum:
					if (selected[addfrontnum])continue;
					if (vapoon->frontnum < 4)
					{
						SkillUI[i]->setTexture("model/skill/addfrontnum.png");
						SkillUI[i]->setTag(addfrontnum);
						isdecided = true;
						selected[addfrontnum] = true;
						break;
					}
					continue;
				case addbacknum:
					if (selected[addbacknum])break;
					if (vapoon->backnum < 4)
					{
						SkillUI[i]->setTexture("model/skill/addbacknum.png");
						SkillUI[i]->setTag(addbacknum);
						isdecided = true;
						selected[addbacknum] = true;
						break;
					}
					continue;
				case addbesidenum:
					if (selected[addbesidenum])break;
					if (vapoon->besidenum < 2)
					{
						SkillUI[i]->setTexture("model/skill/addbesidenum.png");
						SkillUI[i]->setTag(addbesidenum);
						isdecided = true;
						selected[addbesidenum] = true;
						break;
					}
					continue;
				case scalevapoon:
					if (selected[scalevapoon])break;
					if (!isscale)
					{
						SkillUI[i]->setTexture("model/skill/scalevapoon.png");
						SkillUI[i]->setTag(scalevapoon);
						isdecided = true;
						selected[scalevapoon] = true;
						break;
					}
					continue;
				case addDEF:
					if (selected[addDEF])break;
					SkillUI[i]->setTexture("model/skill/addDEF.png");
					SkillUI[i]->setTag(addDEF);
					isdecided = true;
					selected[addDEF] = true;
					break;
				case addATT:
					if (selected[addATT])break;
					SkillUI[i] ->setTexture("model/skill/addATT.png");
					SkillUI[i]->setTag(addATT);
					isdecided = true;
					selected[addATT] = true;
					break;
				case addSPD:
					if (selected[addSPD])break;
					SkillUI[i]->setTexture("model/skill/addSPD.png");
					SkillUI[i]->setTag(addSPD);
					isdecided = true;

					selected[addSPD] = true;
					break;
				case addRange:
					if (selected[addRange])break;
					SkillUI[i]->setTexture("model/skill/addRange.png");
					SkillUI[i]->setTag(addRange);
					isdecided = true;
					selected[addRange] = true;
					break;
				case addXPRate:
					if (selected[addXPRate])break;
					if (!isXPrate)
					{
						SkillUI[i]->setTexture("model/skill/addXPRate.png");
						SkillUI[i]->setTag(addXPRate);
						isdecided = true;
						selected[addXPRate] = true;
						break;
					}
					continue;
				case addHP:
					if (selected[addHP])break;
					if (MaxHP < 7)
					{
						SkillUI[i]->setTexture("model/skill/addHP.png");
						SkillUI[i]->setTag(addHP);
						isdecided = true;
						selected[addHP] = true;
						break;
					}
					continue;
				case addAll:
					if (selected[addAll])break;
					SkillUI[i]->setTexture("model/skill/addAll.png");
					SkillUI[i]->setTag(addAll);
					isdecided = true;
					selected[addAll] = true;
					break;
				case K1A2:
					if (selected[K1A2])break;
					if (!isK1A2)
					{
						SkillUI[i]->setTexture("model/skill/K1A2.png");
						SkillUI[i]->setTag(K1A2);
						isdecided = true;
						selected[K1A2] = true;
						break;
					}
					continue;
				case characterCross:
					if (selected[characterCross])break;
					if (!isCharacterCross)
					{
						SkillUI[i]->setTexture("model/skill/characterCross.png");
						SkillUI[i]->setTag(characterCross);
						isdecided = true;
						selected[characterCross] = true;
						break;
					}
					continue;
				case vapoonCross:
					if (selected[vapoonCross])break;
					if (!isVapoonCross)
					{
						SkillUI[i]->setTexture("model/skill/vapoonCross.png");
						SkillUI[i]->setTag(vapoonCross);
						isdecided = true;
						selected[vapoonCross] = true;
						break;
					}
				case useCrossBow:
					if (selected[useCrossBow])break;
					if (vapoonID != crossbow)
					{
						SkillUI[i]->setTexture("model/skill/usecrossbow.png");
						SkillUI[i]->setTag(useCrossBow);
						isdecided = true;
						selected[useCrossBow] = true;
						break;
					}
					continue;
				case useGun:
					if (selected[useGun])break;
					if (vapoonID != gun)
					{
						SkillUI[i]->setTexture("model/skill/useGun.png");
						SkillUI[i]->setTag(useGun);
						isdecided = true;
						selected[useGun] = true;
						break;
					}
					continue;
				case useFireBall:
					if (selected[useFireBall])break;
					if (vapoonID != fireball)
					{
						SkillUI[i]->setTexture("model/skill/useFireBall.png");
						SkillUI[i]->setTag(useFireBall);
						isdecided = true;
						selected[useFireBall] = true;
						break;
					}
					continue;


				}
				log("%d %d", isdecided, selection);

			}
			
		}
		for (int s = 0; s < 17; s++)
			{
				selected[s] = false;
			}
	}
	void getSkill(skillID id)
	{
		switch (id)
		{
		case addfrontnum:
			vapoon->frontnum++;
			break;
		case addbacknum:
			vapoon->backnum++;
			break;
		case addbesidenum:
			vapoon->besidenum++;
			break;
		case scalevapoon:
			isscale = true;
			break;
		case addDEF:
			add_DEF();
			break;
		case addATT: 
			add_ATT();
			break;
		case addSPD:
			add_SPD();
			break;
		case addHP:
			add_MaxHP();
			break;
		case addXPRate:
			isXPrate = true;
			XPrate = 1.5;
			break;
		case addRange:
			addATT_Range();
			break;
		case addAll:
			add_All();
			break;
		case useCrossBow:
			this->vapoonID = crossbow;
			if (this->getTag() == 0)
			{
				vapoon->VapoonUI->setTexture("scene/gameUI/crossbowUI.png");
				setTexture("model/character/character-crossbow.png");
			}
			vapoon->setproperties(vapoonID);
			break;
		case useGun:
			this->vapoonID = gun;
			if (this->getTag() == 0)
			{
				vapoon->VapoonUI->setTexture("scene/gameUI/gunUI.png");
				setTexture("model/character/character-gun.png");
			}
			vapoon->setproperties(vapoonID);
			break;
		case useFireBall:
			this->vapoonID = fireball;
			if (this->getTag() == 0)
			{
				vapoon->VapoonUI->setTexture("scene/gameUI/fireballUI.png");
				setTexture("model/character/character-fireball.png");
			}
			vapoon->setproperties(vapoonID);
			break;
		case K1A2:
			break;
		case characterCross:
			isCharacterCross = true;
			break;
		case vapoonCross:
			isVapoonCross = true; 
			break;
		}
		/*
			scalevapoon,



			useFireBall,
			K1A2*/
	}
};
#endif ;/* Character_h */