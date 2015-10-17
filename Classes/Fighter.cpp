//
//  Fighter.cpp
//  KOF
//
//  Created by Dongtao Yu on 11/09/2015.
//
//

#include "Fighter.h"

Fighter::Fighter(cocos2d::Sprite* sprite, cocos2d::ui::LoadingBar* health , std::string name, bool isLeft)
{
    this->name = name;
    this->isLeft = isLeft;
    this->sprite = sprite;
    this->health = health;
    this->isDie = false;
    this->sprite->setSpriteFrame(fmt::format(CHARACTER_INITIAL_FRAME, name));
    this->start();
}


void Fighter::update(float)
{
    
}


cocos2d::Vec2 Fighter::getPosition()
{
    return this->sprite->getPosition();
}

void Fighter::setPosition(cocos2d::Vec2 pos)
{
    if (isLeft)
    {
        if (pos.x > opponent->getPosition().x)
            return;
    }
    else
    {
        if (pos.x < opponent->getPosition().x)
            return;
    }
    this->sprite->setPosition(pos);
}

cocos2d::Rect Fighter::getBoundingBox()
{
    return this->sprite->getBoundingBox();
}

cocos2d::Vec2 Fighter::getScreenPosition()
{
    return this->sprite->getParent()->convertToWorldSpace(this->getPosition());
}

cocos2d::Node* Fighter::getParent()
{
    return this->sprite->getParent();
}


void Fighter::processCommand(command_t cmd)
{
    if (this->getIsDie() || opponent->getIsDie())
        return;
    
    switch (cmd.operation) {
        case OP_GPS_ACTION_1_STAND:
            this->stand();
            break;
        
        case OP_GPS_ACTION_1_STAND_MOVEFORWARD:
            this->stand_moveforward();
            break;
            
        case OP_GPS_ACTION_1_STAND_MOVEBACK:
            this->stand_moveback();
            break;
            
        case OP_GPS_ACTION_2_STAND_JUMP:
            this->stand_jump(atoi(GameHelper::split(cmd.properties, '%').at(0).c_str()));
            break;
            
        case OP_GPS_ACTION_1_SQUAT_DOWN:
            this->squat_down();
            break;
            
        case OP_GPS_ACTION_1_SQUAT_MOVEFORWARD:
            this->squat_moveforward();
            break;
        
        case OP_GPS_ACTION_1_SQUAT_MOVEBACK:
            this->squat_moveback();
            break;
            
        case OP_GPS_ACTION_2_STAND_PUNCH1:
            this->punch1();
            break;
            
        case OP_GPS_ACTION_2_STAND_PUNCH2:
            this->punch2();
            break;
            
        case OP_GPS_ACTION_2_STAND_KICK1:
            this->kick1();
            break;
            
        case OP_GPS_ACTION_2_STAND_KICK2:
            this->kick2();
            break;
            
        default:
            break;
    }
}

void Fighter::stand()
{
//    CCLOG("in stand: running actions: %zd", this->sprite->getNumberOfRunningActions());
    if (!isStand() && (this->sprite->getNumberOfRunningActions() < 1 || isActionStoppable()))
    {
//        this->physicsSprite->stopAllActions();
        this->sprite->stopAllActions();
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND, name));
        auto animate = cocos2d::Animate::create(animation);
        auto repeat = cocos2d::RepeatForever::create(animate);
        repeat->setTag(OP_GPS_ACTION_1_STAND);
        this->sprite->runAction(repeat);
    }
}

void Fighter::stand_hit()
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_HIT, name));
    auto animate = cocos2d::Animate::create(animation);
    animate->setDuration(0.8f);
    auto func = [&]{
        this->sprite->stopAllActions();
        this->stand();
    };
    auto sequence = cocos2d::Sequence::create(animate, cocos2d::CallFunc::create(func), NULL);
    sequence->setTag(OP_GPS_ACTION_2_STAND_HIT);
    this->sprite->runAction(sequence);

}

void Fighter::stand_jump(int distance)
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_JUMP, name));
    auto animate = cocos2d::Animate::create(animation);
    auto jumpBy = cocos2d::JumpBy::create(animate->getDuration(), cocos2d::Vec2(distance, 0), 300.0f, 1);
    auto callFunc = cocos2d::CallFunc::create([&]{this->sprite->stopAllActions();this->stand();});
    auto spawn = cocos2d::Spawn::create(animate, jumpBy, NULL);
    auto sequence = cocos2d::Sequence::create(spawn, callFunc, NULL);
    sequence->setTag(OP_GPS_ACTION_2_STAND_JUMP);
    this->sprite->runAction(sequence);
    
}



void Fighter::stand_moveback()
{
    if (!this->sprite->getActionByTag(ANIMATION_ACTION_1_STAND_MOVEBACK)) {
        this->sprite->stopAllActions();
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_MOVEBACK, name));
        auto animate = cocos2d::Animate::create(animation);
        auto animateForever = cocos2d::RepeatForever::create(animate);
        animateForever->setTag(ANIMATION_ACTION_1_STAND_MOVEBACK);
        this->sprite->runAction(animateForever);
    }
    auto moveBy = cocos2d::MoveBy::create(GAME_FRAME_PER_LOCKSTEP * (GAME_FRAME_LENGTH * 2) / 1000, cocos2d::Vec2(-30, 0));
    moveBy->setTag(OP_GPS_ACTION_1_STAND_MOVEBACK);
    this->sprite->runAction(moveBy);
}



void Fighter::stand_moveforward()
{
    if (!this->sprite->getActionByTag(ANIMATION_ACTION_1_STAND_MOVEFORWARD)) {
        this->sprite->stopAllActions();
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_MOVEFORWARD, name));
        auto animate = cocos2d::Animate::create(animation);
        auto animateForever = cocos2d::RepeatForever::create(animate);
        animateForever->setTag(ANIMATION_ACTION_1_STAND_MOVEFORWARD);
        this->sprite->runAction(animateForever);
    }
    auto moveBy = cocos2d::MoveBy::create(GAME_FRAME_PER_LOCKSTEP * (GAME_FRAME_LENGTH * 2) / 1000, cocos2d::Vec2(30, 0));
    moveBy->setTag(OP_GPS_ACTION_1_STAND_MOVEFORWARD);
    this->sprite->runAction(moveBy);
}


bool Fighter::isNextAction()
{
    if (   this->sprite->getActionByTag(OP_GPS_ACTION_1_STAND_MOVEBACK)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_STAND_MOVEFORWARD)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_STAND_JUMP)
        
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_MOVEBACK)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_MOVEFORWARD)
        
        
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_STAND_KICK1)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_STAND_KICK2)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_STAND_PUNCH1)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_STAND_PUNCH2)
        
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_KICK1)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_KICK2)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_PUNCH1)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_PUNCH2)
        ) {
        return false;
    }
    return true;
}



void Fighter::squat()
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT, name));
    auto animate = cocos2d::Animate::create(animation);
    auto repeat = cocos2d::RepeatForever::create(animate);
    repeat->setTag(OP_GPS_ACTION_1_SQUAT);
    this->sprite->runAction(repeat);
}

void Fighter::squat_down()
{
    if (   !this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_DOWN)
        && !this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT)) {
        
        this->sprite->stopAllActions();
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_DOWN, name));
        auto animate = cocos2d::Animate::create(animation);
        auto func = cocos2d::CallFunc::create([&]{this->sprite->stopAllActions();this->squat();});
        auto sequence = cocos2d::Sequence::create(animate, func, NULL);
        sequence->setTag(OP_GPS_ACTION_1_SQUAT_DOWN);
        this->sprite->runAction(sequence);
    }
}


void Fighter::squat_hit()
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_HIT, name));
    auto animate = cocos2d::Animate::create(animation);
    animate->setDuration(0.5f);
    auto func = [&]{
        this->sprite->stopAllActions();
        this->squat();
    };
    auto sequence = cocos2d::Sequence::create(animate, cocos2d::CallFunc::create(func), NULL);
    sequence->setTag(OP_GPS_ACTION_2_SQUAT_HIT);
    this->sprite->runAction(sequence);
}


void Fighter::squat_moveback()
{
    if (!this->sprite->getActionByTag(ANIMATION_ACTION_1_SQUAT_MOVEBACK)) {
        this->sprite->stopAllActions();
        cocos2d::Animation* animation = NULL;
        if (isLeft)
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_MOVEBACK, name));
        else
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_MOVEFORWARD, name));
        
        
        auto animate = cocos2d::Animate::create(animation);
        auto animateForever = cocos2d::RepeatForever::create(animate);
        animateForever->setTag(ANIMATION_ACTION_1_SQUAT_MOVEBACK);
        this->sprite->runAction(animateForever);
    }
    auto moveBy = cocos2d::MoveBy::create(GAME_FRAME_PER_LOCKSTEP * (GAME_FRAME_LENGTH) / 1000, cocos2d::Vec2(-35, 0));
    moveBy->setTag(OP_GPS_ACTION_1_SQUAT_MOVEBACK);
    this->sprite->runAction(moveBy);
}

void Fighter::squat_moveforward()
{
    if (!this->sprite->getActionByTag(ANIMATION_ACTION_1_SQUAT_MOVEFORWARD)) {
        this->sprite->stopAllActions();
        cocos2d::Animation* animation = NULL;
        if (isLeft)
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_MOVEFORWARD, name));
        else
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_MOVEBACK, name));
        auto animate = cocos2d::Animate::create(animation);
        auto animateForever = cocos2d::RepeatForever::create(animate);
        animateForever->setTag(ANIMATION_ACTION_1_SQUAT_MOVEFORWARD);
        this->sprite->runAction(animateForever);
    }
    auto moveBy = cocos2d::MoveBy::create(GAME_FRAME_PER_LOCKSTEP * (GAME_FRAME_LENGTH) / 1000, cocos2d::Vec2(+35, 0));
    moveBy->setTag(OP_GPS_ACTION_1_SQUAT_MOVEFORWARD);
    this->sprite->runAction(moveBy);
    
//    if(!(this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_MOVEFORWARD)) && isActionStoppable())
//    {
//        this->physicsSprite->stopAllActions();
//        this->sprite->stopAllActions();
//        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_MOVEFORWARD, name));
//        auto animate = cocos2d::Animate::create(animation);
//        auto animateForever = cocos2d::RepeatForever::create(animate);
//        animateForever->setTag(OP_GPS_ACTION_1_SQUAT_MOVEFORWARD);
//        auto moveby = cocos2d::MoveBy::create(animate->getDuration(), cocos2d::Vec2(ACTION_MOVE_SPEED, 0));
//        auto movebyForever = cocos2d::RepeatForever::create(moveby);
//        this->sprite->runAction(animateForever);
//        this->physicsSprite->runAction(movebyForever);
//    }
}

void Fighter::squat_up()
{
    
}

void Fighter::start()
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_START, name));
    auto animate = cocos2d::Animate::create(animation);
    auto sequence = cocos2d::Sequence::create(animate, cocos2d::CallFunc::create([&]{ this->sprite->stopAllActions(); this->stand();}), NULL);
    this->sprite->runAction(sequence);
    
}

void Fighter::win()
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_WIN, name));
    auto animate = cocos2d::Animate::create(animation);
    auto animateForever = cocos2d::RepeatForever::create(animate);
    animateForever->setTag(OP_GPS_ACTION_3_WIN);
    this->sprite->runAction(animateForever);
}

void Fighter::die()
{
    this->sprite->stopAllActions();
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_DIE, name));
    auto func = [&]
    {
        isDie = true;
    };
    auto animate = cocos2d::Animate::create(animation);
    auto sequence = cocos2d::Sequence::create(cocos2d::CallFunc::create(func), animate, NULL);
    sequence->setTag(OP_GPS_ACTION_3_DIE);
    this->sprite->runAction(sequence);
}

void Fighter::kick1()
{
    
        cocos2d::Animation* animation = NULL;
        if (!isSquat())
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_KICK1, name));
        else
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_KICK1, name));
        auto animate = cocos2d::Animate::create(animation);
        
        auto preFunc = [&]
        {
            if (this->isHit())
            {
                auto h = opponent->gethealth();
                opponent->setHealthPercentage(h->getPercent() - KICK1_DAMAGE);
                if (opponent->isStand()) {
                    opponent->stand_hit();
                }
                if (opponent->isSquat()) {
                    opponent->squat_hit();
                }
            }
        };
        
        auto func = [&]{
            if (!this->isSquat()) {
                this->sprite->stopAllActions();
                this->stand();
                return;
            }
            this->squat();
            
        };
        
        
        auto sequence = cocos2d::Sequence::create(cocos2d::CallFunc::create(preFunc), animate, cocos2d::CallFunc::create(func), NULL);
        if(!isSquat())
            sequence->setTag(OP_GPS_ACTION_2_STAND_KICK1);
        else
            sequence->setTag(OP_GPS_ACTION_2_SQUAT_KICK1);
        this->sprite->stopAllActions();
        this->sprite->runAction(sequence);
    
}

void Fighter::kick2()
{
    
        cocos2d::Animation* animation = NULL;
        if (!isSquat())
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_KICK2, name));
        else
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_KICK2, name));
        auto animate = cocos2d::Animate::create(animation);
        
        auto preFunc = [&]
        {
            if (this->isHit())
            {
                auto h = opponent->gethealth();
                opponent->setHealthPercentage(h->getPercent() - KICK1_DAMAGE);
                if (opponent->isStand()) {
                    opponent->stand_hit();
                }
                if (opponent->isSquat()) {
                    opponent->squat_hit();
                }
            }
        };
        
        auto func = [&]{
            if (!this->isSquat()) {
                this->sprite->stopAllActions();
                this->stand();
                return;
            }
            this->squat();
            
        };
        
        
        auto sequence = cocos2d::Sequence::create(cocos2d::CallFunc::create(preFunc), animate, cocos2d::CallFunc::create(func), NULL);

        if(!isSquat())
            sequence->setTag(OP_GPS_ACTION_2_STAND_KICK2);
        else
            sequence->setTag(OP_GPS_ACTION_2_SQUAT_KICK2);
        this->sprite->stopAllActions();
        this->sprite->runAction(sequence);
    
}

void Fighter::punch1()
{
    
        cocos2d::Animation* animation = NULL;
        if (!isSquat())
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_PUNCH1, name));
        else
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_PUNCH1, name));
        auto animate = cocos2d::Animate::create(animation);
        
        auto preFunc = [&]
        {
            if (this->isHit())
            {
                auto h = opponent->gethealth();
                opponent->setHealthPercentage(h->getPercent() - KICK1_DAMAGE);
                if (opponent->isStand()) {
                    opponent->stand_hit();
                }
                if (opponent->isSquat()) {
                    opponent->squat_hit();
                }
            }
        };
        
        auto func = [&]{
            if (!this->isSquat()) {
                this->sprite->stopAllActions();
                this->stand();
                return;
            }
            this->squat();
            
        };
        
        
        auto sequence = cocos2d::Sequence::create(cocos2d::CallFunc::create(preFunc), animate, cocos2d::CallFunc::create(func), NULL);
        if(!isSquat())
            sequence->setTag(OP_GPS_ACTION_2_STAND_PUNCH1);
        else
            sequence->setTag(OP_GPS_ACTION_2_SQUAT_PUNCH1);
        this->sprite->stopAllActions();
        this->sprite->runAction(sequence);
    
}

void Fighter::punch2()
{
    
        cocos2d::Animation* animation = NULL;
        if (!isSquat())
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_STAND_PUNCH2, name));
        else
            animation = cocos2d::AnimationCache::getInstance()->getAnimation(fmt::format(CHARACTER_SQUAT_PUNCH2, name));
        auto animate = cocos2d::Animate::create(animation);
        
        auto preFunc = [&]
        {
            if (this->isHit())
            {
                auto h = opponent->gethealth();
                opponent->setHealthPercentage(h->getPercent() - KICK1_DAMAGE);
                if (opponent->isStand()) {
                    opponent->stand_hit();
                }
                if (opponent->isSquat()) {
                    opponent->squat_hit();
                }
            }
        };
        
        auto func = [&]{
            if (!this->isSquat()) {
                this->sprite->stopAllActions();
                this->stand();
                return;
            }
            this->squat();
            
        };
        
        
        auto sequence = cocos2d::Sequence::create(cocos2d::CallFunc::create(preFunc), animate, cocos2d::CallFunc::create(func), NULL);
        if(!isSquat())
            sequence->setTag(OP_GPS_ACTION_2_STAND_PUNCH2);
        else
            sequence->setTag(OP_GPS_ACTION_2_SQUAT_PUNCH2);
        this->sprite->stopAllActions();
        this->sprite->runAction(sequence);
    
}


bool Fighter::isStand()
{
    if(this->sprite->getActionByTag(OP_GPS_ACTION_1_STAND))
        return true;
    return false;
}

bool Fighter::isSquat()
{
    if (   this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_DOWN)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_PUNCH1)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_PUNCH2)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_KICK1)
        || this->sprite->getActionByTag(OP_GPS_ACTION_2_SQUAT_KICK2)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_MOVEBACK)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_MOVEFORWARD)
        )
        return true;
    
    return false;
}

bool Fighter::isActionStoppable()
{
    if (
           this->sprite->getActionByTag(OP_GPS_ACTION_1_STAND)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT)
        || this->sprite->getActionByTag(OP_GPS_ACTION_1_SQUAT_DOWN)
        
        || this->sprite->getActionByTag(ANIMATION_ACTION_1_STAND_MOVEBACK)
        || this->sprite->getActionByTag(ANIMATION_ACTION_1_STAND_MOVEFORWARD)
        || this->sprite->getActionByTag(ANIMATION_ACTION_1_SQUAT_MOVEBACK)
        || this->sprite->getActionByTag(ANIMATION_ACTION_1_SQUAT_MOVEFORWARD)
        
        )
        return true;
    return false;
}


bool Fighter::isHit()
{
    auto px = getPosition().x;
    auto ox = opponent->getPosition().x;
    auto pw = getSprite()->getBoundingBox().size.width / 2;// * getSprite()->getScaleX() / 2;
    auto ow = opponent->getSprite()->getBoundingBox().size.width / 2;// * opponent->getSprite()->getScaleX() / 2;
    if (isLeft) {
        if ((px + pw) > (ox - ow))
        {
            return true;
        }
        return false;
    }
    else
    {
        if ((px - pw) < (ox + ow))
        {
            return true;
        }
        return false;
    }
}


void Fighter::setHealthPercentage(float p)
{
    if (p <= 0) {
        this->die();
        opponent->win();
    }
    isHealthChanged = true;
    this->health->setPercent(p);
}

float Fighter::getHealthPercentage()
{
    return this->gethealth()->getPercent();
}


