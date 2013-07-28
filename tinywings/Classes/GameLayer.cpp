#include "GameLayer.h"

void GameLayer::createBox2DWorld() {
	
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	
	world = new b2World(gravity);
}

CCScene * GameLayer::scene() {
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

void GameLayer::generateBackground() {
    int textureSize = 512;
	ccColor4F c = ccc4f(140.0f/255.0f,205.0f/255.0f,221.0f/255.0f, 255.0f);
    CCRenderTexture * rt = CCRenderTexture::create(textureSize, textureSize);
    rt->beginWithClear(c.r, c.g, c.b, c.a);
    rt->end();
    
    setBackground(rt->getSprite());
    background->removeFromParentAndCleanup(true);
    
    background->setPosition(ccp(screenW/2,screenH/2));
    ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    background->getTexture()->setTexParameters(&tp);
    
    addChild(background);
    return;
}

bool GameLayer::init() {
    
    CCLayer::init();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    screenW = size.width;
    screenH = size.height;
    
    createBox2DWorld();
    
    generateBackground();
    
    setTerrain(Terrain::terrainWithWorld(world));
    
    addChild(getTerrain());
    
    setHero(Hero::heroWithWorld(world));
    getTerrain()->addChild(getHero());
    
    setTouchEnabled(true);
    
    tapDown = false;
    
    scheduleUpdate();
    return true;
}

GameLayer::~GameLayer() {
    delete world;
    world = NULL;
    
    getBackground()->release();
    getTerrain()->release();
    
    setBackground(NULL);
    setTerrain(NULL);
}

void GameLayer::registerWithTouchDispatcher() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    tapDown = true;
    return true;
}

void GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    tapDown = false;
}

void GameLayer::update(float dt) {
    if (tapDown) {
        if ( ! hero->getAwake()) {
            hero->wake();
            tapDown = false;
        } else {
            hero->dive();
        }
    }
    
    hero->limitVelocity();

    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    world->Step(dt, velocityIterations, positionIterations);
    
    hero->updateNodePosition();
    
    float scale = (screenH*4/5) / hero->getPosition().y;
    if (scale > 1) scale = 1;
    terrain->setScale(scale);
    terrain->setOffsetX(hero->getPosition().x);
    
    CCSize size = getBackground()->getTextureRect().size;
    getBackground()->setTextureRect(CCRectMake(getTerrain()->getOffsetX()*0.2f, 0, size.width, size.height));
}
