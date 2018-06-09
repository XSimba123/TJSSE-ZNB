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

#include "Scene\OrdinaryScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* OrdinaryScene::createScene()
{
	return OrdinaryScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in OrdinarySceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool OrdinaryScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	map = TMXTiledMap::create("mainmap.tmx");
	this->addChild(map,0,100);
	/*auto group = map->getObjectGroup("wall");
	ValueMap spawnPoint = group->getObject("character");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();
*/


	character = Sprite::create("character-forward.png");//创建
	character->setScale(1.2f);
	character->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(character,1);

	this->scheduleUpdate();//加入定时器


	auto listener = EventListenerKeyboard::create();//监听器create
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) //监听键盘的按下
	{
		keys[keyCode] = true;
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) //监听键盘的松开
	{
		keys[keyCode] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	
	return true;
}
void OrdinaryScene::update(float delta) //每帧进行的更新操作
{
	Node::update(delta);
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW, rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	auto upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW, downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(leftArrow))
	{
		keyPressedDuration(leftArrow);
	}
	else if (isKeyPressed(rightArrow))
	{
		keyPressedDuration(rightArrow);
	}
	else if (isKeyPressed(upArrow))
	{
		keyPressedDuration(upArrow);
	}
	else if (isKeyPressed(downArrow))
	{
		keyPressedDuration(downArrow);
	}
}

bool OrdinaryScene::isKeyPressed(EventKeyboard::KeyCode keyCode)//判断是否一直被按住
{
	if (keys[keyCode])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void OrdinaryScene::keyPressedDuration(EventKeyboard::KeyCode code) //精灵进行的操作
{
	int offsetX = 0, offsetY = 0;
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		offsetX = 3;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX = -3;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY = -3;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY = 3;
		break;
	default:
		offsetY = offsetX = 0;
		break;
	}
	auto moveTo = MoveTo::create(0.3, Vec2(map->getPositionX() + offsetX, map->getPositionY() + offsetY));
	map->runAction(moveTo);
}

void OrdinaryScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0); n
#endif

		/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

		//EventCustom customEndEvent("game_scene_close_event");
		//_eventDispatcher->dispatchEvent(&customEndEvent);


}
