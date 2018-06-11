#include "setname.h"  

USING_NS_CC;
USING_NS_CC_EXT;

Scene* loginscene::createScene()
{
	auto scene = Scene::create();

	auto layer = loginscene::create();

	scene->addChild(layer);

	return scene;
}
bool loginscene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	createBackground();

	initUserBox();

	return true;
}
void loginscene::createBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite * bg = Sprite::create("bg.jpg");
	bg->setScale(2.4f);
	//set transparent
	bg->setOpacity(255);
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	addChild(bg);
}


void loginscene::initUserBox()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto usernameBG = Sprite::create("jingyan.png");
	usernameBG->setScale(1.2f);
	usernameBG->setOpacity(200);

	usernameInput = ui::TextField::create("Input Username Here", "Arial", 30);
	usernameInput->setString(UserDefault::getInstance()->getStringForKey("username", ""));
	usernameInput->setColor(Color3B::BLACK);
	usernameInput->setCursorChar('|');
	usernameInput->setCursorEnabled(true);
	usernameInput->setMaxLength(15);
	usernameInput->setMaxLengthEnabled(true);
	usernameInput->setPosition(usernameBG->getContentSize() / 2);

	usernameBG->addChild(usernameInput);

	usernameBG->setScale(1.5);
	usernameBG->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height / 2 + usernameBG->getContentSize().height * 2));

	addChild(usernameBG);

}




void loginscene::menuBackCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0); n
#endif
}
