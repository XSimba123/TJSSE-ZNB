#include "Scene\LoginScene.h"
Scene* LoginScene::createScene(std::string Name)
{
	auto scene = Scene::create();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = LoginScene::create();
	layer->_playName = Name;
	scene->addChild(layer, -1);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoginSceneScene.cpp\n");
}

bool LoginScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("scene/startMenu/roombg.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);


	auto start_server = MenuItemImage::create(
		"scene/gameUI/Start_Server.png",
		"scene/gameUI/Start_Server2.png",
		CC_CALLBACK_1(LoginScene::Start_server, this));
	start_server->setPosition(Director::getInstance()->convertToGL(Vec2(300, 300)));

	auto start_client = MenuItemImage::create(
		"scene/gameUI/Start_Client.png",
		"scene/gameUI/Start_Client2.png",
		CC_CALLBACK_1(LoginScene::Start_client, this));
	start_client->setPosition(Director::getInstance()->convertToGL(Vec2(300, 500)));
	auto begin = MenuItemImage::create(
		"scene/gameUI/begin.png",
		"scene/gameUI/begin2.png",
		CC_CALLBACK_1(LoginScene::begin, this));
	begin->setPosition(Director::getInstance()->convertToGL(Vec2(1050, 480)));

	retoname = MenuItemImage::create("scene/gameUI/TextCancel.png",
		"scene/gameUI/TextCancel2.png", CC_CALLBACK_1(LoginScene::retonamelog, this));
	retoname->setPosition(Director::getInstance()->convertToGL(Vec2(1150, 50)));


	menu = Menu::create(start_server, start_client,begin,retoname, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);


	portBG = Sprite::create("scene/gameUI/port.png");
	portBG->setScale(1.2f);
	portBG->setOpacity(200);
	portInput = ui::TextField::create("Input port", "arial", 20);
	portInput->setColor(Color3B::BLACK);
	portInput->setCursorChar('|');
	portInput->setCursorEnabled(true);
	portInput->setMaxLength(5);
	portInput->setMaxLengthEnabled(true);
	portInput->setPosition(portBG->getContentSize()-Size(75,25));
	portInput->addEventListener(CC_CALLBACK_2(LoginScene::porttextFieldEvent, this));
	portBG->addChild(portInput);

	portBG->setScale(1.5);
	portBG->setPosition(Vec2(700, 280));

	addChild(portBG);

	ipBG = Sprite::create("scene/gameUI/ip.png");
	ipBG->setScale(1.2f);
	ipInput = ui::TextField::create("Input ip", "arial", 20);
	ipInput->setColor(Color3B::BLACK);
	ipInput->setCursorChar('|');
	ipInput->setCursorEnabled(true);
	ipInput->setMaxLength(20);
	ipInput->setMaxLengthEnabled(true);
	ipInput->setPosition(portBG->getContentSize() - Size(35, 25));
	ipInput->addEventListener(CC_CALLBACK_2(LoginScene::iptextFieldEvent, this));
	ipBG->addChild(ipInput);

	ipBG->setScale(1.5);
	ipBG->setPosition(Vec2(750, 480));

	addChild(ipBG);



	
	this->scheduleUpdate();



	return true;
}
void LoginScene::Start_server(cocos2d::Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	if (!serverisbuil)
	{
		service = SocketServer::create();
		if (service->startServer())
		{
			serverisbuil = true;
			char a[1024];
			char hostName[256];
			gethostname(hostName, sizeof(hostName));
			struct hostent* hostInfo = gethostbyname(hostName);
			char* aip = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
			sprintf(a, "You build the server successfully.\nYour IP is %s and your port is 8000", aip);
			serverLabel = Label::create(a, "Aril", 36);
			serverLabel->setPosition(Vec2(640, 700));
			serverLabel->setColor(Color3B(0, 0, 0));
			addChild(serverLabel);
			//log("you build the server successfully.");
		}
	}
	else
	{
		if (label != nullptr)this->removeChild(label);
		label = Label::create("you have been built one.", "Aril", 36);
		label->setPosition(Vec2(640, 650));
		label->setColor(Color3B(255, 0, 0));
		addChild(label);
		this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([&]() {     //
			this->removeChild(label);
		}), NULL));
	}
}
void LoginScene::Start_client(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	if (!clientisbuil)
	{
		myplat = new SocketClient();
		int i;
		sscanf(port.c_str(), "%d", &i);
		
		if (myplat->connectServer(ip.c_str(), i))
		{
			clientisbuil = true;
			char a[1024];
			char hostName[256];
			gethostname(hostName, sizeof(hostName));
			struct hostent* hostInfo = gethostbyname(hostName);
			char* aip = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
			sprintf(a, "You connect with  ""%s"" successfully.", aip);
			serverLabel = Label::create(a, "Aril", 36);
			serverLabel->setPosition(Vec2(640, 100));
			serverLabel->setColor(Color3B(255, 0, 0));
			addChild(serverLabel);
			char b[20];
			sprintf(b, "CN %s", _playName.c_str());
			myplat->sendMessage(a, strlen(b) + 1);
		}
	}
	else
	{
		if (label != nullptr)this->removeChild(label);
		label = Label::create("you have been connected.", "Aril", 36);
		label->setPosition(Vec2(640, 150));
		label->setColor(Color3B(255, 0, 0));
		addChild(label);
		this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([&]() {     //
			this->removeChild(label);
		}), NULL));
	}
	
}
void LoginScene::update(float delta)
{
	
	if (clientisbuil)
	{
		if (myplat->canbegin)
		{
			if (!serverisbuil)
			{
				auto scene = OrdinaryScene::createScene(_playName, myplat);
				Director::getInstance()->replaceScene(scene);
			}
			else
			{
				auto scene = OrdinaryScene::createScene(_playName, myplat,service);
				Director::getInstance()->replaceScene(scene);
			}
		}
	}
}
void LoginScene::begin(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	if (myplat->totalNum >= 1 && clientisbuil && serverisbuil)
	{
		log("canbegin");
		service->sendMessage("begin", 6);
		
	}
}
void LoginScene::porttextFieldEvent(Ref* sender, ui::TextField::EventType event)
{
	switch (event) {
	case TextField::EventType::INSERT_TEXT:
		port = portInput->getString();
		break;
	case TextField::EventType::DELETE_BACKWARD:
		port = portInput->getString();
		break;
	}
}
void LoginScene::iptextFieldEvent(Ref* sender, ui::TextField::EventType event)
{
	switch (event) {
	case TextField::EventType::INSERT_TEXT:
		ip = ipInput->getString();
		break;
	case TextField::EventType::DELETE_BACKWARD:
		ip = ipInput->getString();
		break;
	}
}
void LoginScene::retonamelog(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	if (serverisbuil)service->close();
	auto a=GameScene::createScene();
	Director::getInstance()->replaceScene(a);
}