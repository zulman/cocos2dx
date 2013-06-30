#include "cocos2d.h"
#include "MainScene.h"

class GameScene : public MainScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	void didAccelerate(CCAcceleration* pAccelerationValue);

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

	virtual void registerWithTouchDispatcher(void);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	bool ccTouchBegan(CCTouch* touch, CCEvent* event);

	CCPoint bird_pos;
	ccVertex2F bird_vel;
	ccVertex2F bird_acc;	

	float currentPlatformY;
	int currentPlatformTag;
	float currentMaxPlatformStep;
	int currentBonusPlatformIndex;
	int currentBonusType;
	int platformCount;
	
	bool gameSuspended;
	bool birdLookingRight;
	
	int score;

	void	initPlatforms(void);
	void	initPlatform(void);
	void	startGame(void);
	void	resetPlatforms(void);
	void	resetPlatform(void);
	void	resetBird(void);
	void	resetBonus(void);
	void	step(float dt);
	void	jump(void);
	void	showHighscores(void);

};

