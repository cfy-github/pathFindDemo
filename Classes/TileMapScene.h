//
//  TileMapScene.h
//  pathFindDemo
//
//  Created by fychen on 10/12/14.
//
//

#ifndef __pathFindDemo__TileMapScene__
#define __pathFindDemo__TileMapScene__
#include "cocos2d.h"
#include "AStarFinder.h"

#include <iostream>

USING_NS_CC;

class TileMapScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void menuGoCallback(Ref* pSender);
    
    virtual bool onTouchBegan(Touch* t, Event* e);
    virtual void onTouchMoved(Touch* t, Event* e);
    virtual void onTouchEnded(Touch* t, Event* e);
    virtual void onTouchCancelled(Touch* t, Event* e);
    
    //Start callback
    //void menuStartCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TileMapScene);
    
    static inline long millisecondNow() {
        struct timeval now;
        gettimeofday(&now, NULL);
        return (now.tv_sec * 1000 + now.tv_usec / 1000);
    }
    
    void ccLogVec2(Vec2 v);
    
private:
    //Point tileCoordForPosition(Point pos);
    
    void initTouchEvent();
    
    void setViewPointCetner(Point p);
    
    Point position2TileCoord(Point pos);
    
    Point tileCoord2Center(Point p);
    
    void checkLongPress(float t);
    
    void setAnchorPoint2Center(Sprite* sp);
    void setAnchorPoint2Origin(Sprite* sp);
    
    void initImageCache();
    
    void initTilesType();
    
    void switchTileTypes();
    
    void runScaleAction(Sprite* ps);
    
    void updateMyTiles();
    
    Sprite* getTileAt(Point p);
    
    //void touchMoved
    
private:
    EventListenerTouchOneByOne* listener;
    
    AStarFinder* _finderAstar;
    
    TMXTiledMap* _tileMap;
    TMXLayer* _background;
    TMXLayer* _foreground;
    //Sprite* _player;
    
    Sprite* _start;
    Sprite* _goal;
    Sprite* _currDft;
    
    int _idStart;
    int _idGoal;
    
    Texture2D* _defaultTileTexture;
    Texture2D* _startTileTexture;
    Texture2D* _goalTileTexture;
    Texture2D* _wallTileTexture;
    Texture2D* _greenTileTexture;
    Texture2D* _blueTileTexture;
    
    std::vector<std::vector<Sprite*> > vTileSprites;
    std::vector<std::vector<Sprite*> > vTilesConfig;
    
    //Touch
    bool isTouchMoved=false;
    bool isStartLongPressed=false;
    bool isGoalLongPressed=false;
    bool isDftLongPressed=false;
    
    
    bool isStartTouched=false;
    bool isGoalTouched=false;
    bool isDftTouched=false;
    
    
    Point _blueCoord;
    Point _blackCoord;
    Point _dftCoord;
    
    Point _currCoord;
    
    int moveAlterFlag=-1;
};

#endif /* defined(__pathFindDemo__TileMapScene__) */
