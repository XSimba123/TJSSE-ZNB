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
	auto scene = Scene::createWithPhysics();
	/*scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);*/
	auto layer = OrdinaryScene::create();
	scene->addChild(layer);
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


	map = TMXTiledMap::create("mainmap.tmx");
	this->addChild(map,0,100);
	wall = map->getLayer("wall");




	/*auto group = map->getObjectGroup("wall");
	ValueMap spawnPoint = group->getObject("character");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();
*/

	character = Sprite::create("character-forward.png");
	character->setAnchorPoint(Vec2(0.5,0.2));
	character->setPosition(Vec2(map->getMapSize().width *30 + origin.x, map->getMapSize().height *30 + origin.y));
	this->addChild(character,1);
	this->runAction(Follow::create(character, Rect::ZERO));
	enemy = Sprite::create("character-forward.png");
	enemy->setAnchorPoint(Vec2(0.5, 0.2));
	enemy->setPosition(Vec2(map->getMapSize().width * 30 + origin.x, map->getMapSize().height * 30 + origin.y));
	this->addChild(enemy, 1);
	PhysicsBody* enemybody = PhysicsBody::createBox(enemy->getContentSize());
	enemybody->setDynamic(false);
	enemy->setPhysicsBody(enemybody);
	enemy->setTag(1);
	enemybody->setCategoryBitmask(arrowMask);
	enemybody->setCollisionBitmask(heartMask | characterMask | dotMask | arrowMask|enemyMask);
	enemybody->setContactTestBitmask(heartMask | characterMask | dotMask | arrowMask | enemyMask);
	/*character->setPosition(Vec2(600,700));*/

	


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(OrdinaryScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


	PhysicsBody* heartbody[25];
	PhysicsBody* dotbody[2000];

	for (int i = 0; i < 25; i++) 
	{
		heart[i] = Sprite::create("heart.png");
		if ((i + 1) % 5)heart[i]->setPosition(Vec2(600 + ((i + 1) % 5 - 1) * 1200 - 30, 600 + ((i + 1) / 5) * 1200 - 30));
		else if ((i + 1) % 5 == 0)heart[i]->setPosition(Vec2(5400 - 30, 600 + (((i + 1) / 5) - 1) * 1200 - 30));
		this->addChild(heart[i], 1);
		heartbody[i] = PhysicsBody::createCircle(30,
			PHYSICSBODY_MATERIAL_DEFAULT);
		heartbody[i]->setDynamic(false);
		heart[i]->setPhysicsBody(heartbody[i]);
		heart[i]->setTag(i+2);
		heartbody[i]->setCategoryBitmask(heartMask);
		heartbody[i]->setCollisionBitmask(heartMask | characterMask | dotMask);
		heartbody[i]->setContactTestBitmask(heartMask | characterMask | dotMask);
	}
	for (int i = 0; i < 2000; i++)
	{
		dot[i] = Sprite::create("jingyan.png");
		if (i < 400)
		{
			if ((i + 1) % 20)dot[i]->setPosition(Vec2(210 + ((i + 1) % 20 - 1) * 60 , 210 + ((i + 1) / 20) * 60 ));
			else if ((i + 1) % 20 == 0)dot[i]->setPosition(Vec2(750, 210 + ((i + 1) / 20 - 1) * 60));
		}
		else if (i < 800)
		{
			if ((i -399) % 20)dot[i]->setPosition(Vec2(4650 + ((i -399) % 20 - 1) * 60, 210 + ((i -399) / 20) * 60));
			else if ((i -399) % 20 == 0)dot[i]->setPosition(Vec2(5790, 210 + ((i -399) / 20 - 1) * 60));
		}
		else if (i < 1200)
		{
			if ((i -799) % 20)dot[i]->setPosition(Vec2(210 + ((i - 799) % 20 - 1) * 60, 4650 + ((i - 799) / 20) * 60));
			else if ((i -799) % 20 == 0)dot[i]->setPosition(Vec2(750, 4650 + ((i - 799) / 20 - 1) * 60));
		}
		else if (i < 1600)
		{
			if ((i - 1199) % 20)dot[i]->setPosition(Vec2(4650 + ((i - 1199) % 20 - 1) * 60, 4650 + ((i - 1199) / 20) * 60));
			else if ((i - 1199) % 20 == 0)dot[i]->setPosition(Vec2(5790, 4650 + ((i - 1199) / 20 - 1) * 60));
		}
		else if (i < 2000)
		{
			if ((i - 1599) % 20)dot[i]->setPosition(Vec2(2430 + ((i - 1599) % 20 - 1) * 60, 2430 + ((i - 1599) / 20) * 60));
			else if ((i - 1599) % 20 == 0)dot[i]->setPosition(Vec2(3570, 2430 + ((i - 1599) / 20 - 1) * 60));
		}
		this->addChild(dot[i], 1);
		dotbody[i] = PhysicsBody::createCircle(30,
			PHYSICSBODY_MATERIAL_DEFAULT);
		dotbody[i]->setDynamic(false);
		dot[i]->setPhysicsBody(dotbody[i]);
		dot[i]->setTag(i + 50);
		dotbody[i]->setCategoryBitmask(dotMask);
		dotbody[i]->setCollisionBitmask(heartMask | characterMask | dotMask);
		dotbody[i]->setContactTestBitmask(heartMask | characterMask | dotMask);
	}
	PhysicsBody* characterbody = PhysicsBody::createBox(character->getContentSize());
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
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



	auto myMouseListener = EventListenerMouse::create();//创建事件监听器鼠标事件
														//鼠标移动
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myMouseListener, this);
	myMouseListener->onMouseMove = [=](Event *event)
	{
		auto e = (EventMouse*)event;
		auto playerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
		Point vector = mousePos - playerPos;
		auto rotateRadians = vector.getAngle();
		auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians) + 90;
		character->runAction(RotateTo::create(0.01, rotateDegrees));
	};

	myMouseListener->onMouseUp = [=](Event *event)
	{
		if (!isAttacking)
		{
			arrow = Sprite::create("arrow.png");
			PhysicsBody* arrowbody = PhysicsBody::createBox(arrow->getContentSize());
			arrowbody->setDynamic(false);
			arrow->setPhysicsBody(arrowbody);
			arrow->setTag(30);
			arrowbody->setCategoryBitmask(arrowMask);
			arrowbody->setCollisionBitmask(heartMask | characterMask | dotMask | arrowMask);
			arrowbody->setContactTestBitmask(heartMask | characterMask | dotMask | arrowMask);

			auto e = (EventMouse*)event;
			auto playerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
			Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
			Point vector = mousePos - playerPos;
			auto rotateRadians = vector.getAngle();
			auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians) + 90;
			arrow->runAction(RotateTo::create(0, rotateDegrees));
			direction = mousePos - playerPos;
			direction = direction / direction.getLength();
			arrow->setPosition(character->getPosition()+direction*100);
			direction *= 380;
			arrow->runAction(MoveBy::create(0.1, direction));
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shoot crossbow.wav");
			this->addChild(arrow, 2);
			isAttacking = true;
			
			this->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create([&]() {
				isAttacking = false;
				this->removeChild(arrow, true);

			}), NULL));
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
	if (tagA == 0 && tagA >= 2 && tagB <= 29 && !heartiseaten[tagB - 2])
	{
		heartiseaten[tagB - 2] = true;
	}
	else if (tagB == 0 && tagA >= 2 && tagA <= 29 && !heartiseaten[tagA - 2])
	{
		heartiseaten[tagA - 2] = true;
	}
	if (tagA == 0 && tagB >= 50 && !dotiseaten[tagB - 50])
	{
		dotiseaten[tagB - 50] = true;
	}
	else if (tagB == 0 && tagA >= 50 && !dotiseaten[tagA - 50])
	{
		dotiseaten[tagA - 50] = true;
	}
	if (tagA == 30 && tagB == 1)
	{
		beAttacken = true;
		spriteA->stopAllActions();
	}
	else if (tagB == 30 && tagA == 1)
	{
		beAttacken = true;
		spriteB->stopAllActions();
	}
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


	int nowTime = GetCurrentTime();
	if (beAttacken)
	{
		enemy->runAction(Blink::create(0.1, 1));
		enemy->runAction(TintTo::create(0.05, 255, 0, 0));
		beginblink = true;
		coloroldtime = GetCurrentTime();
		beAttacken = false;
	}
	if (beginblink)
	{
		if (nowTime-coloroldtime >= 200)
		{
			enemy->runAction(TintTo::create(0.1, 255, 255, 255));
			beginblink = false;
		}
	}

	for (int i = 0; i < 25; i++)
	{
		if (heartiseaten[i] && !heartisloading[i])
		{
			heart[i]->runAction(FadeOut::create(0.5));
			this->heartoldTime[i] = GetCurrentTime();
			heartisloading[i] = true;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
		}
		if (heartisloading[i])
		{
			if(nowTime - heartoldTime[i] >= 30000)
			{ 
				heart[i]->runAction(FadeIn::create(1));
				heartisloading[i] = false;
				heartiseaten[i] = false;
			}
		}
	}
	for (int i = 0; i < 2000; i++)
	{
		if (dotiseaten[i] && !dotisloading[i])
		{
			dot[i]->runAction(FadeOut::create(0.5));
			this->dotoldTime[i] = GetCurrentTime();
			dotisloading[i] = true;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
		}
		if (dotisloading[i])
		{
			if (nowTime - dotoldTime[i] >= 60000)
			{
				dot[i]->runAction(FadeIn::create(1));
				dotisloading[i] = false;
				dotiseaten[i] = false;
			}
		}
	}
	if (isAttacking) 
	{
		Vec2 tileCoord = Vec2(arrow->getPosition().x / 60, 100 - arrow->getPosition().y / 60);
		if(direction.x>0)tileCoord += Vec2(1,0);
		else if (direction.x<0)tileCoord += Vec2(-1, 0);
		if (direction.y>0)tileCoord -= Vec2(0, 1);
		else if (direction.y<0)tileCoord += Vec2(0, 1);
		int tileGid = wall->getTileGIDAt(tileCoord);
		if (tileGid > 0)
		{
			/*Value prop = map->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();
			bool collision = propValueMap["collidable"].asBool();*/
			arrow->stopAllActions();
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
		offsetX = -8;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX = 8;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY = 8;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY = -8;
		break;
	default:
		offsetY = offsetX = 0;
		break;
	}
	Vec2 tileCoord = Vec2(character->getPositionX() / 60, 100 - character->getPositionY() / 60);
	if (offsetX > 0)tileCoord += Vec2(1, 0);
	else if (offsetX < 0)tileCoord -= Vec2(1, 0);
	if (offsetY > 0)tileCoord -= Vec2(0,1);
	else if (offsetY < 0)tileCoord += Vec2(0, 1);
	int tileGid = wall->getTileGIDAt(tileCoord);
	if (tileGid > 0)
	{
		/*Value prop = map->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();
		bool collision = propValueMap["collidable"].asBool();*/
		character->stopAllActions();
		offsetY = offsetX = 0;
	}
	auto moveTo = MoveTo::create(0.5, Vec2(character->getPositionX() + offsetX, character->getPositionY() + offsetY));
	character->runAction(moveTo);
	
}

void initHeart()
{

}

void initStar()
{

}