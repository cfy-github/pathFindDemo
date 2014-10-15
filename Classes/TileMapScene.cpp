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

void TileMapScene::runScaleAction(Sprite* ps) {
    auto scaleUp=ScaleTo::create(0.2f, 1.2);
    auto scaleDown=ScaleTo::create(0.2f, 1);
    auto seqScale=Sequence::create(scaleUp, scaleDown, NULL);
    ps->runAction(seqScale);
}

void TileMapScene::switchTileTypes() {
    CCLOG("Switch types\n");
    auto ps=vTileSprites[_currCoord.x][_currCoord.y];
    
    if(moveAlterFlag==-1) {//First touch
        if(ps->isVisible()) {
            ps->setVisible(false);
            runScaleAction(_background->getTileAt(_currCoord));
            moveAlterFlag=0; //0<-->false
        } else {
            ps->setVisible(true);
            runScaleAction(ps);
            moveAlterFlag=1; //1<-->true
        }
    } else {
        if(moveAlterFlag==0) {
            bool tmp=ps->isVisible();
            ps->setVisible(false);
            if(tmp) {
                runScaleAction(_background->getTileAt(_currCoord));
            }
        } else {
            bool tmp=ps->isVisible();
            ps->setVisible(true);
            if(!tmp) {
                runScaleAction(ps);
            }
        }
    }
    /*Sprite* ps=_currDft;
    if(!ps) {
        CCLOG("curr default tile is NULL\n");
        return;
    }
    auto coord=position2TileCoord(ps->getPosition());
    auto tileSet=_background->getTileSet();
    
    if(tilesType[ps]==0) {
        CCLOG("dft to wall\n");
        //new...ps
        auto gid=_background->getTileGIDAt(_blackCoord);
        CCLOG("%d\n", gid);
        Rect rect=tileSet->getRectForGID(gid);
        CCLOG("%f, %f, %f, %f\n", rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
        
        auto newPs=Sprite::createWithTexture(_blackNode->getTexture(), Rect(0,0,32,32));
        newPs->setBatchNode(_blackNode);
        newPs->retain();
        newPs->setAnchorPoint(ps->getAnchorPoint());
        newPs->setPosition(ps->getPosition());
        //int z = (int)(coord.x + coord.y * _background->getLayerSize().width);
        //ssize_t indexForZ = _background->a
        _blackNode->addChild(newPs);
        
        tilesType.erase(ps);
        tilesType[newPs]=1;
        vTileSprites[coord.x][coord.y]=newPs;
        newPs->release();
        auto batchNode=ps->getBatchNode();
        batchNode->Node::removeChild(ps, true);
        _currDft=newPs;
    } else {
        CCLOG("wall to dft\n");
        auto gid=_background->getTileGIDAt(_dftCoord);
        CCLOG("%d\n", gid);
        Rect rect=tileSet->getRectForGID(gid);
        CCLOG("%f, %f, %f, %f\n", rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
        
        auto newPs=Sprite::createWithTexture(_dftNode->getTexture(), rect);
        newPs->setBatchNode(_dftNode);
        newPs->retain();
        newPs->setAnchorPoint(ps->getAnchorPoint());
        newPs->setPosition(ps->getPosition());
        _dftNode->Node::addChild(newPs);
        tilesType.erase(ps);
        tilesType[newPs]=0;
        vTileSprites[coord.x][coord.y]=newPs;
        newPs->release();
        auto batchNode=ps->getBatchNode();
        batchNode->Node::removeChild(ps, true);
        _currDft=newPs;
    }*/
}

void TileMapScene::checkLongPress(float t) {
    CCLOG("In check\n");
    this->unschedule(schedule_selector(TileMapScene::checkLongPress));
    
    if((isStartTouched || isGoalTouched || isDftTouched) && !isTouchMoved) {
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
        
        if(isDftTouched) {
            CCLOG("in LP DFT\n");
            switchTileTypes();
            isDftLongPressed=true;
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
        CCLOG("Start Touched\n");
        isStartTouched=true;
        this->schedule(schedule_selector(TileMapScene::checkLongPress), 0.9f);
    }
    else if(_goal->getBoundingBox().containsPoint(loc)) {
        //Click the goal point
        CCLOG("Goal Touched\n");
        isGoalTouched=true;
        this->schedule(schedule_selector(TileMapScene::checkLongPress), 0.9f);
    }
    else {
        CCLOG("Touch DFT\n");
        //Normal tile touched
        isDftTouched=true;
        //auto mLoc=_tileMap->convertToNodeSpace(loc);
        //CCLOG("%f-%f,,,%f-%f\n", loc.x, loc.y, mLoc.x, mLoc.y);
        auto tileCoord=position2TileCoord(loc);
        //CCLOG("%f,%f\n", tileCoord.x, tileCoord.y);
        _currCoord=tileCoord;
        this->schedule(schedule_selector(TileMapScene::checkLongPress), 0.6f);
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
            //TODO: HITS wall
        }
        
        //Dragging goal sprite
        if(isGoalLongPressed) {
            auto originTileCoord=position2TileCoord(_tileMap->convertToNodeSpace(_goal->getPosition()));
            auto currTileCoord=position2TileCoord(tLoc);
            if(originTileCoord!=currTileCoord) {
                //Move goal sprite;
                _goal->setPosition(tileCoord2Center(currTileCoord));
            }
            //todo: hits wall
        }
        
        if(isDftLongPressed) {
            auto originTileCoord=_currCoord;
            //position2TileCoord(_tileMap->convertToNodeSpace(_goal->getPosition()));
            auto currTileCoord=position2TileCoord(tLoc);
            if(originTileCoord!=currTileCoord) {
                //Move DFT sprite;
                CCLOG("DFT: To another tile\n");
                if(currTileCoord==position2TileCoord(_start->getPosition()) || currTileCoord==position2TileCoord(_goal->getPosition())) {
                    //cannot move to start or goal point
                    return;
                }
                //auto ps=getTileAt(currTileCoord);
                //_currDft=ps;
                _currCoord=currTileCoord;
                switchTileTypes();
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
    
    if(isDftTouched) {
        isDftTouched=false;
        if(isDftLongPressed) {
            isDftLongPressed=false;
            //_currCoord
        }
    }
    
    moveAlterFlag=-1;
    
    //TODO
    isTouchMoved=false;
}

void TileMapScene::onTouchEnded(Touch* t, Event* e) {
    onTouchCancelled(t, e);
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
    _wallTileTexture=Director::getInstance()->getTextureCache()->addImage("wall.png");
}

void TileMapScene::initTilesType() {
    auto size=_tileMap->getMapSize();
    for(int i=0;i<size.width;++i) {
        for(int j=0;j<size.height;++j) {
            auto ps=Sprite::createWithTexture(_wallTileTexture);
            ps->setAnchorPoint(Vec2(0.5,0.5));
            auto center=tileCoord2Center(Point(i,j));
            ps->setPosition(center);
            ps->setVisible(false);
            vTileSprites[i][j]=ps;
            //tilesType[ps]=0; //0 for normal type
            addChild(ps, 10);
        }
    }
}

Sprite* TileMapScene::getTileAt(Point p) {
    return vTileSprites[p.x][p.y];
}

bool TileMapScene::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) return false;
    
    
    initImageCache();
    //Load tile map
    // create a TMX map
    _tileMap=TMXTiledMap::create("pathFind.tmx");
    _background=_tileMap->getLayer("Background");
    addChild(_tileMap, 0, 66);
    auto size=_tileMap->getMapSize();
    vTileSprites=std::vector<std::vector<Sprite* > >(size.height, std::vector<Sprite *>(size.width, nullptr));
    initTilesType();
    
    
    auto objGroup=_tileMap->getObjectGroup("objects");
    if(objGroup==NULL) {
        CCLOG("No object layer found!\n");
        return false;
    }
    
    auto initObj=objGroup->getObject("initPoint");
    auto goalObj=objGroup->getObject("goalPoint");
    auto dftObj=objGroup->getObject("defaultPoint");
    auto blueObj=objGroup->getObject("blue");
    auto blackObj=objGroup->getObject("black");
    
    auto initPoint=Point(initObj["x"].asInt(), initObj["y"].asInt());
    auto goalPoint=Point(goalObj["x"].asInt(), goalObj["y"].asInt());
    auto dftPoint=Point(dftObj["x"].asInt(), dftObj["y"].asInt());
    auto bluePoint=Point(blueObj["x"].asInt(), blueObj["y"].asInt());
    auto blackPoint=Point(blackObj["x"].asInt(), blackObj["y"].asInt());
    
    
    auto initTileCoord=position2TileCoord(initPoint);
    auto goalTileCoord=position2TileCoord(goalPoint);
    
    auto dftTileCoord=_dftCoord=position2TileCoord(dftPoint);
    auto blueTileCoord=_blueCoord=position2TileCoord(bluePoint);
    auto blackTileCoord=_blackCoord=position2TileCoord(blackPoint);
    
    //Init blue batch node
    ///_blueNode=_background->getTileAt(blueTileCoord)->getBatchNode();
    //_blackNode=_background->getTileAt(blackTileCoord)->getBatchNode();
    //_dftNode=_background->getTileAt(dftTileCoord)->getBatchNode();
    
    //ccLogVec2(blueTileCoord);
    //ccLogVec2(blackTileCoord);
    //ccLogVec2(dftTileCoord);
    
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

void TileMapScene::ccLogVec2(Vec2 v) {
    CCLOG("(%f, %f)\n", v.x, v.y);
}

