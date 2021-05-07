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

class HelloWorld : public cocos2d::Scene
{
public:
    //иначе tinyxml не может их схоронить
    float coefSpeed;
    float coefWheels;
    float stopDist = 0;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    //функци€ обновлени€ кадра
    void update(float) override;

    //если была нажата кака€-нибудь клавиша
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, Event* event);

    //сохран€ем настройки в xml-файл...
    void save_settings();
    //...а теперь вытаскиваем
    bool load_settings();

    //алгоритм выбора тормозного пути
    float getStopDist();

    //функци€ плавности; использован метод Lerp - линейна€ интерпол€ци€
    float Easing(float xStart, float xEnd, float time);

private:
    
    Sprite* w1;
    Sprite* w2;
    Sprite* road;

    bool keyPressed;
    bool stop;
    Size visibleSize;
    Vec2 origin;
    //длины тормозных путей  (в пиксел€х) с весами
    std::map<float, float> mapStopDist = { {167.746f, 0.3f}, {293.56f, 0.3f}, {391.41f, 0.23f}, {587.112f, 0.17f} };
};

#endif // __HELLOWORLD_SCENE_H__
