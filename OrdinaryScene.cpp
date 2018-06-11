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
	auto enemy = Sprite::create("character-forward.png");
	enemy->setAnchorPoint(Vec2(0.5, 0.2));
	enemy->setPosition(Vec2(map->getMapSize().width * 30 + origin.x, map->getMapSize().height * 30 + origin.y));
	this->addChild(enemy, 1);
	/*character->setPosition(Vec2(600,700));*/

	
	auto jingyan = Sprite::create("jingyan.png");
	jingyan->setPosition(Vec2(map->getMapSize().width *30 + origin.x-200, map->getMapSize().height *30 + origin.y));
	this->addChild(jingyan, 1);


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(OrdinaryScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


	PhysicsBody* heartbody[81];

	for (int i = 0; i < 81; i++) 
	{

		heart[i] = Sprite::create("heart.png");
		if (i % 9)heart[i]->setPosition(Vec2((i % 9) * 600, (i / 9 + 1) * 600));
		else if(i % 9==0)heart[i]->setPosition(Vec2( 9 * 600, i / 9  * 600));
		this->addChild(heart[i], 1);
		heartbody[i] = PhysicsBody::createCircle(30,
			PHYSICSBODY_MATERIAL_DEFAULT);
		heartbody[i]->setDynamic(false);
		heart[i]->setPhysicsBody(heartbody[i]);
		heart[i]->setTag(i+1);
		heartbody[i]->setCategoryBitmask(heartMask);
		heartbody[i]->setCollisionBitmask(heartMask | characterMask);
		heartbody[i]->setContactTestBitmask(heartMask | characterMask);
	}
	PhysicsBody* characterbody = PhysicsBody::createBox(character->getContentSize());
	characterbody->setDynamic(false);
	character->setPhysicsBody(characterbody);
	character->setTag(0);
	characterbody->setCategoryBitmask(characterMask);
	characterbody->setCollisionBitmask(heartMask | characterMask);
	characterbody->setContactTestBitmask(heartMask | characterMask);

	//auto edgeSpace = Sprite::create();
	//PhysicsBody* boundbody==PhysicsBody


	this->scheduleUpdate();//���붨ʱ��


	auto listener = EventListenerKeyboard::create();//������create
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) //�������̵İ���
	{
		keys[keyCode] = true;
	};

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) //�������̵��ɿ�
	{
		keys[keyCode] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



	auto myMouseListener = EventListenerMouse::create();//�����¼�����������¼�
														//����ƶ�
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
			arrow->setPosition(character->getPosition());

			auto e = (EventMouse*)event;
			auto playerPos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
			Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
			Point vector = mousePos - playerPos;
			auto rotateRadians = vector.getAngle();
			auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians) + 90;
			arrow->runAction(RotateTo::create(0.001, rotateDegrees));
			Vec2 direction = mousePos - playerPos;
			direction = direction / direction.getLength();
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
	if (tagA == 0 && !heartiseaten[tagB - 1])
	{
		heartiseaten[tagB - 1] = true;
	}
	else if (tagB == 0 && !heartiseaten[tagA - 1])
	{
		heartiseaten[tagA - 1]=true;
	}
	return true;
}
void OrdinaryScene::update(float delta) //ÿ֡���еĸ��²���
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
	for (int i = 0; i < 81; i++)
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
}

bool OrdinaryScene::isKeyPressed(EventKeyboard::KeyCode keyCode)//�ж��Ƿ�һֱ����ס
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

void OrdinaryScene::keyPressedDuration(EventKeyboard::KeyCode code) //������еĲ���
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
	auto moveTo = MoveTo::create(0.5, Vec2(character->getPositionX() + offsetX, character->getPositionY() + offsetY));
    character->runAction(moveTo);
	
}

void initHeart()
{

}

void initStar()
{

}