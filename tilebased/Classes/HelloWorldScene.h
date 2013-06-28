#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "HelloWorldHud.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    ~HelloWorld();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	void setViewpointCenter(cocos2d::CCPoint point);
	void setPlayerPosition(cocos2d::CCPoint point);
	cocos2d::CCPoint tileCoordForPosition(cocos2d::CCPoint position);
	
    void addEnemyAt(int x, int y);
	void enemyMoveFinished(cocos2d::CCSprite *enemy);
	void animateEnemy(cocos2d::CCSprite *enemy);
	void projectileMoveFinished(cocos2d::CCSprite *sprite);
	void testCollisions(float dt);
	void win();
	void lose();
    
	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);
    
    CC_SYNTHESIZE(cocos2d::CCTMXTiledMap*, _tileMap, TileMap);
    CC_SYNTHESIZE(cocos2d::CCTMXLayer*, _background, Background);
	CC_SYNTHESIZE(cocos2d::CCTMXLayer*, _foreground, Foreground);
    CC_SYNTHESIZE(cocos2d::CCTMXLayer*, _meta, Meta);
	CC_SYNTHESIZE(cocos2d::CCSprite *, _player, Player);
	CC_SYNTHESIZE(HelloWorldHud *, _hud, Hud);
	CC_SYNTHESIZE(int, _numCollected, NumCollected);
    
    CC_SYNTHESIZE(int, _mode, Mode);
	CC_SYNTHESIZE(cocos2d::CCArray*, _enemies, Enemies);
	CC_SYNTHESIZE(cocos2d::CCArray*, _projectiles, Projectiles);
};

#endif // __HELLOWORLD_SCENE_H__
