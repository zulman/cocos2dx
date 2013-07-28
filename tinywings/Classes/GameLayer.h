#include "Terrain.h"
#include "cocos2d.h"
#include "Hero.h"
#include "Box2D.h"

using namespace cocos2d;

class GameLayer : public CCLayer {
    
public:
    
    float screenW;
	float screenH;
    
    b2World *world;    
    bool tapDown;
    CC_SYNTHESIZE_RETAIN(Hero *, hero, Hero);
    
    CC_SYNTHESIZE_RETAIN(CCSprite *, background, Background);
    CC_SYNTHESIZE_RETAIN(Terrain *, terrain, Terrain);
    CREATE_FUNC(GameLayer);
    static CCScene * scene();
    
    bool init();
    
    ~GameLayer();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void generateBackground();
    
    void update(float dt);
    
    void createBox2DWorld();
};
