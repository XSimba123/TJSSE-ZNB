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
USING_NS_CC;

class OrdinaryScene : public cocos2d::Layer
{
	cocos2d::TMXTiledMap* map;
	cocos2d::TMXLayer* wall;
	cocos2d::Sprite *character;
	cocos2d::Sprite *enemy;
	cocos2d::Sprite *arrow;
	cocos2d::Sprite *heart[25];
	cocos2d::Sprite *dot[2000];
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	bool isAttacking;
	bool beAttacken=false;
	int heartMask =0x01;
	int dotMask = 0x01;
	int characterMask = 0x01;
	int arrowMask = 0x01;
	int enemyMask = 0x01;
	int coloroldtime;
	bool beginblink;
	Vec2 direction;
public:
	struct timeval now;
	int heartoldTime[25];
	bool heartiseaten[25] = { false };
	bool heartisloading[25] = { false };
	int dotoldTime[2000];
	bool dotiseaten[2000] = { false };
	bool dotisloading[2000] = { false };


	bool onContactBegin(const PhysicsContact& contact);
	static cocos2d::Scene* createScene();
	virtual bool init();

	// a selector callback

	// implement the "static create()" method manually

	CREATE_FUNC(OrdinaryScene);


	void update(float delta) override;
	virtual bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	void keyPressedDuration(EventKeyboard::KeyCode code);


	void initHeart();
	void initStar();



private:
	Point preTouchPoint;      // 上一个触摸点
	Point currTouchPoint;     // 当前触摸点


private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
};

#endif // __OrdinaryScene_SCENE_H__
#pragma once
