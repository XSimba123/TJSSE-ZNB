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
#include "NetWork\SocketServer.h"
#include "NetWork\SocketClient.h"
USING_NS_CC;
Scene* OrdinaryScene::createScene(std::string Name, SocketClient* _myplat, SocketServer* _service)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = OrdinaryScene::create();
	layer->_playName = Name;
	layer->myplat = _myplat;
	layer->service = _service;
	scene->addChild(layer, -1);
	return scene;
}
Scene* OrdinaryScene::createScene(std::string Name, SocketClient* _myplat)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = OrdinaryScene::create();
	layer->_playName = Name;
	layer->myplat = _myplat;
	scene->addChild(layer, -1);
	return scene;
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
	if (!Layer::init())
	{
		return false;
	}

	//network

	CharacterLayer=Layer::create();
	MapLayer = Layer::create();
	MenuLayer = Layer::create();


	character = Character::create("model/character/character-crossbow.png");
	character->setAnchorPoint(Vec2(0.5, 0.3));
	character->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	CharacterLayer->addChild(character, 1);

	auto audioengine = SimpleAudioEngine::getInstance();
	audioengine->playBackgroundMusic("sound/BGM/Kevin MacLeod - Monkeys Spinning Monkeys.mp3", true);
	this->addChild(CharacterLayer, 1);
	this->addChild(MenuLayer, 2);
	this->addChild(MapLayer, -1);
	map = TMXTiledMap::create("scene/Map/mainmap.tmx");
	MapLayer->addChild(map,0,100);
	wall = map->getLayer("wall");
	MapLayer->setPosition(Vec2(origin.x + visibleSize.width / 2-map->getMapSize().width*30, origin.y + visibleSize.height / 2-map->getMapSize().height*30));





	//circle = Sprite::create("scene/Map/circle.png");
	//circle->setPosition(Vec2(3000, 3000));
	//MapLayer->addChild(circle, 3);
	//std::thread t1(&SocketServer::myThread, this, 10, 20);//创建一个分支线程，回调到myThread函数里   
	//t1.join();




	initHeart();
	initStar();
	

	character->vapoon->VapoonUI->setPosition(Vec2(160, 160));
	character->vapoon->VapoonUI->setTexture("scene/gameUI/crossbowUI.png");
	MenuLayer->addChild(character->vapoon->VapoonUI);

	auto RankingBoard = Sprite::create("scene/gameUI/RankingBoard.png");
	RankingBoard->setAnchorPoint(Vec2(1, 1));
	RankingBoard->setPosition(Vec2(1270, 750));
	MenuLayer->addChild(RankingBoard, 0);

	baselevelUI = Sprite::create("scene/gameUI/levelbase.png");
	baselevelUI->setAnchorPoint(Vec2(0, 0.5));
	baselevelUI->setPosition(Vec2(550, 700));
	MenuLayer->addChild(baselevelUI,0);

	nowlevelUI = Sprite::create("scene/gameUI/levellight.png", Rect(0,0,0,0));
	nowlevelUI->setAnchorPoint(Vec2(0, 0.5));
	nowlevelUI->setPosition(Director::getInstance()->convertToUI(Vec2(550, 68)));
	MenuLayer->addChild(nowlevelUI, 1);

	Button* button = Button::create("scene/gameUI/exit.png", "scene/gameUI/exit.png");
	button->setPosition(Director::getInstance()->convertToUI(Vec2(1220, 400)));
	button->addTouchEventListener(CC_CALLBACK_1(OrdinaryScene::exit, this));
	this->addChild(button);

	TTFConfig ttf1("fonts/Quicksand-Bold.ttf", 30, GlyphCollection::DYNAMIC);
	level = Label::createWithTTF(ttf1,"Lv . " + character->getLevel());
	//level->setColor(Color3B(0, 0, 0));
	level->setPosition(Vec2(410, 705));
	MenuLayer->addChild(level);

	auto no_1 = Sprite::create("1.png");
	no_1->setPosition(Vec2(960, 660));
	MenuLayer->addChild(no_1);
	auto no_2 = Sprite::create("2.png");
	no_2->setPosition(Vec2(960, 610));
	MenuLayer->addChild(no_2);

	auto kill_1 = Sprite::create("kill.png");
	kill_1->setPosition(Vec2(1160, 660));
	MenuLayer->addChild(kill_1);
	auto kill_2 = Sprite::create("kill.png");
	kill_2->setPosition(Vec2(1160, 610));
	MenuLayer->addChild(kill_2);

	TTFConfig ttf("fonts/Marker Felt.ttf", 30, GlyphCollection::DYNAMIC);
	timeminder = Label::createWithTTF(ttf, "Time:\n05:00");
	timeminder->setPosition(Vec2(100, 700));
	MenuLayer->addChild(timeminder);

	character->maxHpUI->setPosition(Vec2(character->getPositionX(), -60+character->getPositionY()));
	MenuLayer->addChild(character->maxHpUI);


	character->nowHpUI->setPosition(character->maxHpUI->getPosition()-Vec2(character->get_HP()*15,0));
	MenuLayer->addChild(character->nowHpUI);

	for (int i = 0; i < 3; i++)MenuLayer->addChild(character->SkillUI[i]);
	

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(OrdinaryScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


	characterbody = PhysicsBody::createBox(Size(80,100));
	characterbody->setDynamic(false);
	character->setPhysicsBody(characterbody);
	character->setTag(0);
	characterbody->setCategoryBitmask(characterMask);
	characterbody->setCollisionBitmask(heartMask | characterMask | dotMask);
	characterbody->setContactTestBitmask(heartMask | characterMask | dotMask);

	//auto edgeSpace = Sprite::create();
	//PhysicsBody* boundbody==PhysicsBody


	this->scheduleUpdate();//加入定时器


	auto listener = EventListenerKeyboard::create();//监听器create
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) //监听键盘的按下
	{
		
		keys[keyCode] = true;
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) //监听键盘的松开
	{
		keys[keyCode] = false;
		auto button1 = EventKeyboard::KeyCode::KEY_1;
		auto button2 = EventKeyboard::KeyCode::KEY_2;
		auto button3 = EventKeyboard::KeyCode::KEY_3;
		auto button4 = EventKeyboard::KeyCode::KEY_4;
		if (character->canSelect == 2 && (keyCode == button1 || keyCode == button2 || keyCode == button3|| keyCode == button4))
		{
			if (keyCode == button1)
			{
				character->getSkill((skillID)character->SkillUI[0]->getTag());
				character->SkillUI[0]->runAction(Spawn::create(Blink::create(0.1, 1),
					Sequence::create(ScaleTo::create(0.05, 1.2), ScaleTo::create(0.05, 1.0), NULL), NULL));
				char send[1024];
				sprintf(send, "K %d", character->SkillUI[0]->getTag());
				myplat->sendMessage(send, strlen(send) + 1);

			}
			else if (keyCode == button2)
			{
				character->getSkill((skillID)character->SkillUI[1]->getTag());
				character->SkillUI[1]->runAction(Spawn::create(Blink::create(0.1, 1),
					Sequence::create(ScaleTo::create(0.05, 1.2), ScaleTo::create(0.05, 1.0), NULL), NULL));
				char send[1024];
				sprintf(send, "K %d", character->SkillUI[1]->getTag());
				myplat->sendMessage(send, strlen(send) + 1);

			}
			else if (keyCode == button3)
			{
				character->getSkill((skillID)character->SkillUI[2]->getTag());
				character->SkillUI[2]->runAction(Spawn::create(Blink::create(0.1, 1),
					Sequence::create(ScaleTo::create(0.05, 1.2), ScaleTo::create(0.05, 1.0), NULL), NULL));
				char send[1024];
				sprintf(send, "K %d", character->SkillUI[2]->getTag());
				myplat->sendMessage(send, strlen(send) + 1);

			}
			this->runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create([&]() {     //
				for (int j = 0; j < 3; j++)character->SkillUI[j]->setVisible(false);
				/*MenuLayer->removeChild(character->SkillUI[0]);
				MenuLayer->removeChild(character->SkillUI[1]);
				MenuLayer->removeChild(character->SkillUI[2]);*/
				character->canSelect = 0;
			}), NULL));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



	auto myMouseListener = EventListenerMouse::create();//创建事件监听器鼠标事件
														//鼠标移动
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myMouseListener, this);
	myMouseListener->onMouseMove = [=](Event *event)
	{
		if (character->movable)
		{
			auto e = (EventMouse*)event;
			auto playerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
			Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
			Point vector = mousePos - playerPos;
			auto rotateRadians = vector.getAngle();
			auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians) + 90;
			char send[1024];
			sprintf(send, "CR %f", rotateDegrees);
			myplat->sendMessage(send, strlen(send) + 1);

			character->runAction(RotateTo::create(0, rotateDegrees));
		}
	};

	myMouseListener->onMouseUp = [=](Event *event)
	{
		if (character->movable&&!isAttacking[0][0])
		{

			for (int i = 0; i < 3; i++)
			{
				int k;
				if (i == 0)k = character->vapoon->frontnum;
				else if (i == 1)k = character->vapoon->besidenum*2;
				else k = character->vapoon->backnum;
				for (int j = 0; j < k; j++)
				{
					if (character->vapoonID == crossbow)arrow[i][j] = Sprite::create("model/vapoon/bullet/arrow.png");
					else if (character->vapoonID == gun)arrow[i][j] = Sprite::create("model/vapoon/bullet/bullet.png");
					else if (character->vapoonID == fireball)arrow[i][j] = Sprite::create("model/vapoon/bullet/fire.png");
					if (character->isscale)arrow[i][j]->setScale(1.5f);
					arrowbody[i][j] = PhysicsBody::createBox(arrow[0][0]->getContentSize());
					arrowbody[i][j]->setDynamic(false);            //
					arrow[i][j]->setPhysicsBody(arrowbody[i][j]); //
					arrow[i][j]->setTag(50+3*i+j);                     //
					isAttacking[i][j] = true;
					arrowbody[i][j]->setCategoryBitmask(arrowMask);
					arrowbody[i][j]->setCollisionBitmask(heartMask | characterMask | dotMask | arrowMask);
					arrowbody[i][j]->setContactTestBitmask(heartMask | characterMask | dotMask | arrowMask);
					MapLayer->addChild(arrow[i][j], 2);
				}
			}
			auto e = (EventMouse*)event;
			auto playerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
			Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());

			FrontDirection = mousePos - playerPos;
			auto rotateRadians = FrontDirection.getAngle();
			auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians) + 90;
			//sin 15=0.2588190451,cos15=0.9659258263
			FrontDirection = FrontDirection / FrontDirection.getLength();
			FrontLeftDirection = Vec2(FrontDirection.x*cos15 + FrontDirection.y*sin15, -FrontDirection.x*sin15 + FrontDirection.y*cos15);
			FrontRightDirection = Vec2(FrontDirection.x*cos15 - FrontDirection.y*sin15, FrontDirection.x*sin15 + FrontDirection.y*cos15);
			BackDirection = -FrontDirection;
			BackLeftDirection = -FrontLeftDirection;
			BackRightDirection = -FrontRightDirection;
			BesideDirection = Vec2(FrontDirection.y, -FrontDirection.x);
			BesideFrontDirection = Vec2(BesideDirection.x*cos15 + BesideDirection.y*sin15, -BesideDirection.x*sin15 + BesideDirection.y*cos15);
			BesideBackDirection = Vec2(BesideDirection.x*cos15 - BesideDirection.y*sin15, BesideDirection.x*sin15 + BesideDirection.y*cos15);
			char send[1024];
			sprintf(send, "A %d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", character->vapoon->frontnum, character->vapoon->besidenum, character->vapoon->backnum,
				FrontDirection.x, FrontDirection.y, FrontLeftDirection.x, FrontLeftDirection.y, FrontRightDirection.x, FrontRightDirection.y,
				BackDirection.x, BackDirection.y, BackLeftDirection.x, BackLeftDirection.y, BackRightDirection.x, BackRightDirection.y,
				BesideDirection.x, BesideDirection.y, BesideFrontDirection.x, BesideFrontDirection.y, BesideBackDirection.x, BesideBackDirection.y, rotateDegrees);
			myplat->sendMessage(send, strlen(send) + 1);
			if (character->vapoon->frontnum == 1)
			{
				arrow[0][0]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150);
				FrontDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[0][0]->runAction(RotateTo::create(0, rotateDegrees));
				arrow[0][0]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontDirection));                                     //
				

				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					for (int s = 0; s < 3; s++)
					{
						for (int t = 0; t < 4; t++)isAttacking[s][t] = false;
					}
					MapLayer->removeChild(arrow[0][0], true);
					
				}), NULL));
			}
			else if (character->vapoon->frontnum == 2)
			{
				arrow[0][0]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150+ BesideDirection*15);
				arrow[0][1]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 - BesideDirection * 15);
				FrontDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[0][0]->runAction(RotateTo::create(0, rotateDegrees));
				arrow[0][1]->runAction(RotateTo::create(0, rotateDegrees));
				arrow[0][0]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontDirection));   
				arrow[0][1]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontDirection)); //


				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					for (int s = 0; s < 3; s++)
					{
						for (int t = 0; t < 4; t++)isAttacking[s][t] = false;
					}
					MapLayer->removeChild(arrow[0][0], true);
					MapLayer->removeChild(arrow[0][1], true);
				}), NULL));
			}
			else if (character->vapoon->frontnum == 3)
			{
				arrow[0][0]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150  );
				arrow[0][1]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 + BesideDirection * 10);
				arrow[0][2]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 - BesideDirection * 10);
				FrontDirection *= character->getATT_Range() + character->vapoon->Range;
				FrontLeftDirection *= character->getATT_Range() + character->vapoon->Range;
				FrontRightDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[0][0]->runAction(RotateTo::create(0, rotateDegrees));
				arrow[0][1]->runAction(RotateTo::create(0, rotateDegrees+15));
				arrow[0][2]->runAction(RotateTo::create(0, rotateDegrees-15));
				arrow[0][0]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontDirection));
				arrow[0][1]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontLeftDirection));
				arrow[0][2]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontRightDirection));

				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					for (int s = 0; s < 3; s++)
					{
						for (int t = 0; t < 4; t++)isAttacking[s][t] = false;
					}
					MapLayer->removeChild(arrow[0][0], true);
					MapLayer->removeChild(arrow[0][1], true);
					MapLayer->removeChild(arrow[0][2], true);
				}), NULL));

			}
			else if (character->vapoon->frontnum == 4)
			{
				arrow[0][0]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 + BesideDirection * 30);
				arrow[0][1]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 + BesideDirection * 10);
				arrow[0][2]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 - BesideDirection * 10);
				arrow[0][3]->setPosition(character->getPosition() - MapLayer->getPosition() + FrontDirection * 150 - BesideDirection * 30);
				FrontDirection *= character->getATT_Range() + character->vapoon->Range;
				FrontLeftDirection *= character->getATT_Range() + character->vapoon->Range;
				FrontRightDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[0][0]->runAction(RotateTo::create(0, rotateDegrees));
				arrow[0][1]->runAction(RotateTo::create(0, rotateDegrees + 15));
				arrow[0][2]->runAction(RotateTo::create(0, rotateDegrees - 15));
				arrow[0][3]->runAction(RotateTo::create(0, rotateDegrees));
				arrow[0][0]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontDirection));
				arrow[0][1]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontLeftDirection));
				arrow[0][2]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontRightDirection));
				arrow[0][3]->runAction(MoveBy::create(character->vapoon->shootSPD, FrontDirection));

				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					for (int s = 0; s < 3; s++)
					{
						for (int t = 0; t < 4; t++)isAttacking[s][t] = false;
					}
					MapLayer->removeChild(arrow[0][0], true);
					MapLayer->removeChild(arrow[0][1], true);
					MapLayer->removeChild(arrow[0][2], true);
					MapLayer->removeChild(arrow[0][3], true);
				}), NULL));
			}

			if (character->vapoon->backnum == 1)
			{
				arrow[2][0]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50);
				BackDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[2][0]->runAction(RotateTo::create(0, rotateDegrees+180));
				arrow[2][0]->runAction(MoveBy::create(character->vapoon->shootSPD, BackDirection));                                     //



				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					MapLayer->removeChild(arrow[2][0], true);

				}), NULL));
			}
			else if (character->vapoon->backnum == 2)
			{
				arrow[2][0]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 + BesideDirection * 15);
				arrow[2][1]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 - BesideDirection * 15);
				BackDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[2][0]->runAction(RotateTo::create(0, rotateDegrees+180));
				arrow[2][1]->runAction(RotateTo::create(0, rotateDegrees+180));
				arrow[2][0]->runAction(MoveBy::create(character->vapoon->shootSPD, BackDirection));
				arrow[2][1]->runAction(MoveBy::create(character->vapoon->shootSPD, BackDirection)); 



				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					MapLayer->removeChild(arrow[2][0], true);
					MapLayer->removeChild(arrow[2][1], true);
				}), NULL));
			}
			else if (character->vapoon->backnum == 3)
			{
				arrow[2][0]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50);
				arrow[2][1]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 - BesideDirection * 10);
				arrow[2][2]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 + BesideDirection * 10);
				BackDirection *= character->getATT_Range() + character->vapoon->Range;
				BackLeftDirection *= character->getATT_Range() + character->vapoon->Range;
				BackRightDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[2][0]->runAction(RotateTo::create(0, rotateDegrees+180));
				arrow[2][1]->runAction(RotateTo::create(0, rotateDegrees +180+ 15));
				arrow[2][2]->runAction(RotateTo::create(0, rotateDegrees +180- 15));
				arrow[2][0]->runAction(MoveBy::create(character->vapoon->shootSPD, BackDirection));
				arrow[2][1]->runAction(MoveBy::create(character->vapoon->shootSPD, BackLeftDirection));
				arrow[2][2]->runAction(MoveBy::create(character->vapoon->shootSPD, BackRightDirection));
				
				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					MapLayer->removeChild(arrow[2][0], true);
					MapLayer->removeChild(arrow[2][1], true);
					MapLayer->removeChild(arrow[2][2], true);
				}), NULL));

			}
			else if (character->vapoon->backnum == 4)
			{
				arrow[2][0]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 + BesideDirection * 30);
				arrow[2][1]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 - BesideDirection * 10);
				arrow[2][2]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 + BesideDirection * 10);
				arrow[2][3]->setPosition(character->getPosition() - MapLayer->getPosition() + BackDirection * 50 - BesideDirection * 30);
				BackDirection *= character->getATT_Range() + character->vapoon->Range;
				BackLeftDirection *= character->getATT_Range() + character->vapoon->Range;
				BackRightDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[2][0]->runAction(RotateTo::create(0, rotateDegrees + 180));
				arrow[2][1]->runAction(RotateTo::create(0, rotateDegrees + 180 + 15));
				arrow[2][2]->runAction(RotateTo::create(0, rotateDegrees + 180 - 15));
				arrow[2][3]->runAction(RotateTo::create(0, rotateDegrees + 180));
				arrow[2][0]->runAction(MoveBy::create(character->vapoon->shootSPD, BackDirection));
				arrow[2][1]->runAction(MoveBy::create(character->vapoon->shootSPD, BackLeftDirection));
				arrow[2][2]->runAction(MoveBy::create(character->vapoon->shootSPD, BackRightDirection));
				arrow[2][3]->runAction(MoveBy::create(character->vapoon->shootSPD, BackDirection));

				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //

					MapLayer->removeChild(arrow[2][0], true);
					MapLayer->removeChild(arrow[2][1], true);
					MapLayer->removeChild(arrow[2][2], true);
					MapLayer->removeChild(arrow[2][3], true);
				}), NULL));
			}

			if (character->vapoon->besidenum == 1)
			{
				arrow[1][0]->setPosition(character->getPosition() - MapLayer->getPosition() - BesideDirection * 80);
				arrow[1][1]->setPosition(character->getPosition() - MapLayer->getPosition() + BesideDirection * 80);
				BesideDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[1][0]->runAction(RotateTo::create(0, rotateDegrees - 90));
				arrow[1][1]->runAction(RotateTo::create(0, rotateDegrees + 90));
				arrow[1][0]->runAction(MoveBy::create(character->vapoon->shootSPD, -BesideDirection));                                     //
				arrow[1][1]->runAction(MoveBy::create(character->vapoon->shootSPD, BesideDirection));


				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					MapLayer->removeChild(arrow[1][0], true);
					MapLayer->removeChild(arrow[1][1], true);

				}), NULL));
			}
			else if (character->vapoon->besidenum == 2)
			{
				arrow[1][0]->setPosition(character->getPosition() - MapLayer->getPosition() - BesideDirection * 80 + 20 * FrontDirection / FrontDirection.getLength());
				arrow[1][1]->setPosition(character->getPosition() - MapLayer->getPosition() + BesideDirection * 80 + 20 * FrontDirection / FrontDirection.getLength());
				arrow[1][2]->setPosition(character->getPosition() - MapLayer->getPosition() - BesideDirection * 80 + 8 * FrontDirection / FrontDirection.getLength());
				arrow[1][3]->setPosition(character->getPosition() - MapLayer->getPosition() + BesideDirection * 80 + 8 * FrontDirection / FrontDirection.getLength());
				BesideDirection *= character->getATT_Range() + character->vapoon->Range;
				BesideFrontDirection *= character->getATT_Range() + character->vapoon->Range;
				BesideBackDirection *= character->getATT_Range() + character->vapoon->Range;
				arrow[1][0]->runAction(RotateTo::create(0, rotateDegrees - 90+15));
				arrow[1][1]->runAction(RotateTo::create(0, rotateDegrees + 90-15));
				arrow[1][2]->runAction(RotateTo::create(0, rotateDegrees - 90-15));
				arrow[1][3]->runAction(RotateTo::create(0, rotateDegrees + 90+15));
				arrow[1][0]->runAction(MoveBy::create(character->vapoon->shootSPD, -BesideFrontDirection));                                     //
				arrow[1][1]->runAction(MoveBy::create(character->vapoon->shootSPD, BesideBackDirection));
				arrow[1][2]->runAction(MoveBy::create(character->vapoon->shootSPD, -BesideBackDirection));                                     //
				arrow[1][3]->runAction(MoveBy::create(character->vapoon->shootSPD, BesideFrontDirection));


				this->runAction(Sequence::create(DelayTime::create(character->vapoon->waitTime), CallFunc::create([&]() {     //
					MapLayer->removeChild(arrow[1][0], true);
					MapLayer->removeChild(arrow[1][1], true);
					MapLayer->removeChild(arrow[1][2], true);
					MapLayer->removeChild(arrow[1][3], true);

				}), NULL));
			}

			
			if (character->vapoonID == crossbow)CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/Vapoon/shoot crossbow.wav");             //
			else if (character->vapoonID == gun)CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/Vapoon/shoot.wav");
			else if (character->vapoonID == fireball)CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/Vapoon/fire.wav");
		}

	};



	
	return true;
}
int num;
bool OrdinaryScene::onContactBegin(const PhysicsContact& contact)
{
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	Sprite* becontact;
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	if (tagA == 0 && tagA >= 2 && tagB < 50 )
	{
		if (!myplat->heartiseaten[tagB - 11])
		{
			char send[1024];
			sprintf(send, "1 %d", tagB-11);
			myplat->sendMessage("EG", 3);
			character->add_HP(1);
		}
		
	}
	else if (tagB == 0 && tagA >= 2 && tagA < 50 )
	{
		if (!myplat->heartiseaten[tagA - 11])
		{
			char send[1024];
			sprintf(send, "1 %d", tagA - 11);
			myplat->sendMessage("EG", 3);
			myplat->sendMessage(send, strlen(send) + 1);
			character->add_HP(1);
		}
	}

	if (tagA == 0 && tagB >= 101 )
	{
		if (!myplat->dotiseaten[tagB - 101])
		{
			char send[1024];
			sprintf(send, "2 %d", tagB - 101);
			myplat->sendMessage(send, strlen(send) + 1);
			character->addXP(2* character->XPrate,nowlevelUI,level);
		}
		
	}
	else if (tagB == 0 && tagA >= 101)
	{
		if (!myplat->dotiseaten[tagA - 101])
		{
			char send[1024];
			sprintf(send, "2 %d", tagA - 101);
			myplat->sendMessage(send, strlen(send) + 1);
			character->addXP(2*character->XPrate, nowlevelUI, level);
		}
	}

	if (tagA >= 50 && tagB == 1 && tagA < 65)
	{
		if ((tagA - 49) % 4 != 0)
		{
			beAttacken[(tagA - 49) / 4][(tagA - 49) % 4 - 1] = true;
		}
		else
		{
			beAttacken[(tagA - 49) / 4 - 1][3] = true;
		}
		spriteA->stopAllActions();
	}
	else if (tagB >= 50 && tagA == 1 && tagB < 65)
	{
		if ((tagB - 49) % 4 != 0)
		{
			beAttacken[(tagB - 49) / 4][(tagB - 49) % 4 - 1] = true;
		}
		else
		{
			beAttacken[(tagB - 49) / 4 - 1][3] = true;
		}
		spriteB->stopAllActions();

	}
	/*if (tagA >= 65 && tagB == 0 && tagA < 80)
	{
		if ((tagA - 64) % 4 != 0)
		{
			beEnemyAttacken[(tagA - 64) / 4][(tagA - 64) % 4 - 1] = true;
		}
		else
		{
			beEnemyAttacken[(tagA - 64) / 4 - 1][3] = true;
		}
		spriteA->stopAllActions();
	}
	else if (tagB >= 65 && tagA == 0 && tagB < 80)
	{
		if ((tagB - 64) % 4 != 0)
		{
			beEnemyAttacken[(tagB - 64) / 4][(tagB - 64) % 4 - 1] = true;
		}
		else
		{
			beEnemyAttacken[(tagB - 64) / 4 - 1][3] = true;
		}
		spriteB->stopAllActions();

	}*/
	return true;
}
void OrdinaryScene::updateBoard()
{
	char a[5];
	sprintf(a, "%d", character->killNum);
	My_Num->setString(a);
	char b[5];
	sprintf(b, "%d", myplat->enemy->killNum);
	Enemy_Num->setString(b);

	if (character->killNum >= myplat->enemy->killNum)
	{
		My_Name->setPosition(Vec2(1000, 660));
		My_Num->setPosition(Vec2(1200, 660));
		Enemy_Name->setPosition(Vec2(1000, 610));
		Enemy_Num->setPosition(Vec2(1200, 610));
	}
	else
	{
		Enemy_Name->setPosition(Vec2(1000, 660));
		Enemy_Num->setPosition(Vec2(1200, 660));
		My_Name->setPosition(Vec2(1000, 610));
		My_Num->setPosition(Vec2(1200, 610));
	}
}
void OrdinaryScene::update(float delta) //每帧进行的更新操作
{
	if (!isover)
	{
		Node::update(delta);
		if (myplat->change == 1)
		{
			myplat->enemy->setTexture("model/character/character-crossbow.png");
			myplat->change = 0;
		}
		else if (myplat->change == 2)
		{
			myplat->enemy->setTexture("model/character/character-gun.png");
			myplat->change = 0;
		}
		else if (myplat->change == 3)
		{
			myplat->enemy->setTexture("model/character/character-fireball.png");
			myplat->change = 0;
		}
		
		if (!initServer&&myplat->totalNum >= 1)
		{
			myplat->enemy = Character::create("model/character/character-crossbow.png");
			myplat->enemybuilt = true;
			if (myplat->ID == 1)MapLayer->setPosition(Vec2(380, -5300));
			else if (myplat->ID == 2)MapLayer->setPosition(Vec2(-5050, 100));
			myplat->enemy->setAnchorPoint(Vec2(0.5, 0.2));
			myplat->enemy->setPosition(Vec2(3000, 3000));
			MapLayer->addChild(myplat->enemy, 2);
			PhysicsBody* enemybody = PhysicsBody::createBox(Size(80, 100));
			enemybody->setDynamic(false);
			myplat->enemy->setPhysicsBody(enemybody);
			myplat->enemy->setTag(1);
			enemybody->setCategoryBitmask(arrowMask);
			enemybody->setCollisionBitmask(heartMask | characterMask | dotMask | arrowMask | enemyMask);
			enemybody->setContactTestBitmask(heartMask | characterMask | dotMask | arrowMask | enemyMask); if (myplat->ID == 1)MapLayer->setPosition(Vec2(380, -5300));
			else if (myplat->ID == 2)MapLayer->setPosition(Vec2(-5050, 100));
			myplat->enemy->maxHpUI->setPosition(Vec2(myplat->enemy->getPositionX(), -60 + myplat->enemy->getPositionY()));
			MapLayer->addChild(myplat->enemy->maxHpUI);

			myplat->enemy->nowHpUI->setPosition(myplat->enemy->maxHpUI->getPosition() - Vec2(myplat->enemy->get_HP() * 15, 0));
			MapLayer->addChild(myplat->enemy->nowHpUI);

			
			initServer = true;
		}
		if (initServer)
		{
			if (myplat->enemy->getAlreadyDead() && myplat->addkill)
			{
				character->killNum++;
				updateBoard();
				myplat->addkill = false;
			}
			if (!myplat->enemy->getAlreadyDead())
			{
				myplat->addkill = true;
			}
		}
		if (initServer&&strlen(character->_playName.c_str()) == 0)
		{
			character->_playName = _playName;
			char a[20];
			sprintf(a, "CN %s", character->_playName.c_str());
			myplat->sendMessage(a, strlen(a) + 1);
		}
		if (!initBoard)
		{
			log("110 %s %d ", myplat->playname , strlen(myplat->playname));
			//log("%s\n%s", character->_playName.c_str(), myplat->playername.c_str());
			if ((strlen(character->_playName.c_str()) != 0) && (strlen(myplat->playname) != 0))
			{
				TTFConfig ttf2("fonts/Montserrat-Medium.ttf", 20, GlyphCollection::DYNAMIC);
				My_Name = Label::createWithTTF(ttf2, character->_playName.c_str());
				char a[5];
				sprintf(a, "%d", character->killNum);
				My_Num= Label::createWithTTF(ttf2, a);
				My_Name->setAnchorPoint(Vec2(0, 0.5));
				My_Name->setPosition(Vec2(1000, 660));
				MenuLayer->addChild(My_Name);
				My_Num->setAnchorPoint(Vec2(0, 0.5));
				My_Num->setPosition(Vec2(1200, 660));
				MenuLayer->addChild(My_Num);


				Enemy_Name = Label::createWithTTF(ttf2, myplat->playname);
				char b[5];
				sprintf(b, "%d", myplat->enemy->killNum);
				Enemy_Num = Label::createWithTTF(ttf2, b);
				Enemy_Name->setAnchorPoint(Vec2(0, 0.5));
				Enemy_Name->setPosition(Vec2(1000, 610));
				MenuLayer->addChild(Enemy_Name);
				Enemy_Num->setAnchorPoint(Vec2(0, 0.5));
				Enemy_Num->setPosition(Vec2(1200, 610));
				MenuLayer->addChild(Enemy_Num);

				initBoard = true;
			}
		}

		if (knum!=myplat->enemy->killNum)
		{
			knum = myplat->enemy->killNum;
			updateBoard();
		}




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
		if (!isKeyPressed(upArrow) && isKeyPressed(downArrow) && isKeyPressed(leftArrow) && isKeyPressed(rightArrow))character->stopAllActions();





		if (begintime == 0)begintime = GetCurrentTime();
		int nowTime = GetCurrentTime();
		if (isStart)
		{
			int time = nowTime - begintime;

			if (totalTime > time)
			{
				if (totalTime - time < 30000)timeminder->setColor(Color3B(255, 0, 0));
				int showtime = (totalTime - time) / 1000;
				char a[20];
				sprintf(a, "  Time:\n%02d : %02d", showtime / 60, showtime % 60);
				timeminder->setString(a);
			}
			if (time >= totalTime)
			{
				if (character->killNum >= myplat->enemy->killNum)
				{
					auto result = Sprite::create("scene/gameUI/win.png");
					result->setPosition(Vec2(640, 384));
					MenuLayer->addChild(result);
					result->setScale(0.2f);
					result->runAction(ScaleTo::create(1, 1.2));
					character->movable = false;
					isover = true;
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/win.mp3");
				}
				else
				{
					auto result = Sprite::create("scene/gameUI/lose.png");
					result->setPosition(Vec2(640, 384));
					MenuLayer->addChild(result);
					result->setScale(0.2f);
					result->runAction(ScaleTo::create(1, 1.2));
					character->movable = false;
					isover = true;
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lose.wav");
				}

			}
		}

		if (character->getAlreadyDead())
		{
			character->movable = false;
			if (character->deadbegintime == 0)
			{
				character->deadbegintime = GetCurrentTime();
				myplat->sendMessage("ED", 3);
			}
			if (nowTime - character->deadbegintime >= 1000 && countnumber == 10)
			{
				number = Sprite::create("scene/gameUI/9.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 11;
			}
			else if (nowTime - character->deadbegintime >= 2000 && countnumber == 11)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/8.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 12;
			}
			else if (nowTime - character->deadbegintime >= 3000 && countnumber == 12)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/7.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 13;
			}
			else if (nowTime - character->deadbegintime >= 4000 && countnumber == 13)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/6.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 14;
			}
			else if (nowTime - character->deadbegintime >= 5000 && countnumber == 14)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/5.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 15;
			}
			else if (nowTime - character->deadbegintime >= 6000 && countnumber == 15)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/4.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 16;
			}
			else if (nowTime - character->deadbegintime >= 7000 && countnumber == 16)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/3.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 17;
			}
			else if (nowTime - character->deadbegintime >= 8000 && countnumber == 17)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/2.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 18;
			}
			else if (nowTime - character->deadbegintime >= 9000 && countnumber == 18)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/1.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 19;
			}
			else if (nowTime - character->deadbegintime >= 10000 && countnumber == 19)
			{
				MenuLayer->removeChild(number);
				number = Sprite::create("scene/gameUI/0.png");
				number->setPosition(Vec2(640, 384));
				MenuLayer->addChild(number);
				number->setScale(3.0f);
				number->runAction(ScaleTo::create(0.5, 1));
				countnumber = 20;
			}
			else if (nowTime - character->deadbegintime >= 11000 && countnumber == 20)
			{
				MenuLayer->removeChild(number);
				character->setColor(Color3B(255, 255, 255));
				character->setcomeback(nowlevelUI, level);
				character->setTexture("model/character/character-crossbow.png");
				character->movable = true;
				if (myplat->ID == 1)MapLayer->setPosition(Vec2(380, -5300));
				else if (myplat->ID == 2)MapLayer->setPosition(Vec2(-5050, 100));
				myplat->sendMessage("EC", 3);
				countnumber = 10;
			}
		}
		if (nowTime - begintime >= 1000 && countnumber == 1)
		{
			number = Sprite::create("scene/gameUI/3.png");
			number->setPosition(Vec2(640, 384));
			MenuLayer->addChild(number);
			number->setScale(3.0f);
			number->runAction(ScaleTo::create(0.5, 1));
			countnumber = 2;
		}
		else if (nowTime - begintime >= 2000 && countnumber == 2)
		{
			MenuLayer->removeChild(number);
			number = Sprite::create("scene/gameUI/2.png");
			number->setPosition(Vec2(640, 384));
			MenuLayer->addChild(number);
			number->setScale(3.0f);
			number->runAction(ScaleTo::create(0.5, 1));
			countnumber = 3;
		}
		else if (nowTime - begintime >= 3000 && countnumber == 3)
		{
			MenuLayer->removeChild(number);
			number = Sprite::create("scene/gameUI/1.png");
			number->setPosition(Vec2(640, 384));
			MenuLayer->addChild(number);
			number->setScale(3.0f);
			number->runAction(ScaleTo::create(0.5, 1));
			countnumber = 4;
		}
		else if (nowTime - begintime >= 4000 && countnumber == 4)
		{
			MenuLayer->removeChild(number);
			number = Sprite::create("scene/gameUI/0.png");
			number->setPosition(Vec2(640, 384));
			MenuLayer->addChild(number);
			number->setScale(3.0f);
			number->runAction(ScaleTo::create(0.5, 1));
			countnumber = 5;
		}
		else if (nowTime - begintime >= 5000 && countnumber == 5)
		{
			MenuLayer->removeChild(number);
			character->movable = true;
			begintime = GetCurrentTime();
			isStart = true;
			countnumber = 10;
		}


		/*circle->setScale(1 - ((float)nowTime - (float)begintime) / 60000 * 0.4);
		if (3000-640 + MapLayer->getPositionX() > 2960 * circle->getScaleX()
		|| 3000-384 + MapLayer->getPositionY() > 2960 * circle->getScaleY())
		{
		if(circletime==0)circletime= GetCurrentTime();
		else if (nowTime - circletime >= 2000)
		{
		character->setDamage(0.1);
		circletime = 0;
		circleblinktime = 1;
		circlewaittime = GetCurrentTime();
		character->runAction(Blink::create(0.1, 1));
		character->runAction(TintTo::create(0.05, 0, 255, 255));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/circle.wav");
		}
		if (circleblinktime == 1)
		{
		if (nowTime - circlewaittime >= 300)xp
		{
		character->runAction(TintTo::create(0.1, 255, 255, 255));
		circleblinktime = 0;
		}
		}

		}*/

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (beAttacken[i][j])
				{
					myplat->sendMessage("EA", 3);
					if (myplat->enemy->setDamage(character->get_ATT() + character->vapoon->ATT - myplat->enemy->getDEF()))
					{
						character->addXP(50 * character->XPrate, nowlevelUI, level);
						
						char a[1024];
						sprintf(a, "EK %d", character->killNum);
						myplat->enemy->setColor(Color3B(100, 100, 100));
						myplat->sendMessage(a, strlen(a) + 1);
						updateBoard();
					}
					if (!myplat->enemy->getAlreadyDead() && blinktime == 0)
					{
						myplat->enemy->runAction(Blink::create(0.1, 1));
						myplat->enemy->runAction(TintTo::create(0.05, 255, 0, 0));
						blinktime = 1;
						coloroldtime = GetCurrentTime();

					}
					beAttacken[i][j] = false;
				}
				if (blinktime == 1)
				{
					if (nowTime - coloroldtime >= 200)
					{
						myplat->enemy->runAction(TintTo::create(0.1, 255, 255, 255));
						blinktime = 0;
					}
				}

			}
		}
		if (nowTime - begintime >= 3000)
		{

			if (deltatime == 0)deltatime = nowTime;
			if (nowTime - deltatime >= delta)
			{
				char sendhealth[1024];
				//log("%f %f", character->getPositionX() - MapLayer->getPositionX(), character->getPositionY() - MapLayer->getPositionY());
				sprintf(sendhealth, "CP %f,%f,%f,%d,%d", character->get_HP(), character->getPositionX() - MapLayer->getPositionX(), character->getPositionY() - MapLayer->getPositionY(),
					character->get_level(),character->killNum);
				myplat->sendMessage(sendhealth, strlen(sendhealth) + 1);
				myplat->enemy->maxHpUI->setPosition(Vec2(myplat->enemy->getPositionX(), -60 + myplat->enemy->getPositionY()));
				myplat->enemy->nowHpUI->setPosition(myplat->enemy->maxHpUI->getPosition() - Vec2(myplat->enemy->getMaxHP() * 15, 0));
				myplat->enemy->setPosition(myplat->EnemyPos);
				deltatime = 0;
				myplat->enemy->runAction(RotateTo::create(0, myplat->EnemyDir));
			}
			if (myplat->beAttackenTime > 0)
			{
				if (character->setDamage(myplat->enemy->get_ATT() + myplat->enemy->vapoon->ATT - character->getDEF()))character->setColor(Color3B(100, 100, 100));
				else if (Enemyblinktime == 0)
				{
					character->runAction(Blink::create(0.1, 1));
					character->runAction(TintTo::create(0.05, 255, 0, 0));
					Enemyblinktime = 1;
					Enemycoloroldtime = GetCurrentTime();
				}
				myplat->beAttackenTime--;
			}
			if (Enemyblinktime == 1)
			{
				if (nowTime - Enemycoloroldtime >= 200)
				{
					character->runAction(TintTo::create(0.1, 255, 255, 255));
					Enemyblinktime = 0;
				}
			}

			if (myplat->enemyShootTime > 0 && !isEnemyAttacking[0][0])             //改
			{
				for (int i = 0; i < 3; i++)
				{
					int k;
					if (i == 0)k = myplat->enemyfrontnum;
					else if (i == 1)k = myplat->enemybesidenum * 2;
					else k = myplat->enemybacknum;
					for (int j = 0; j < k; j++)
					{
						if (myplat->enemy->vapoonID == crossbow)enemyarrow[i][j] = Sprite::create("model/vapoon/bullet/arrow.png");
						else if (myplat->enemy->vapoonID == gun)enemyarrow[i][j] = Sprite::create("model/vapoon/bullet/bullet.png");
						else if (myplat->enemy->vapoonID == fireball)enemyarrow[i][j] = Sprite::create("model/vapoon/bullet/fire.png");
						if (myplat->enemy->isscale)enemyarrow[i][j]->setScale(1.5f);
						enemyarrowbody[i][j] = PhysicsBody::createBox(enemyarrow[0][0]->getContentSize());
						enemyarrowbody[i][j]->setDynamic(false);            //
						enemyarrow[i][j]->setPhysicsBody(enemyarrowbody[i][j]); //
						enemyarrow[i][j]->setTag(65 + 3 * i + j);                     //
						isEnemyAttacking[i][j] = true;
						enemyarrowbody[i][j]->setCategoryBitmask(arrowMask);
						enemyarrowbody[i][j]->setCollisionBitmask(heartMask | characterMask | dotMask | arrowMask);
						enemyarrowbody[i][j]->setContactTestBitmask(heartMask | characterMask | dotMask | arrowMask);
						MapLayer->addChild(enemyarrow[i][j], 2);
					}
				}
				if (myplat->enemyfrontnum == 1)
				{
					enemyarrow[0][0]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150);
					myplat->FrontDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[0][0]->runAction(RotateTo::create(0, myplat->rotateDegrees));
					enemyarrow[0][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontDirection));                                     //


					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						for (int s = 0; s < 3; s++)
						{
							for (int t = 0; t < 4; t++)isEnemyAttacking[s][t] = false;
						}
						MapLayer->removeChild(enemyarrow[0][0], true);

					}), NULL));
				}
				else if (myplat->enemyfrontnum == 2)
				{
					enemyarrow[0][0]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 + myplat->BesideDirection * 15);
					enemyarrow[0][1]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 - myplat->BesideDirection * 15);
					myplat->FrontDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[0][0]->runAction(RotateTo::create(0, myplat->rotateDegrees));
					enemyarrow[0][1]->runAction(RotateTo::create(0, myplat->rotateDegrees));
					enemyarrow[0][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontDirection));
					enemyarrow[0][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontDirection)); //


					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						for (int s = 0; s < 3; s++)
						{
							for (int t = 0; t < 4; t++)isEnemyAttacking[s][t] = false;
						}
						MapLayer->removeChild(enemyarrow[0][0], true);
						MapLayer->removeChild(enemyarrow[0][1], true);
					}), NULL));
				}
				else if (myplat->enemyfrontnum == 3)
				{
					enemyarrow[0][0]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150);
					enemyarrow[0][1]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 + myplat->BesideDirection * 10);
					enemyarrow[0][2]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 - myplat->BesideDirection * 10);
					myplat->FrontDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->FrontLeftDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->FrontRightDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[0][0]->runAction(RotateTo::create(0, myplat->rotateDegrees));
					enemyarrow[0][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 15));
					enemyarrow[0][2]->runAction(RotateTo::create(0, myplat->rotateDegrees - 15));
					enemyarrow[0][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontDirection));
					enemyarrow[0][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontLeftDirection));
					enemyarrow[0][2]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontRightDirection));

					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						for (int s = 0; s < 3; s++)
						{
							for (int t = 0; t < 4; t++)isEnemyAttacking[s][t] = false;
						}
						MapLayer->removeChild(enemyarrow[0][0], true);
						MapLayer->removeChild(enemyarrow[0][1], true);
						MapLayer->removeChild(enemyarrow[0][2], true);
					}), NULL));

				}
				else if (myplat->enemyfrontnum == 4)
				{
					enemyarrow[0][0]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 + myplat->BesideDirection * 30);
					enemyarrow[0][1]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 + myplat->BesideDirection * 10);
					enemyarrow[0][2]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 - myplat->BesideDirection * 10);
					enemyarrow[0][3]->setPosition(myplat->EnemyPos + myplat->FrontDirection * 150 - myplat->BesideDirection * 30);
					myplat->FrontDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->FrontLeftDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->FrontRightDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[0][0]->runAction(RotateTo::create(0, myplat->rotateDegrees));
					enemyarrow[0][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 15));
					enemyarrow[0][2]->runAction(RotateTo::create(0, myplat->rotateDegrees - 15));
					enemyarrow[0][3]->runAction(RotateTo::create(0, myplat->rotateDegrees));
					enemyarrow[0][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontDirection));
					enemyarrow[0][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontLeftDirection));
					enemyarrow[0][2]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontRightDirection));
					enemyarrow[0][3]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->FrontDirection));

					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						for (int s = 0; s < 3; s++)
						{
							for (int t = 0; t < 4; t++)isEnemyAttacking[s][t] = false;
						}
						MapLayer->removeChild(enemyarrow[0][0], true);
						MapLayer->removeChild(enemyarrow[0][1], true);
						MapLayer->removeChild(enemyarrow[0][2], true);
						MapLayer->removeChild(enemyarrow[0][3], true);
					}), NULL));
				}

				if (myplat->enemybacknum == 1)
				{
					enemyarrow[2][0]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50);
					myplat->BackDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[2][0]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180));
					enemyarrow[2][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackDirection));                                     //

					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						MapLayer->removeChild(enemyarrow[2][0], true);

					}), NULL));
				}
				else if (myplat->enemybacknum == 2)
				{
					enemyarrow[2][0]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 + myplat->BesideDirection * 15);
					enemyarrow[2][1]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 - myplat->BesideDirection * 15);
					myplat->BackDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[2][0]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180));
					enemyarrow[2][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180));
					enemyarrow[2][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackDirection));
					enemyarrow[2][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackDirection));

					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						MapLayer->removeChild(enemyarrow[2][0], true);
						MapLayer->removeChild(enemyarrow[2][1], true);
					}), NULL));
				}
				else if (myplat->enemybacknum == 3)
				{
					enemyarrow[2][0]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50);
					enemyarrow[2][1]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 - myplat->BesideDirection * 10);
					enemyarrow[2][2]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 + myplat->BesideDirection * 10);
					myplat->BackDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->BackLeftDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->BackRightDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[2][0]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180));
					enemyarrow[2][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180 + 15));
					enemyarrow[2][2]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180 - 15));
					enemyarrow[2][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackDirection));
					enemyarrow[2][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackLeftDirection));
					enemyarrow[2][2]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackRightDirection));

					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						MapLayer->removeChild(enemyarrow[2][0], true);
						MapLayer->removeChild(enemyarrow[2][1], true);
						MapLayer->removeChild(enemyarrow[2][2], true);
					}), NULL));

				}
				else if (myplat->enemybacknum == 4)
				{
					enemyarrow[2][0]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 + myplat->BesideDirection * 30);
					enemyarrow[2][1]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 - myplat->BesideDirection * 10);
					enemyarrow[2][2]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 + myplat->BesideDirection * 10);
					enemyarrow[2][3]->setPosition(myplat->EnemyPos + myplat->BackDirection * 50 - myplat->BesideDirection * 30);
					myplat->BackDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->BackLeftDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->BackRightDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[2][0]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180));
					enemyarrow[2][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180 + 15));
					enemyarrow[2][2]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180 - 15));
					enemyarrow[2][3]->runAction(RotateTo::create(0, myplat->rotateDegrees + 180));
					enemyarrow[2][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackDirection));
					enemyarrow[2][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackLeftDirection));
					enemyarrow[2][2]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackRightDirection));
					enemyarrow[2][3]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BackDirection));

					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //

						MapLayer->removeChild(enemyarrow[2][0], true);
						MapLayer->removeChild(enemyarrow[2][1], true);
						MapLayer->removeChild(enemyarrow[2][2], true);
						MapLayer->removeChild(enemyarrow[2][3], true);
					}), NULL));
				}

				if (myplat->enemybesidenum == 1)
				{
					enemyarrow[1][0]->setPosition(myplat->EnemyPos - myplat->BesideDirection * 80);
					enemyarrow[1][1]->setPosition(myplat->EnemyPos + myplat->BesideDirection * 80);
					myplat->BesideDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[1][0]->runAction(RotateTo::create(0, myplat->rotateDegrees - 90));
					enemyarrow[1][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 90));
					enemyarrow[1][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, -myplat->BesideDirection));                                     //
					enemyarrow[1][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BesideDirection));


					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						MapLayer->removeChild(enemyarrow[1][0], true);
						MapLayer->removeChild(enemyarrow[1][1], true);

					}), NULL));
				}
				else if (myplat->enemybesidenum == 2)
				{
					enemyarrow[1][0]->setPosition(myplat->EnemyPos - myplat->BesideDirection * 80 + 20 * myplat->FrontDirection / myplat->FrontDirection.getLength());
					enemyarrow[1][1]->setPosition(myplat->EnemyPos + myplat->BesideDirection * 80 + 20 * myplat->FrontDirection / myplat->FrontDirection.getLength());
					enemyarrow[1][2]->setPosition(myplat->EnemyPos - myplat->BesideDirection * 80 + 8 * myplat->FrontDirection / myplat->FrontDirection.getLength());
					enemyarrow[1][3]->setPosition(myplat->EnemyPos + myplat->BesideDirection * 80 + 8 * myplat->FrontDirection / myplat->FrontDirection.getLength());
					myplat->BesideDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->BesideFrontDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					myplat->BesideBackDirection *= myplat->enemy->getATT_Range() + myplat->enemy->vapoon->Range;
					enemyarrow[1][0]->runAction(RotateTo::create(0, myplat->rotateDegrees - 90 + 15));
					enemyarrow[1][1]->runAction(RotateTo::create(0, myplat->rotateDegrees + 90 - 15));
					enemyarrow[1][2]->runAction(RotateTo::create(0, myplat->rotateDegrees - 90 - 15));
					enemyarrow[1][3]->runAction(RotateTo::create(0, myplat->rotateDegrees + 90 + 15));
					enemyarrow[1][0]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, -myplat->BesideFrontDirection));                                     //
					enemyarrow[1][1]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BesideBackDirection));
					enemyarrow[1][2]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, -myplat->BesideBackDirection));                                     //
					enemyarrow[1][3]->runAction(MoveBy::create(myplat->enemy->vapoon->shootSPD, myplat->BesideFrontDirection));


					this->runAction(Sequence::create(DelayTime::create(myplat->enemy->vapoon->waitTime), CallFunc::create([&]() {     //
						MapLayer->removeChild(enemyarrow[1][0], true);
						MapLayer->removeChild(enemyarrow[1][1], true);
						MapLayer->removeChild(enemyarrow[1][2], true);
						MapLayer->removeChild(enemyarrow[1][3], true);

					}), NULL));
				}


				if (myplat->enemy->vapoonID == crossbow)CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/Vapoon/shoot crossbow.wav");             //
				else if (myplat->enemy->vapoonID == gun)CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/Vapoon/shoot.wav");
				else if (myplat->enemy->vapoonID == fireball)CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/Vapoon/fire.wav");
				myplat->enemyShootTime--;
			}
		}


		for (int i = 0; i < 36; i++)
		{
			if (myplat->heartiseaten[i] && !heartisloading[i])
			{
				heart[i]->runAction(FadeOut::create(0.5));
				this->heartoldTime[i] = GetCurrentTime();
				heartisloading[i] = true;
			}
			if (heartisloading[i])
			{
				if (nowTime - heartoldTime[i] >= 30000)
				{
					heart[i]->runAction(FadeIn::create(1));
					heartisloading[i] = false;
					myplat->heartiseaten[i] = false;
				}
			}
		}


		for (int i = 0; i < 500; i++)
		{
			if (myplat->dotiseaten[i] && !dotisloading[i])
			{
				dot[i]->runAction(FadeOut::create(0.5));
				this->dotoldTime[i] = GetCurrentTime();
				dotisloading[i] = true;
				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
			}
			if (dotisloading[i])
			{
				if (nowTime - dotoldTime[i] >= 60000)
				{
					dot[i]->runAction(FadeIn::create(1));
					dotisloading[i] = false;
					myplat->dotiseaten[i] = false;
				}
			}
		}

		if (!character->isVapoonCross)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (isAttacking[i][j])
					{
						Vec2 tileCoord = Vec2(arrow[i][j]->getPosition().x / 60, 100 - arrow[i][j]->getPosition().y / 60);
						if ((character->vapoon->frontnum == 1 && i == 0) || (character->vapoon->frontnum == 2 && i == 0) ||
							(character->vapoon->frontnum == 3 && i == 0 && j == 0) || (character->vapoon->frontnum == 4 && i == 0 && (j == 0 || j == 3)))
						{
							if (FrontDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (FrontDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (FrontDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (FrontDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if ((character->vapoon->frontnum == 3 && i == 0 && j == 1) || (character->vapoon->frontnum == 4 && i == 0 && j == 1))
						{
							if (FrontLeftDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (FrontLeftDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (FrontLeftDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (FrontLeftDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if ((character->vapoon->frontnum == 3 && i == 0 && j == 2) || (character->vapoon->frontnum == 4 && i == 0 && j == 2))
						{
							if (FrontRightDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (FrontRightDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (FrontRightDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (FrontRightDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if ((character->vapoon->backnum == 1 && i == 2) || (character->vapoon->backnum == 2 && i == 2) ||
							(character->vapoon->backnum == 3 && i == 2 && j == 0) || (character->vapoon->backnum == 4 && i == 2 && (j == 0 || j == 3)))
						{
							if (BackDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (BackDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (BackDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (BackDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if ((character->vapoon->backnum == 3 && i == 2 && j == 1) || (character->vapoon->backnum == 4 && i == 2 && j == 1))
						{
							if (BackLeftDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (BackLeftDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (BackLeftDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (BackLeftDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if ((character->vapoon->backnum == 3 && i == 2 && j == 2) || (character->vapoon->backnum == 4 && i == 2 && j == 2))
						{
							if (BackRightDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (BackRightDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (BackRightDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (BackRightDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if (character->vapoon->besidenum == 1 && i == 1 && j == 0)
						{
							if (BesideDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (BesideDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (BesideDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (BesideDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if (character->vapoon->besidenum == 1 && i == 1 && j == 1)
						{
							if (!BesideDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (!BesideDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (!BesideDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (!BesideDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if (character->vapoon->besidenum == 2 && i == 1 && j == 0)
						{
							if (-BesideFrontDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (-BesideFrontDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (-BesideFrontDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (-BesideFrontDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if (character->vapoon->besidenum == 2 && i == 1 && j == 1)
						{
							if (BesideBackDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (BesideBackDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (BesideBackDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (BesideBackDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if (character->vapoon->besidenum == 2 && i == 1 && j == 2)
						{
							if (-BesideBackDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (-BesideBackDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (-BesideBackDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (-BesideBackDirection.y < 0)tileCoord += Vec2(0, 1);
						}
						else if (character->vapoon->besidenum == 2 && i == 1 && j == 3)
						{
							if (BesideFrontDirection.x > 0)tileCoord += Vec2(1, 0);
							else if (BesideFrontDirection.x < 0)tileCoord += Vec2(-1, 0);
							if (BesideFrontDirection.y > 0)tileCoord -= Vec2(0, 1);
							else if (BesideFrontDirection.y < 0)tileCoord += Vec2(0, 1);
						}

						int tileGid = wall->getTileGIDAt(tileCoord);
						if (tileGid > 0)
						{
							arrow[i][j]->stopAllActions();
						}
					}
				}
			}
		}
		if (character->select_time > 0 && character->canSelect == 0)
		{
			character->setSkillUI();
			character->select_time--;
		}
		if (character->canSelect == 1)
		{
			for (int j = 0; j < 3; j++)character->SkillUI[j]->setVisible(true);
			character->canSelect = 2;
		}
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
		offsetX = character->get_SPD();
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX = -character->get_SPD();
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY = -character->get_SPD();
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY = character->get_SPD();
		break;
	default:
		offsetY = offsetX = 0;
		break;
	}
	if(!character->movable)offsetY = offsetX = 0;
	if (!character->isCharacterCross)
	{
		Vec2 characterPosition = character->getPosition() - MapLayer->getPosition();
		Vec2 tileCoord = Vec2((int)characterPosition.x / 60, (int)100 - characterPosition.y / 60);
		if (offsetX > 0)tileCoord -= Vec2(1, 0);
		else if (offsetX < 0)tileCoord += Vec2(1, 0);
		if (offsetY > 0)tileCoord += Vec2(0, 1);
		else if (offsetY < 0)tileCoord -= Vec2(0, 1);
		int tileGid = wall->getTileGIDAt(tileCoord);
		if (tileGid > 0)
		{
			MapLayer->stopAllActions();
			offsetY = offsetX = 0;
		}
	}
	auto moveTo = MoveTo::create(0.1, Vec2(MapLayer->getPositionX() + offsetX, MapLayer->getPositionY() + offsetY));
	MapLayer->runAction(moveTo);
}

void OrdinaryScene::initHeart()
{
	for (int i = 0; i < 36; i++)
	{
		heart[i] = Sprite::create("scene/Map/heart.png");
		if (i < 5) heart[i]->setPosition(Vec2(20 * 60 + i * 12 * 60 + 30, 30 + 11 * 60));
		else if (i < 8)heart[i]->setPosition(Vec2(9 * 60 + (i - 5) * 42 * 60 + 30, 30 + 20 * 60));
		else if (i < 11)heart[i]->setPosition(Vec2(9 * 60 + (i - 8) * 42 * 60 + 30, 30 + 33 * 60));
		else if (i < 13)heart[i]->setPosition(Vec2(26 * 60 + (i - 11) * 51 * 60 + 30, 30 + 66 * 60));
		else if (i < 15)heart[i]->setPosition(Vec2(26 * 60 + (i - 13) * 51 * 60 + 30, 30 + 66 * 60));
		else if (i < 20) heart[i]->setPosition(Vec2(20 * 60 + (i - 15) * 12 * 60 + 30, 30 + 88 * 60));
		else if (i < 23)heart[i]->setPosition(Vec2(9 * 60 + (i - 20) * 42 * 60 + 30, 30 + 60 * 80));
		else if (i < 26)heart[i]->setPosition(Vec2(9 * 60 + (i - 23) * 42 * 60 + 30, 30 + 60 * 70));
		else if (i < 34)heart[i]->setPosition(Vec2(8 * 60 + (i - 26) * 12 * 60 + 30, 30 + 60 * 50));
		else if (i < 36)heart[i]->setPosition(Vec2(50 * 60 + 30, 30 + 60 * 41 + (i - 34) * 18 * 60));

		MapLayer->addChild(heart[i], 1);
		heartbody[i] = PhysicsBody::createCircle(20,
			PHYSICSBODY_MATERIAL_DEFAULT);
		heartbody[i]->setDynamic(false);
		heart[i]->setPhysicsBody(heartbody[i]);
		heart[i]->setTag(11 + i);
		heartbody[i]->setCategoryBitmask(heartMask);
		heartbody[i]->setCollisionBitmask(heartMask | characterMask | dotMask);
		heartbody[i]->setContactTestBitmask(heartMask | characterMask | dotMask);
	}
}

void  OrdinaryScene::exit(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/click.wav");
	auto b = GameScene::createScene();
	if(service!=nullptr)service->close();
	Director::getInstance()->replaceScene(b);
}

void OrdinaryScene::initStar()
{
	for (int i = 0; i < 500; i++)
	{
		dot[i] = Sprite::create("scene/Map/XP.png");
		if (i < 25)
		{
			if ((i + 1) % 5)dot[i]->setPosition(Vec2(7 * 60 + 30 + ((i + 1) % 5 - 1)*2 * 60, 7 * 60 + 30 + ((i + 1) / 5)*2 * 60));
			else if ((i + 1) % 5 == 0)dot[i]->setPosition(Vec2(15 * 60 + 30, 7 * 60 + 30 + ((i + 1) / 5 - 1)*2 * 60));
		}
		else if (i < 50)
		{
			if ((i - 24) % 5)dot[i]->setPosition(Vec2(40 * 60 + 30 + ((i - 24) % 5 - 1) *2* 60, 17 * 60 + 30 + ((i - 24) / 5)*2 * 60));
			else if ((i - 24) % 5 == 0)dot[i]->setPosition(Vec2(48 * 60 + 30, 17 * 60 + 30 + ((i - 24) / 5 - 1) * 60));
		}
		else if (i < 75)
		{
			if ((i - 24 - 25) % 5)dot[i]->setPosition(Vec2(58 * 60 + 30 + ((i - 24 - 25) % 5 - 1)*2 * 60, 17 * 60 + 30 + ((i - 24 - 25) / 5) *2 *60));
			else if ((i - 24 - 25) % 5 == 0)dot[i]->setPosition(Vec2(66 * 60 + 30, 17 * 60 + 30 + ((i - 24 - 25) / 5 - 1)*2 * 60));
		}
		else if (i < 100)
		{
			if ((i - 24 - 25 * 2) % 5)dot[i]->setPosition(Vec2(88 * 60 + 30 + ((i - 24 - 25 * 2) % 5 - 1) *2* 60, 7 * 60 + 30 + ((i - 24 - 25 * 2) / 5) *2* 60));
			else if ((i - 24 - 25 * 2) % 5 == 0)dot[i]->setPosition(Vec2(96 * 60 + 30, 7 * 60 + 30 + ((i - 24 - 25 * 2) / 5 - 1) *2* 60));
		}
		else if (i < 125)
		{
			if ((i - 24 - 25 * 3) % 5)dot[i]->setPosition(Vec2(18 * 60 + 30 + ((i - 24 - 25 * 3) % 5 - 1)*2 * 60, 18 * 60 + 30 + ((i - 24 - 25 * 3) / 5)*2 * 60));
			else if ((i - 24 - 25 * 3) % 5 == 0)dot[i]->setPosition(Vec2(26 * 60 + 30, 18 * 60 + 30 + ((i - 24 - 25 * 3) / 5 - 1)*2 * 60));
		}
		else if (i < 150)
		{
			if ((i - 24 - 25 * 4) % 5)dot[i]->setPosition(Vec2(80 * 60 + 30 + ((i - 24 - 25 * 4) % 5 - 1)*2 * 60, 18 * 60 + 30 + ((i - 24 - 25 * 4) / 5)*2 * 60));
			else if ((i - 24 - 25 * 4) % 5 == 0)dot[i]->setPosition(Vec2(88 * 60 + 30, 18 * 60 + 30 + ((i - 24 - 25 * 4) / 5 - 1)*2 * 60));
		}
		else if (i < 175)
		{
			if ((i - 24 - 25 * 5) % 5)dot[i]->setPosition(Vec2(12 * 60 + 30 + ((i - 24 - 25 * 5) % 5 - 1)*2 * 60, 41 * 60 + 30 + ((i - 24 - 25 * 5) / 5)*2 * 60));
			else if ((i - 24 - 25 * 5) % 5 == 0)dot[i]->setPosition(Vec2(20 * 60 + 30, 41 * 60 + 30 + ((i - 24 - 25 * 5) / 5 - 1)*2 * 60));
		}
		else if (i < 200)
		{
			if ((i - 24 - 25 * 6) % 5)dot[i]->setPosition(Vec2(40 * 60 + 30 + ((i - 24 - 25 * 6) % 5 - 1)*2 * 60, 41 * 60 + 30 + ((i - 24 - 25 * 6) / 5)*2 * 60));
			else if ((i - 24 - 25 * 6) % 5 == 0)dot[i]->setPosition(Vec2(48 * 60 + 30, 41 * 60 + 30 + ((i - 24 - 25 * 6) / 5 - 1)*2 * 60));
		}
		else if (i < 225)
		{
			if ((i - 24 - 25 * 7) % 5)dot[i]->setPosition(Vec2(59 * 60 + 30 + ((i - 24 - 25 * 7) % 5 - 1)*2 * 60, 41 * 60 + 30 + ((i - 24 - 25 * 7) / 5)*2 * 60));
			else if ((i - 24 - 25 * 7) % 5 == 0)dot[i]->setPosition(Vec2(67 * 60 + 30, 41 * 60 + 30 + ((i - 24 - 25 * 7) / 5 - 1)*2 * 60));
		}
		else if (i < 250)
		{
			if ((i - 24 - 25 * 8) % 5)dot[i]->setPosition(Vec2(86 * 60 + 30 + ((i - 24 - 25 * 8) % 5 - 1)*2 * 60, 41 * 60 + 30 + ((i - 24 - 25 * 8) / 5)*2 * 60));
			else if ((i - 24 - 25 * 8) % 5 == 0)dot[i]->setPosition(Vec2(94 * 60 + 30, 41 * 60 + 30 + ((i - 24 - 25 * 8) / 5 - 1)*2 * 60));
		}
		else if (i < 275)
		{
			if ((i - 24 - 25 * 9) % 5)dot[i]->setPosition(Vec2(86 * 60 + 30 + ((i - 24 - 25 * 9) % 5 - 1)*2 * 60, 59 * 60 + 30 + ((i - 24 - 25 * 9) / 5)*2 * 60));
			else if ((i - 24 - 25 * 9) % 5 == 0)dot[i]->setPosition(Vec2(94 * 60 + 30, 59 * 60 + 30 + ((i - 24 - 25 * 9) / 5 - 1)*2 * 60));
		}
		else if (i < 300)
		{
			if ((i - 24 - 25 * 10) % 5)dot[i]->setPosition(Vec2(59 * 60 + 30 + ((i - 24 - 25 * 10) % 5 - 1)*2 * 60, 59 * 60 + 30 + ((i - 24 - 25 * 10) / 5)*2 * 60));
			else if ((i - 24 - 25 * 10) % 5 == 0)dot[i]->setPosition(Vec2(67 * 60 + 30, 59 * 60 + 30 + ((i - 24 - 25 * 10) / 5 - 1)*2 * 60));
		}
		else if (i < 325)
		{
			if ((i - 24 - 25 * 11) % 5)dot[i]->setPosition(Vec2(12 * 60 + 30 + ((i - 24 - 25 * 11) % 5 - 1)*2 * 60, 59 * 60 + 30 + ((i - 24 - 25 * 11) / 5)*2 * 60));
			else if ((i - 24 - 25 * 5) % 5 == 0)dot[i]->setPosition(Vec2(20 * 60 + 30, 59 * 60 + 30 + ((i - 24 - 25 * 11) / 5 - 1)*2 * 60));
		}
		else if (i < 350)
		{
			if ((i - 24 - 25 * 12) % 5)dot[i]->setPosition(Vec2(40 * 60 + 30 + ((i - 24 - 25 * 12) % 5 - 1)*2 * 60, 59 * 60 + 30 + ((i - 24 - 25 * 12) / 5)*2 * 60));
			else if ((i - 24 - 25 * 12) % 5 == 0)dot[i]->setPosition(Vec2(48 * 60 + 30, 59 * 60 + 30 + ((i - 24 - 25 * 12) / 5 - 1)*2 * 60));
		}
		else if (i < 375)
		{
			if ((i - 24 - 25 * 13) % 5)dot[i]->setPosition(Vec2(7 * 60 + 30 + ((i - 24 - 25 * 13) % 5 - 1)*2 * 60, 88 * 60 + 30 + ((i - 24 - 25 * 13) / 5)*2 * 60));
			else if ((i - 24 - 25 * 13) % 5 == 0)dot[i]->setPosition(Vec2(15 * 60 + 30, 88 * 60 + 30 + ((i - 24 - 25 * 13) / 5 - 1)*2 * 60));
		}
		else if (i < 400)
		{
			if ((i - 24 - 25 * 14) % 5)dot[i]->setPosition(Vec2(88 * 60 + 30 + ((i - 24 - 25 * 14) % 5 - 1)*2 * 60, 88 * 60 + 30 + ((i - 24 - 25 * 14) / 5)*2 * 60));
			else if ((i - 24 - 25 * 14) % 5 == 0)dot[i]->setPosition(Vec2(96 * 60 + 30, 88 * 60 + 30 + ((i - 24 - 25 * 14) / 5 - 1)*2 * 60));
		}
		else if (i < 425)
		{
			if ((i - 24 - 25 * 15) % 5)dot[i]->setPosition(Vec2(18 * 60 + 30 + ((i - 24 - 25 * 15) % 5 - 1)*2 * 60, 80 * 60 + 30 + ((i - 24 - 25 * 15) / 5)*2 * 60));
			else if ((i - 24 - 25 * 15) % 5 == 0)dot[i]->setPosition(Vec2(26 * 60 + 30, 80 * 60 + 30 + ((i - 24 - 25 * 15) / 5 - 1)*2 * 60));
		}
		else if (i < 450)
		{
			if ((i - 24 - 25 * 16) % 5)dot[i]->setPosition(Vec2(81 * 60 + 30 + ((i - 24 - 25 * 16) % 5 - 1)*2 * 60, 80 * 60 + 30 + ((i - 24 - 25 * 16) / 5)*2 * 60));
			else if ((i - 24 - 25 * 16) % 5 == 0)dot[i]->setPosition(Vec2(89 * 60 + 30, 80 * 60 + 30 + ((i - 24 - 25 * 16) / 5 - 1)*2 * 60));
		}
		else if (i < 475)
		{
			if ((i - 24 - 25 * 17) % 5)dot[i]->setPosition(Vec2(39 * 60 + 30 + ((i - 24 - 25 * 17) % 5 - 1)*2 * 60, 81 * 60 + 30 + ((i - 24 - 25 * 17) / 5)*2 * 60));
			else if ((i - 24 - 25 * 17) % 5 == 0)dot[i]->setPosition(Vec2(47 * 60 + 30, 81 * 60 + 30 + ((i - 24 - 25 * 17) / 5 - 1)*2 * 60));
		}
		else
		{
			if ((i - 24 - 25 * 18) % 5)dot[i]->setPosition(Vec2(59 * 60 + 30 + ((i - 24 - 25 * 18) % 5 - 1)*2 * 60, 81 * 60 + 30 + ((i - 24 - 25 * 18) / 5)*2 * 60));
			else if ((i - 24 - 25 * 18) % 5 == 0)dot[i]->setPosition(Vec2(67 * 60 + 30, 81 * 60 + 30 + ((i - 24 - 25 * 18) / 5 - 1)*2 * 60));
		}
		MapLayer->addChild(dot[i], 1);
		dotbody[i] = PhysicsBody::createCircle(12,
			PHYSICSBODY_MATERIAL_DEFAULT);
		dotbody[i]->setDynamic(false);
		dot[i]->setPhysicsBody(dotbody[i]);
		dot[i]->setTag(i + 101);
		dotbody[i]->setCategoryBitmask(dotMask);
		dotbody[i]->setCollisionBitmask(heartMask | characterMask | dotMask);
		dotbody[i]->setContactTestBitmask(heartMask | characterMask | dotMask);
	}
}