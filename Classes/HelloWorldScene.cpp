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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "external\tinyxml2\tinyxml2.h"
#include <iostream>

USING_NS_CC;
using namespace tinyxml2;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    if (load_settings() == false)
    {
        coef_speed = 1;
        coef_wheels = 1;
    }

    KeyPressed = false;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    //create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // create and initialize a label

    auto label = Label::createWithTTF("Press Space to stop the car", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    road = Sprite::create("longroad.png");

    if (road == nullptr)
    {
        problemLoading("'longroad.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        road->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 4 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(road, 0);
    }

    //машина
    auto car = Sprite::create("car1.png");
    if (car == nullptr)
    {
        problemLoading("'car1.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        car->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        // add the sprite as a child to this layer
        this->addChild(car, 0);
    }

    //левое колесо
    w1 = Sprite::create("wheel.png");
    if (w1 == nullptr)
    {
        problemLoading("'wheel.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        w1->setPosition(Vec2(car->getContentSize().width * 0.5 + 8, car->getContentSize().height * 0.75 - 20));

        // add the sprite as a child to this layer
        this->addChild(w1, 0);
    }

    //правое колесо
    w2 = Sprite::create("wheel.png");//82
    if (w2 == nullptr)
    {
        problemLoading("'wheel.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        w2->setPosition(Vec2(visibleSize.width * 0.75 - 23, car->getContentSize().height * 0.75 - 20));

        // add the sprite as a child to this layer
        this->addChild(w2, 0);
    }

    w1->setScale(coef_wheels);
    w2->setScale(coef_wheels);

    auto listener = EventListenerKeyboard::create();                         
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this); 
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); 
    this->scheduleUpdate();
    return true;
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        coef_speed -= 10;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        coef_speed += 10;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
        coef_wheels += 0.25;
        KeyPressed = true;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
        coef_wheels -= 0.25;
        KeyPressed = true;
        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        coef_speed = 0;
        break;
    default:
        break;
    }
}

void HelloWorld::update(float delta) {

    //поворот колёс
    auto rot = w1->getRotation();
 
    rot += 10 * delta * coef_speed;
    w1->setRotation(rot);
    w2->setRotation(rot);
    
    //масштабирование колёс
    if (KeyPressed)
    {
        w1->setScale(coef_wheels);
        w2->setScale(coef_wheels);
    }

    //движение дороги
    if ((road->getPositionX() - coef_speed * 0.5)<= origin.x)
        road->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 4 + origin.y));
    else
        road->setPosition(road->getPositionX() - coef_speed * 0.5, road->getPositionY());
}

void HelloWorld::save_settings()
{
    tinyxml2::XMLDocument doc;

    //проверяем, вдруг такой файл уже есть
    if (doc.LoadFile("settings.xml") == XML_SUCCESS)
    {
        //получаем корневой элемент
        XMLNode* root = doc.RootElement();

        //перезаписываем
        XMLElement* pElement1 = doc.FirstChildElement("SETTINGS")->FirstChildElement("Speed");
        pElement1->SetText(coef_speed);
        XMLElement* pElement2 = doc.FirstChildElement("SETTINGS")->FirstChildElement("Wheels");
        pElement2->SetText(coef_wheels);
    }
    
    else //если файла таки нет
    {
        //корневой элемент
        XMLNode* pRoot = doc.NewElement("SETTINGS");
        doc.InsertFirstChild(pRoot);

        //остальные
        XMLElement* pElement1 = doc.NewElement("Speed");
        pElement1->SetText(coef_speed);
        pRoot->InsertEndChild(pElement1);

        XMLElement* pElement2 = doc.NewElement("Wheels");
        pElement2->SetText(coef_wheels);
        pRoot->InsertEndChild(pElement2);
    }

    doc.SaveFile("settings.xml");
}

bool HelloWorld::load_settings()
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("settings.xml") == XML_SUCCESS)
    {
        //проверяем, есть ли корень
        XMLNode* pRoot = doc.FirstChild();
        if (pRoot == nullptr) return false;

        //проверяем каждый узел, если есть, считываем значение
        XMLElement* pElement1 = pRoot->FirstChildElement("Speed");
        if (pElement1 == nullptr)
            return false;
        else
            pElement1->QueryIntText(&coef_speed);

        XMLElement* pElement2 = pRoot->FirstChildElement("Wheels");
        if (pElement2 == nullptr)
            return false;
        else
            pElement2->QueryIntText(&coef_wheels);

        return true;
    }
    else
        return false;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //сохраняем настройки
    save_settings();

    //закрываем сцену и выходим
    Director::getInstance()->end();
}
