//
//  Multiplayer.h
//  KOF
//
//  Created by Dongtao Yu on 26/08/2015.
//
//

#ifndef __KOF__Multiplayer__
#define __KOF__Multiplayer__


#include <stdio.h>
#include "appwarp.h"
#include "cocos2d.h"
#include "Definitions.h"
#include "ChooseRoomScene.h"


class Multiplayer : public AppWarp::ConnectionRequestListener, public AppWarp::ZoneRequestListener, public AppWarp::RoomRequestListener
{
    
public:
    
    static Multiplayer* getInstance();
    
    static void initialize(std::string);
    
//    void fetchRooms(ChooseRoomScene* scene);
    
    void createRoom(std::map<std::string, std::string>);
    
    bool isConnected();
    
    void recoverConnection();
    
private:
    
    
    std::string username;
    
    static Multiplayer* _instance;
    
    bool connected;
    
    // Private constructor
    Multiplayer(std::string username);
    
    
    
    // ConnectionRequestListener
    void onConnectDone(int, int);
    
    // ZoneRequestListener
    void onGetAllRoomsDone(AppWarp::liveresult);
    
    void onCreateRoomDone(AppWarp::room);
    
    // RoomRequestListner
//    void onGetLiveRoomInfoDone(AppWarp::liveroom);
    
    
};

#endif /* defined(__KOF__Multiplayer__) */
