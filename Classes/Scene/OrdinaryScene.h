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

#ifndef __OrdinaryScene_SCENE_H__
#define __OrdinaryScene_SCENE_H__

#include "cocos2d.h"
#include "Model\character.h"
#include "NetWork\SocketServer.h"
#include "NetWork\SocketClient.h"
#include "Scene\GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;
#define cos15 0.9659258263
#define sin15 0.2588190451

class OrdinaryScene : public cocos2d::Layer
{
	cocos2d::TMXTiledMap* map;
	cocos2d::TMXLayer* wall;
	cocos2d::Sprite *heart[36];
	cocos2d::Sprite *dot[500];
	cocos2d::Sprite* nowlevelUI;
	cocos2d::Sprite* baselevelUI;
	cocos2d::Label* level;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	bool isAttacking[3][4];
	bool beAttacken[3][4] = { false };
	bool isEnemyAttacking[3][4];
	bool beEnemyAttacken[3][4] = { false };
	int heartMask =0x01;
	int dotMask = 0x01;
	int characterMask = 0x01;
	int arrowMask = 0x01;
	int enemyarrowMask = 0x01;
	int enemyMask = 0x01;
	int coloroldtime;
	int begintime=0;
	int Enemycoloroldtime;
	int blinktime=0;
	int Enemyblinktime = 0;
	int deltatime=0;
	Vec2 FrontDirection;
	Vec2 FrontLeftDirection;
	Vec2 FrontRightDirection;
	Vec2 BackDirection;
	Vec2 BackLeftDirection;
	Vec2 BackRightDirection;
	Vec2 BesideDirection;
	Vec2 BesideFrontDirection;
	Vec2 BesideBackDirection;
	PhysicsBody* heartbody[36];
	PhysicsBody* dotbody[500];
	PhysicsBody* characterbody;
	Sprite* arrow[3][4];
	Menu* Text_menu;
	PhysicsBody* arrowbody[3][4];
	Sprite* enemyarrow[3][4];
	PhysicsBody* enemyarrowbody[3][4];
public:
	bool isStart=false;
	Sprite * number;
	int countnumber=1;
	int totalTime = 300000;
	std::string     _playName;
	Character * character;
	Sprite * circle;
	Layer * CharacterLayer;
	Layer * MapLayer;
	Layer * MenuLayer;
	struct timeval now;
	int heartoldTime[36];
	bool heartiseaten[36] = { false };
	bool heartisloading[36] = { false };
	int dotoldTime[500];
	bool dotiseaten[500] = { false };
	bool dotisloading[500] = { false };
	int circletime;
	int circleblinktime;
	int circlewaittime;
	bool initServer = false;
	bool initBoard = false;
	bool isover = false;
	
	Label* My_Name;
	Label* Enemy_Name;
	Label* My_Num;
	Label* Enemy_Num;
	//void complete(HttpClient *client, HttpResponse* response);
	bool onContactBegin(const PhysicsContact& contact);
	static cocos2d::Scene* createScene(std::string Name, SocketClient* _myplat, SocketServer* _service);
	static cocos2d::Scene* OrdinaryScene::createScene(std::string Name, SocketClient* _myplat);
	virtual bool init();
	void updateBoard();
	Label* timeminder;
	// a selector callback
	int knum=0;
	// implement the "static create()" method manually


	void exit(Ref* pSender);
	void update(float delta) override;


	virtual bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	void keyPressedDuration(EventKeyboard::KeyCode code);


	void initHeart();
	void initStar();

	SocketClient* myplat;
	SocketServer* service;
	CREATE_FUNC(OrdinaryScene);


private:
	Point preTouchPoint;      // 上一个触摸点
	Point currTouchPoint;     // 当前触摸点


private:
	


	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif // __OrdinaryScene_SCENE_H__
