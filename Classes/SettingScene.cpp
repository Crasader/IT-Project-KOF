//
//  SettingScene.cpp
//  KOF
//
//  Created by Dongtao Yu on 15/08/2015.
//
//

#include "SettingScene.h"

Scene* SettingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getWinSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    
    //TODO: help text
    auto node = CSLoader::createNode("Setting.csb");
    ui::Button* buttonBack =  static_cast<ui::Button*>(node->getChildByName("buttonBack"));
    buttonBack->addTouchEventListener(CC_CALLBACK_2(SettingScene::GotoMainMenuScene, this));
    this->addChild(node);
    
    //get the node of music slide bar
    ui::Slider* musicSlideBar = static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("sound_slidebar"));
    
    
    //add event listener to call back the function
    musicSlideBar->addEventListener(CC_CALLBACK_2(SettingScene::updateMusicSlideBar, this));
    //save the status of current music bar
    musicSlideBar->setPercent(CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume()/SETTING_SCENE_PERCENTAGE);
    
    //get the node of effect slide bar
    ui::Slider* effectSlidebar = static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("Effect_slidebar"));
    
    //add event listener to call back the function
    effectSlidebar->addEventListener(CC_CALLBACK_2(SettingScene::updateEffectSlideBar, this));
    //save the status of current sound effect bar
    effectSlidebar->setPercent(CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume()/SETTING_SCENE_PERCENTAGE);
    
    
    
    
    //get the node of checkbox
    cocos2d::ui::CheckBox* musicCheckBox = static_cast<cocos2d::ui::CheckBox*>(this->getChildByName("SettingScene")->getChildByName("check_box"));
    
    
    if ( CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume()==0)
    {
        musicCheckBox->setSelected(true);
    }
    musicCheckBox->addEventListener(CC_CALLBACK_2(SettingScene::updateCheckBox, this));
    
    
    
    return true;
}



void SettingScene::GotoMainMenuScene(Ref* pSender, ui::Widget::TouchEventType type)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
    
}

void SettingScene::updateMusicSlideBar(Ref* pSender, ui::Slider::EventType type)
{
    ui::Slider* musicSlideBar =  static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("sound_slidebar"));
    float percent = musicSlideBar->getPercent();
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent*SETTING_SCENE_PERCENTAGE);
}
                                                                               

void SettingScene::updateEffectSlideBar(Ref* pSender, ui::Slider::EventType type)
{
        ui::Slider* effectSlidebar =  static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("Effect_slidebar"));
        float percent = effectSlidebar->getPercent();
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(percent*SETTING_SCENE_PERCENTAGE);
}

void SettingScene::updateCheckBox(Ref *pSender,ui::CheckBox::EventType type)
{
    if (type ==CheckBox::EventType::SELECTED)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        ui::Slider* effectSlidebar =  static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("Effect_slidebar"));
        effectSlidebar->setPercent(0);
        ui::Slider* musicSlideBar =  static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("sound_slidebar"));
        musicSlideBar->setPercent(0);
        
    }
    if (type ==CheckBox::EventType::UNSELECTED)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
        ui::Slider* effectSlidebar =  static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("Effect_slidebar"));
        effectSlidebar->setPercent(100);
        ui::Slider* musicSlideBar =  static_cast<ui::Slider*>(this->getChildByName("SettingScene")->getChildByName("sound_slidebar"));
        musicSlideBar->setPercent(100);
    }
}

