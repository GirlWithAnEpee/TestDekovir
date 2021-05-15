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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <map>

USING_NS_CC;

struct StopInfo //для хранения необходимых для остановки данных
{
    float stopDist = 0;
    float passedTime = 0; //сколько времени уже прошло с начала остановки
    float stopTime; //сколько времени потребуется на остановку при текущей скорости
    //начальные и конечные значения поворота колеса и движения дороги
    float roadXStart[3];
    float rotStart;
    float roadXEnd[3];
    float rotEnd = 0.0f;
};

class HelloWorld : public cocos2d::Scene
{
public:
    //иначе tinyxml не может их схоронить
    float coefSpeed;
    float coefWheels;
    
    StopInfo stopInfo;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    //функция обновления кадра
    void update(float) override;

    //если была нажата какая-нибудь клавиша
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, Event* event);

    //сохраняем настройки в xml-файл...
    void save_settings();
    //...а теперь вытаскиваем
    bool load_settings();

    //алгоритм выбора тормозного пути
    float getStopDist();

    //функция плавности
    float Easing(float xStart, float xEnd, float time);

private:
    
    Sprite* w1;
    Sprite* w2;
    Vector<Sprite*> roadSections;
    const int sectionWidth = 1798;

    bool keyPressed;
    bool stop;
    Size visibleSize;
    Vec2 origin;
    //длины тормозных путей (в пикселях) с весами
    std::map<float, int> mapStopDist = { {167.746f, 4}, {293.56f, 3}, {391.41f, 2}, {387.112f, 2} };
};

#endif // __HELLOWORLD_SCENE_H__
