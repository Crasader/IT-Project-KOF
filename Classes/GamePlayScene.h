//
//  GamePlayScene.h
//  KOF
//
//  Created by Dongtao Yu on 6/09/2015.
//
//

#ifndef __KOF__GamePlayScene__
#define __KOF__GamePlayScene__

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Multiplayer.h"
#include "MainMenuScene.h"
#include "LoadingLayer.h"
#include "SneakyJoystick.h"
#include "SneakyJoystickSkinnedBase.h"
#include "Fighter.h"

class GamePlayScene : public cocos2d::Layer, public AppWarp::NotificationListener, public AppWarp::RoomRequestListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GamePlayScene);
    
    CC_SYNTHESIZE(std::string, playerCharactor, PlayerCharactor);
    CC_SYNTHESIZE(std::string, opponentCharactor, OpponentCharactor);
    
    
    
private:
    
//    std::string playerCharactor;
//    std::string opponentCharactor;
//    
    Fighter* player;
    Fighter* opponent;
    
    
    
    SneakyJoystick* joystick;
    
    void startGame();
    void PauseClicked(Ref*, ui::Widget::TouchEventType);
    void ResumeClicked(Ref*, ui::Widget::TouchEventType);
    void MenuClicked(Ref*, ui::Widget::TouchEventType);
    
    void createJoystick();
    void createBackgroundAnimation();
    
    void update(float);
    
    // Listener
    void onChatReceived(AppWarp::chat);
    void onUserLeftRoom(AppWarp::room, std::string);
    
    
    void onUnsubscribeRoomDone(AppWarp::room);
    void onLeaveRoomDone(AppWarp::room);
};


#endif /* defined(__KOF__GamePlayScene__) */