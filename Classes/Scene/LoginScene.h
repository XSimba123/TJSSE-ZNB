#pragma once
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

#ifndef __LoginScene_SCENE_H__
#define __LoginScene_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Scene\OrdinaryScene.h"
#include "Scene\GameScene.h"
#include "ui\CocosGUI.h"
#include "NetWork\SocketServer.h"
#include "NetWork\SocketClient.h"
using namespace ui;
USING_NS_CC;
class LoginScene : public cocos2d::Layer
{
public:
	std::string _playName;
	virtual bool init();
	std::string port;
	std::string ip;
	Menu* menu;
	MenuItemImage* retoname;
	Sprite* title;
	Sprite* portBG;
	TextField* portInput;
	Sprite* ipBG;
	TextField* ipInput;
	SocketClient* myplat;
	SocketServer* service;
	Label* serverLabel;
	Label* clientLabel;
	Label* label;
	bool serverisbuil=false;
	bool clientisbuil = false;
	void update(float delta) override;
	// a selector callback
	/*void menuCloseCallback(cocos2d::Ref* pSender*/

	static cocos2d::Scene* createScene(std::string Name);


	void porttextFieldEvent(Ref* sender, ui::TextField::EventType event);
	void iptextFieldEvent(Ref* sender, ui::TextField::EventType event);
	void Start_server(cocos2d::Ref* pSender);
	void Start_client(Ref* pSender);
	void begin(Ref* pSender);
	void retonamelog(Ref* pSender);
	/*void LoginScene::TextConfirm(Ref* pSender);
	void LoginScene::TextCancel(Ref* pSender);*/
	// implement the "static create()" method manually
	CREATE_FUNC(LoginScene);
};

#endif // __LoginScene_SCENE_H__
#pragma once
