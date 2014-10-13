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

#include <iostream>

USING_NS_CC;

class TileMapScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    //Start callback
    //void menuStartCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TileMapScene);
private:
    Point tileCoordForPosition(Point pos);
    
    void initTouchEvent();
    
    void setViewPointCetner(Point p);
    
private:
    TMXTiledMap* _tileMap;
    TMXLayer* _background;
    Sprite* _player;
    
    TMXLayer* _meta;
};

#endif /* defined(__pathFindDemo__TileMapScene__) */
