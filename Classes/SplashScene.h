//
//  SplashScene.h
//  KOF
//
//  Created by Dongtao Yu on 12/08/2015.
//
//

#ifndef __KOF__SplashScene__
#define __KOF__SplashScene__

#include "cocos2d.h"
#include "CocosGUI.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "Multiplayer.h"
#include <random>
#include <ctime> 

class SplashScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);
    
private:
    void GoToMainMenuScene( float dt );
    
    void updateLoadingBar( float dt );
    std::string randomString(size_t);

    
};

#endif /* defined(__KOF__SplashScene__) */
