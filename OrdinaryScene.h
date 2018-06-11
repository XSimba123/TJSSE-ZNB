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
	cocos2d::Sprite *character;
	cocos2d::Sprite *arrow;
	cocos2d::Sprite *heart[81];
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	bool isAttacking;

	int heartMask = 1 << 0;
	int characterMask = 1 << 1;
public:
	struct timeval now;
	int heartoldTime[81];
	bool heartiseaten[81] = { false };
	bool heartisloading[81] = { false };

	void foodFadeout(int i);
	void foodyanchi(Sprite* food);

	bool onContactBegin(const PhysicsContact& contact);
	static cocos2d::Scene* createScene();
	virtual bool init();

	// a selector callback

	void MenuStartNormal(cocos2d::Ref* pSender);
	// implement the "static create()" method manually

	CREATE_FUNC(OrdinaryScene);

	void rotatePlayer(Point touchPoint);

	void update(float delta) override;
	void updateofMouse(float delta);
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
