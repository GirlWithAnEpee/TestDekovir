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
        coefSpeed = 1;
        coefWheels = 1;
    }

    keyPressed = false;
    stop = false;

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

    roadSections.reserve(3);
    roadSections.pushBack(Sprite::create("longroad.png"));
    roadSections.pushBack(Sprite::create("longroad.png"));
    roadSections.pushBack(Sprite::create("longroad.png"));

    if ((*roadSections.begin()) == nullptr)
    {
        problemLoading("'longroad.png'");
    }
    else
    {
        auto xPosition = visibleSize.width / 2 + origin.x;
        for (auto&& roadSection : roadSections)
        {
            roadSection->setPosition(xPosition, visibleSize.height / 4 + origin.y);
            xPosition += roadSection->getContentSize().width;  // or you can use xPosition += sectionWidth;
            addChild(roadSection);
        }
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

    w1->setScale(coefWheels);
    w2->setScale(coefWheels);

    auto listener = EventListenerKeyboard::create();                         
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this); 
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); 
    this->scheduleUpdate();
    return true;
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    
    switch (keyCode) {

    //изменять скорость - клавиши A, D, Влево и Вправо
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        coefSpeed -= 10;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        coefSpeed += 10;
        break;
        
    //управлять диаметром колеса - клавиши W, S, Вверх и Вниз
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_W:
        coefWheels += 0.25;
        keyPressed = true;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_S:
        coefWheels -= 0.25;
        keyPressed = true;
        break;
    //остановка машины - пробел
    case EventKeyboard::KeyCode::KEY_SPACE:
    {
        stop = true;
        if (stopInfo.stopDist == 0) //если тормозной путь не был выбран, выбираем рандомно
            stopInfo.stopDist = getStopDist();
        stopInfo.stopTime = 2 * stopInfo.stopDist / coefSpeed;//сколько времени потребуется на остановку при текущей скорости
        //начальные и конечные значения поворота колеса и движения дороги
        int i = 0;
        for (auto&& road : roadSections)
        {
            stopInfo.roadXStart[i] = road->getPositionX();
            stopInfo.roadXEnd[i] = stopInfo.roadXStart[i] + stopInfo.stopDist;
            i++;
        }
        stopInfo.rotStart = w1->getRotation();
        stopInfo.rotEnd = stopInfo.rotStart - stopInfo.stopDist / (2 * 3.14 * 33 * coefWheels) * 360;
        break;
    }

    //изменять тормозной путь - клавишами 1-4
    case EventKeyboard::KeyCode::KEY_1:
        stopInfo.stopDist = mapStopDist[0];
        break;
    case EventKeyboard::KeyCode::KEY_2:
        stopInfo.stopDist = mapStopDist[1];
        break;
    case EventKeyboard::KeyCode::KEY_3:
        stopInfo.stopDist = mapStopDist[2];
        break;
    case EventKeyboard::KeyCode::KEY_4:
        stopInfo.stopDist = mapStopDist[3];
        break;
    default:
        break;
    }
}

float HelloWorld::getStopDist()
{
    int weightSum = 0;
    
    for (std::map <float, int> ::iterator it = mapStopDist.begin(); it != mapStopDist.end(); it++)
    {
        weightSum += it->second;
    }

    float rnd = rand() * weightSum / RAND_MAX;

    for (std::map <float, int> ::iterator it = mapStopDist.begin(); it != mapStopDist.end(); it++)
    {
        if (rnd < it->second)
            return it->first;
        rnd -= it->second;
    }
}

float f(float x)
{
    return -pow(x,0.5);
}

float HelloWorld::Easing(float xStart, float xEnd, float time)
{
    return (xStart + (xEnd - xStart) * f(time));
}

void HelloWorld::update(float delta) {
    if (stop)//если машина была остановлена
    {
        if(stopInfo.passedTime <= stopInfo.stopTime) //пока время не кончилось
        {
            int i = 0;
            for (auto&& road : roadSections)
            {
                road->setPositionX(Easing(stopInfo.roadXStart[i], stopInfo.roadXEnd[i], stopInfo.passedTime / stopInfo.stopTime));
                i++;
                //если секция находится за пределами видимости,
                //перемещаем её в начало
                if (road->getBoundingBox().getMaxX() < 0)
                {
                    road->setPositionX(road->getPositionX() + sectionWidth * roadSections.size());
                }
            }
 
            w1->setRotation(Easing(stopInfo.rotStart, stopInfo.rotEnd, stopInfo.passedTime / stopInfo.stopTime));
            w2->setRotation(Easing(stopInfo.rotStart, stopInfo.rotEnd, stopInfo.passedTime / stopInfo.stopTime));
            stopInfo.passedTime += delta;
        }
    }
    else
    {
        //поворот колёс
        if (coefSpeed >= 0)
        {
            auto rot = w1->getRotation();
            //33 пикселя - радиус колеса, умножение на 360 необходимо для перевода в градусы
            rot += coefSpeed / (2 * 3.14 * 33 * coefWheels) * delta * 360;
            w1->setRotation(rot);
            w2->setRotation(rot);

            for (auto&& road : roadSections)
            {
                road->setPositionX(road->getPositionX() - coefSpeed*delta);
                //если секция находится за пределами видимости
                if (road->getBoundingBox().getMaxX() < 0)
                {
                    road->setPositionX(road->getPositionX() + sectionWidth*roadSections.size()-4);  // position the current section so it is on the other side of the screen
                }
            }
        }
    }
    //масштабирование колёс
    if (keyPressed && coefWheels > 0)
    {
        w1->setScale(coefWheels);
        w2->setScale(coefWheels);
    }
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
        pElement1->SetText(coefSpeed);
        XMLElement* pElement2 = doc.FirstChildElement("SETTINGS")->FirstChildElement("Wheels");
        pElement2->SetText(coefWheels);
        XMLElement* pElement3 = doc.FirstChildElement("SETTINGS")->FirstChildElement("StopDist");
        pElement3->SetText(stopInfo.stopDist);
    }
    
    else //если файла таки нет
    {
        //корневой элемент
        XMLNode* pRoot = doc.NewElement("SETTINGS");
        doc.InsertFirstChild(pRoot);

        //остальные
        XMLElement* pElement1 = doc.NewElement("Speed");
        pElement1->SetText(coefSpeed);
        pRoot->InsertEndChild(pElement1);

        XMLElement* pElement2 = doc.NewElement("Wheels");
        pElement2->SetText(coefWheels);
        pRoot->InsertEndChild(pElement2);

        XMLElement* pElement3 = doc.NewElement("StopDist");
        pElement3->SetText(stopInfo.stopDist);
        pRoot->InsertEndChild(pElement3);
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
            pElement1->QueryFloatText(&coefSpeed);

        XMLElement* pElement2 = pRoot->FirstChildElement("Wheels");
        if (pElement2 == nullptr)
            return false;
        else
            pElement2->QueryFloatText(&coefWheels);
        
        XMLElement* pElement3 = pRoot->FirstChildElement("StopDist");
        if (pElement3 == nullptr)
            return false;
        else
            pElement3->QueryFloatText(&stopInfo.stopDist);

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
