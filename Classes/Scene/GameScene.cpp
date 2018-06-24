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

	/*auto wsTEXT = new network::WebSocket();
	wsTEXT->init(*this, "ws://180.160.5.202:4000");
	wsTEXT->send("Hello caonima!");*/


	auto background = Sprite::create("scene/startMenu/background.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);


	auto photo1 = Sprite::create("scene/startMenu/photo1.png");
	photo1->setPosition(Vec2(origin.x+180,
		origin.y + visibleSize.height * 1 / 4));
	this->addChild(photo1);

	auto photo2 = Sprite::create("scene/startMenu/photo2.png");
	photo2->setPosition(Vec2(origin.x + visibleSize.width - 300,
		origin.y + visibleSize.height * 3 / 4-50));
	this->addChild(photo2);

	title = Sprite::create("scene/startMenu/title.png");
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height * 3 / 4));
	this->addChild(title);

	auto ac1 = Sequence::create(ScaleTo::create(0.25, 1.5), ScaleTo::create(0.25, 1), NULL);
	auto ac2 = Sequence::create(RotateBy::create(0.5, 360), Blink::create(0.8, 3), NULL);
	auto TotalAct = Spawn::create(ac1, ac2, NULL);
	title->runAction(TotalAct);

	auto start_ordinary = MenuItemImage::create(
		"scene/startMenu/start_ordinary-normal.png",
		"scene/startMenu/start_ordinary-selected.png",
		CC_CALLBACK_1(GameScene::MenuStartNormal,this));
	start_ordinary->setPosition(Director::getInstance()->convertToGL(Vec2(0, 450)));

	auto start_survival = MenuItemImage::create(
		"scene/startMenu/start_survival-normal.png",
		"scene/startMenu/start_survival-selected.png",
		CC_CALLBACK_1(GameScene::MenuStartSurvival, this));
	start_survival->setPosition(Director::getInstance()->convertToGL(Vec2(1280, 450)));

	start_menu = Menu::create(start_ordinary, start_survival,NULL);
	start_menu->setPosition(Vec2::ZERO);
	this->addChild(start_menu);




	_TextConfirm = MenuItemImage::create(
		"scene/gameUI/TextConfirm.png",
		"scene/gameUI/TextConfirm2.png",
		CC_CALLBACK_1(GameScene::TextConfirm, this));
	_TextConfirm->setPosition(Director::getInstance()->convertToGL(Vec2(425,1000)));

	_TextCancel = MenuItemImage::create(
		"scene/gameUI/TextCancel.png",
		"scene/gameUI/TextCancel2.png",
		CC_CALLBACK_1(GameScene::TextCancel, this));
	_TextCancel->setPosition(Director::getInstance()->convertToGL(Vec2(825, 1000)));

	_TextConfirm->runAction(FadeOut::create(0));
	_TextCancel->runAction(FadeOut::create(0));

	Text_menu = Menu::create(_TextConfirm, _TextCancel, NULL);
	Text_menu->setPosition(Vec2::ZERO);
	this->addChild(Text_menu);

	Text_menu->setVisible(false);



	auto button_act_left = MoveTo::create(0.45, Director::getInstance()->convertToGL(Vec2(450, 450)));
	auto button_act_right = MoveTo::create(0.45, Director::getInstance()->convertToGL(Vec2(800, 450)));
	start_ordinary->runAction(button_act_left);
	start_survival->runAction(button_act_right);

	return true;
}

void GameScene::MenuStartNormal(Ref* pSender)
{
	//Director::getInstance()->replaceScene(map);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	start_menu->runAction(FadeOut::create(1));
	title->runAction(FadeOut::create(0.5));
	Text_menu->setVisible(true);
	_TextConfirm->runAction(Sequence::create(DelayTime::create(0.3), 
		Spawn::create(FadeIn::create(0.6), MoveTo::create(0.6, Director::getInstance()->convertToGL(Vec2(425, 525))), NULL), NULL));

	_TextCancel->runAction(Sequence::create(DelayTime::create(0.3),
		Spawn::create(FadeIn::create(0.6), MoveTo::create(0.6, Director::getInstance()->convertToGL(Vec2(750, 525))), NULL), NULL));


	auto Input=Sprite::create("scene/gameUI/Input.png");
	Input->setPosition(Director::getInstance()->convertToGL(Vec2(500, 200)));
	addChild(Input);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	usernameBG = Sprite::create("scene/gameUI/TextBG.png");
	usernameBG->setScale(1.2f);
	usernameBG->setOpacity(200);
	usernameInput = ui::TextField::create("Input Username Here", "arial", 24);
	usernameInput->setColor(Color3B::BLACK);
	usernameInput->setCursorChar('|');
	usernameInput->setCursorEnabled(true);
	usernameInput->setMaxLength(15);
	usernameInput->setMaxLengthEnabled(true);
	usernameInput->setPosition(usernameBG->getContentSize() / 2);
	usernameInput->addEventListener(CC_CALLBACK_2(GameScene::textFieldEvent, this));

	Input->runAction(FadeOut::create(0));
	Input->runAction(Sequence::create(DelayTime::create(0.3), FadeIn::create(0.3), NULL));
	usernameBG->runAction(FadeOut::create(0));
	usernameBG->runAction(Sequence::create(DelayTime::create(0.4), FadeIn::create(0.6), NULL));
	usernameInput->runAction(FadeOut::create(0));
	usernameInput->runAction(Sequence::create(DelayTime::create(0.6), FadeIn::create(0.3), NULL));
	usernameBG->addChild(usernameInput);

	usernameBG->setScale(1.5);
	usernameBG->setPosition(Vec2(600,400));

	addChild(usernameBG);

}

void GameScene::textFieldEvent(Ref* sender, ui::TextField::EventType event)
{
	switch (event) {
	case TextField::EventType::INSERT_TEXT:
		_playerName = usernameInput->getString();
		break;
	case TextField::EventType::DELETE_BACKWARD:
		_playerName = usernameInput->getString();
		break;
	}
}
void GameScene::TextConfirm(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	auto scene = LoginScene::createScene(_playerName);
	Director::getInstance()->replaceScene(scene);
}
void GameScene::TextCancel(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	    auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
}


void GameScene::MenuStartSurvival(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
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
