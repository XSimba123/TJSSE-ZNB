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

#include "Scene\GameScene.h"
#include "SimpleAudioEngine.h"
#include "Scene\OrdinaryScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);


	auto photo1 = Sprite::create("photo1.png");
	photo1->setPosition(Vec2(origin.x+180,
		origin.y + visibleSize.height * 1 / 4));
	this->addChild(photo1);

	auto photo2 = Sprite::create("photo2.png");
	photo2->setPosition(Vec2(origin.x + visibleSize.width - 300,
		origin.y + visibleSize.height * 3 / 4-50));
	this->addChild(photo2);

	auto title = Sprite::create("title.png");
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height * 3 / 4));
	this->addChild(title);

	auto ac1 = Sequence::create(ScaleTo::create(0.25, 1.5), ScaleTo::create(0.25, 1), NULL);
	auto ac2 = Sequence::create(RotateBy::create(0.5, 360), Blink::create(0.8, 3), NULL);
	auto TotalAct = Spawn::create(ac1, ac2, NULL);
	title->runAction(TotalAct);

	auto start_ordinary = MenuItemImage::create(
		"start_ordinary-normal.png",
		"start_ordinary-selected.png",
		CC_CALLBACK_1(GameScene::MenuStartNormal,this));
	start_ordinary->setPosition(Director::getInstance()->convertToGL(Vec2(0, 450)));

	auto start_survival = MenuItemImage::create(
		"start_survival-normal.png",
		"start_survival-selected.png",
		CC_CALLBACK_1(GameScene::MenuStartSurvival, this));
	start_survival->setPosition(Director::getInstance()->convertToGL(Vec2(1280, 450)));

	auto start_menu = Menu::create(start_ordinary, start_survival,NULL);
	start_menu->setPosition(Vec2::ZERO);
	this->addChild(start_menu);

	auto button_act_left = MoveTo::create(0.45, Director::getInstance()->convertToGL(Vec2(450, 450)));
	auto button_act_right = MoveTo::create(0.45, Director::getInstance()->convertToGL(Vec2(800, 450)));
	start_ordinary->runAction(button_act_left);
	start_survival->runAction(button_act_right);

	return true;
}

void GameScene::MenuStartNormal(Ref* pSender)
{
	auto map = OrdinaryScene::createScene();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
	Director::getInstance()->replaceScene(map);

}
void GameScene::MenuStartSurvival(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");

}
void GameScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);n
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
