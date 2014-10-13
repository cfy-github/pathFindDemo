//
//  TileMapScene.cpp
//  pathFindDemo
//
//  Created by fychen on 10/12/14.
//
//

#include "TileMapScene.h"

USING_NS_CC;

Scene* TileMapScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = TileMapScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

void TileMapScene::initTouchEvent() {
    auto listener=EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [](Touch *t, Event* e) {
        auto target = static_cast<Sprite*>(e->getCurrentTarget());
        
        //Get the position of the current point relative to the button
        Point locationInNode = target->convertToNodeSpace(t->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        //TODO
        return true;
    };
    
    //TODO
    listener->onTouchMoved = [](Touch* t, Event* e) {};
    listener->onTouchEnded = [](Touch* t, Event* e) {};
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);//This?
}

bool TileMapScene::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) return false;
    
    //Load tile map
    // create a TMX map
    _tileMap=TMXTiledMap::create("pathFind.tmx");
    addChild(_tileMap, 1, 66);
    _background=_tileMap->getLayer("Background");
    
    auto objGroup=_tileMap->getObjectGroup("objects");
    if(objGroup==NULL) {
        CCLOG("No object layer found!\n");
        return false;
    }
    
    auto initPoint=objGroup->getObject("initPoint");
    
    int x=initPoint["x"].asInt();
    
    int y=initPoint["y"].asInt();
    
    setViewPointCetner(Point(x,y));
    
    
    //_meta=_tileMap->layerNamed("Meta");
    
    return true;
}

void TileMapScene::setViewPointCetner(Point p) {
    auto visibleSize=Director::getInstance()->getVisibleSize();
    
    float x=MAX(p.x, visibleSize.width*0.5);
    float y=MAX(p.y, visibleSize.height*0.5);
    
    x=MIN(x, _tileMap->getMapSize().width*_tileMap->getTileSize().width-visibleSize.width*0.5);
    y=MIN(y, _tileMap->getMapSize().height*_tileMap->getTileSize().width-visibleSize.height*0.5);
    
    auto currP=Point(visibleSize.width*0.5, visibleSize.height*0.5);
    auto actualP=Point(x,y);
    currP.subtract(actualP);
    
    setPosition(currP);
}

