//
//  LoadingLayer.cpp
//  KOF
//
//  Created by Dongtao Yu on 7/09/2015.
//
//

#include "LoadingLayer.h"

USING_NS_CC;


bool LoadingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(cocos2d::Color4B(0 , 0, 0, 0)))
    {
        return false;
    }
    
    this->setName("LoadingLayer");
    auto node = CSLoader::createNode("LoadingLayer.csb");
    this->setOpacity(80);
    this->addChild(node);
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("countdown.plist");
    AnimationCache::getInstance()->addAnimationsWithFile("countdown-animation.plist");
    

    
    return true;
}

void LoadingLayer::AddLoadingLayer(Node* scene)
{
    auto node = LoadingLayer::create();
    scene->addChild(node);
}

void LoadingLayer::RemoveLoadingLayer(Node* scene)
{
    scene->removeChildByName("LoadingLayer");
}


void LoadingLayer::AppendText(Node* scene, std::string value)
{
    if(!isLoadingLayer(scene))
        AddLoadingLayer(scene);
    auto node = scene->getChildByName("LoadingLayer")->getChildByName("LoadingLayer")->getChildByName<ui::Text*>("text");
    node->setString(node->getString() + value);
}

void LoadingLayer::SetText(Node* scene, std::string value)
{
    if(!isLoadingLayer(scene))
        AddLoadingLayer(scene);
    auto node = scene->getChildByName("LoadingLayer")->getChildByName("LoadingLayer")->getChildByName<ui::Text*>("text");
    node->setString(value);
}

void LoadingLayer::SetLoadingBarPercentage(Node* scene, float value)
{
    if(!isLoadingLayer(scene))
        AddLoadingLayer(scene);
    auto node = scene->getChildByName("LoadingLayer")->getChildByName("LoadingLayer")->getChildByName<ui::LoadingBar*>("loadingBar");
    node->setPercent(value);
}

void LoadingLayer::SetTextAndLoadingBar(Node* scene, bool append, std::string string, float value)
{
    if(!isLoadingLayer(scene))
        AddLoadingLayer(scene);
    if(append)
        AppendText(scene, string);
    else
        SetText(scene, string);
    
    SetLoadingBarPercentage(scene, value);
    
}


void LoadingLayer::StartCountDown(Node* scene, cocos2d::CallFunc* callback)
{
    if(!isLoadingLayer(scene))
        AddLoadingLayer(scene);
    
    auto node = scene->getChildByName("LoadingLayer")->getChildByName("LoadingLayer");
    node->getChildByName("loadingBar")->setVisible(false);
    node->getChildByName("loadingBarBorder")->setVisible(false);
    node->getChildByName("background")->setOpacity(0);
    auto countdown = node->getChildByName<Sprite*>("countdown");
    auto animation = AnimationCache::getInstance()->getAnimation("countdown");
    countdown->setScale(0.7);
    countdown->setVisible(true);
    
    
    if (callback)
        countdown->runAction(Sequence::createWithTwoActions(Animate::create(animation), callback));
    else
        countdown->runAction(Animate::create(animation));
    
    
}


bool LoadingLayer::isLoadingLayer(Node* scene)
{
    if(scene->getChildByName("LoadingLayer"))
        return true;
    return false;
}