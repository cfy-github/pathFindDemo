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

Point TileMapScene::position2TileCoord(Point p) {
    auto x=static_cast<int>(p.x/_tileMap->getTileSize().width);
    auto y=static_cast<int>((_tileMap->getTileSize().height*_tileMap->getMapSize().height-p.y)/_tileMap->getTileSize().height);
    return Point(x,y);
}

void TileMapScene::checkLongPress(float t) {
    //CCLOG("In check\n");
    this->unschedule(schedule_selector(TileMapScene::checkLongPress));
    
    if((isStartTouched || isGoalTouched) && !isTouchMoved) {
        if(isStartTouched) {
           // CCLOG("check long start\n");
            _start->setScale(2.0f);
            _start->setOpacity(200);
            isStartLongPressed=true;
        }
        
        if(isGoalTouched) {
           // CCLOG("check long goal\n");
            _goal->setScale(2.0f);
            _goal->setOpacity(200);
            isGoalLongPressed=true;
        }
    }
    
}

bool TileMapScene::onTouchBegan(Touch* t, Event* e) {
   // CCLOG("Touch Begin\n");
    Point loc = t->getLocationInView();
    loc=Director::getInstance()->convertToGL(loc);
    loc=convertToNodeSpace(loc);
    //CCLOG("%f, %f\n", loc.x, loc.y);
    
    auto bbox=_start->getBoundingBox();
    //CCLOG("%f, %f, %f, %f\n", bbox.getMinX(), bbox.getMinY(),bbox.getMaxX(), bbox.getMaxY());
    
    if(_start->getBoundingBox().containsPoint(loc)) {
        //Click the start point
       // CCLOG("Start Touched\n");
        isStartTouched=true;
        this->schedule(schedule_selector(TileMapScene::checkLongPress), 0.8f);
    }
    
    if(_goal->getBoundingBox().containsPoint(loc)) {
        //Click the goal point
        isGoalTouched=true;
        this->schedule(schedule_selector(TileMapScene::checkLongPress), 1.2f);
    }
    
    //Todo other
    return true;
}

void TileMapScene::onTouchMoved(Touch* t, Event* e) {
    //CCLOG("In Touch Moved\n");
    Point deltaPoint=t->getDelta();
    
    auto tLoc=t->getLocationInView();
    tLoc=Director::getInstance()->convertToGL(tLoc);
    tLoc=convertToNodeSpace(tLoc);
    
    if(fabs(deltaPoint.x)>1 || fabs(deltaPoint.y)>1) {
        isTouchMoved=true;
        
        //Dragging start sprite
        if(isStartLongPressed) {
           // CCLOG("Start Dragging\n");
            auto originTileCoord=position2TileCoord(_tileMap->convertToNodeSpace(_start->getPosition()));
            auto loc1=_tileMap->convertToNodeSpace(_start->getPosition());
            auto loc2=_start->getPosition();
            
            auto currTileCoord=position2TileCoord(tLoc);
            //CCLOG("%f, %f,,,%f, %f----%f,%f\n", loc1.x, loc1.y, loc2.x, loc2.y, tLoc.x, tLoc.y);
            if(originTileCoord!=currTileCoord) {
                //Move start sprite;
                _start->setPosition(tileCoord2Center(currTileCoord));
            }
        }
        
        //Dragging goal sprite
        if(isGoalLongPressed) {
            auto originTileCoord=position2TileCoord(_tileMap->convertToNodeSpace(_goal->getPosition()));
            auto currTileCoord=position2TileCoord(tLoc);
            if(originTileCoord!=currTileCoord) {
                //Move goal sprite;
                //CCLOG("To another tile\n");
                _goal->setPosition(tileCoord2Center(currTileCoord));
            }
        }
        
    }
    
    
    //TODO
}

void TileMapScene::onTouchCancelled(Touch* t, Event* e) {
    if(isStartTouched) {
        isStartTouched=false;
        if(isStartLongPressed){
            isStartLongPressed=false;
            _start->setScale(1.0f);
            _start->setOpacity(255);
        }
    }
    
    if(isGoalTouched) {
        isGoalTouched=false;
        if(isGoalLongPressed){
            isGoalLongPressed=false;
            _goal->setScale(1.0f);
            _goal->setOpacity(255);
        }
    }
    
    //TODO
    isTouchMoved=false;
}

void TileMapScene::onTouchEnded(Touch* t, Event* e) {
    if(isStartTouched) {
        isStartTouched=false;
        if(isStartLongPressed){
            isStartLongPressed=false;
            _start->setScale(1.0f);
            _start->setOpacity(255);
        }
    }
    
    if(isGoalTouched) {
        isGoalTouched=false;
        if(isGoalLongPressed){
            isGoalLongPressed=false;
            _goal->setScale(1.0f);
            _goal->setOpacity(255);
        }
    }
    
    //TODO
    isTouchMoved=false;
}

void TileMapScene::initTouchEvent() {
    auto listener=EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(TileMapScene::onTouchBegan, this);
    
    listener->onTouchMoved = CC_CALLBACK_2(TileMapScene::onTouchMoved, this);
    
    listener->onTouchEnded = CC_CALLBACK_2(TileMapScene::onTouchEnded, this);
    
    listener->onTouchCancelled = CC_CALLBACK_2(TileMapScene::onTouchCancelled, this);
    
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void TileMapScene::initImageCache() {
    _startTileTexture=Director::getInstance()->getTextureCache()->addImage("start.png");
    _goalTileTexture=Director::getInstance()->getTextureCache()->addImage("goal.png");
    _defaultTileTexture=Director::getInstance()->getTextureCache()->addImage("tile.png");
}

bool TileMapScene::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) return false;
    
    
    initImageCache();
    //Load tile map
    // create a TMX map
    _tileMap=TMXTiledMap::create("pathFind.tmx");
    addChild(_tileMap, 0, 66);
    _background=_tileMap->getLayer("Background");
    auto bckPos=_background->getPosition();
    
    
    auto objGroup=_tileMap->getObjectGroup("objects");
    if(objGroup==NULL) {
        CCLOG("No object layer found!\n");
        return false;
    }
    
    auto initObj=objGroup->getObject("initPoint");
    auto goalObj=objGroup->getObject("goalPoint");
    auto dftObj=objGroup->getObject("defaultPoint");
    
    auto initPoint=Point(initObj["x"].asInt(), initObj["y"].asInt());
    auto goalPoint=Point(goalObj["x"].asInt(), initObj["y"].asInt());
    auto dftPoint=Point(dftObj["x"].asInt(), initObj["y"].asInt());
    
    
    auto initTileCoord=position2TileCoord(initPoint);
    auto goalTileCoord=position2TileCoord(goalPoint);
    auto dftTileCoord=position2TileCoord(dftPoint);
    
    //Init start sprite;
    _start=Sprite::createWithTexture(_startTileTexture);
    _start->setAnchorPoint(Vec2(0.5,0.5));
    auto startCenter=tileCoord2Center(initTileCoord);
    _start->setPosition(startCenter);
    addChild(_start, 10, 70);
    
    //Init goal sprite
    _goal=Sprite::createWithTexture(_goalTileTexture);
    _goal->setAnchorPoint(Vec2(0.5,0.5));
    auto goalCenter=tileCoord2Center(goalTileCoord);
    _goal->setPosition(goalCenter);
    addChild(_goal, 10, 80);
    
    initTouchEvent();
    
    setViewPointCetner(startCenter);
    
    return true;
}

Point TileMapScene::tileCoord2Center(Point coord) {
    auto size=_tileMap->getTileSize();
    auto mSize=_tileMap->getMapSize();
    Point ret;
    ret.x=(coord.x+0.5)*size.width;
    ret.y=(mSize.height-coord.y-0.5)*size.height;
    
    return ret;
}

void TileMapScene::setAnchorPoint2Center(Sprite* sp) {
    if(!sp) return;
    sp->setAnchorPoint(Vec2(0.5, 0.5));
    auto size=sp->getContentSize();
    auto pos=sp->getPosition();
    sp->setPosition(pos.x+0.5*size.width, pos.y+0.5*size.height);
}

void TileMapScene::setAnchorPoint2Origin(Sprite* sp) {
    if(!sp) return;
    sp->setAnchorPoint(Vec2::ZERO);
    auto size=sp->getContentSize();
    auto pos=sp->getPosition();
    sp->setPosition(pos.x-0.5*size.width, pos.y-0.5*size.height);
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

