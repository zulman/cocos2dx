#ifndef HELLOWORLD_SCENE_H_
#define HELLOWORLD_SCENE_H_

#include "cocos2d.h"
#include "../GameObjects/Balloon.h"
#include "EnemyGenerator.h"

/**
 * This scene contains game objects and score.
 * It inherits after CCLayer because it has only one layer.
 */
class MainScene :
	public cocos2d::CCLayer
{
public:
	MainScene();
	bool init();
	static cocos2d::CCScene* scene();
	void menuCloseCallback(CCObject* pSender);
	void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
	void onEnter();
	void update(float dt);

	// implement the "static node()" method manually
	CREATE_FUNC(MainScene);

private:
	/**
	 * Player controlled character.
	 */
	Balloon* 					  	__pBalloon;

	/**
	 * Generates birds and planes.
	 */
	EnemyGenerator*					__pEnemyGenerator;

	/**
	 * Displays current score.
	 */
	cocos2d::CCLabelTTF*			__pScoreLabel;

	/**
	 * Current score value.
	 */
	int								__score;

	/**
	 * First element of a moving background.
	 */
	cocos2d::CCSprite* 				__pBg1;

	/**
	 * Second element of a moving background.
	 */
	cocos2d::CCSprite* 				__pBg2;

	/**
	 * Third element of a moving background.
	 */
	cocos2d::CCSprite* 				__pBg3;

	void updateEnemies(cocos2d::CCRect balloonRect, cocos2d::CCArray* pEnemies);
	void updateScore();
	void updateBgPosition(cocos2d::CCSprite* pBg);
	void updateBackground();
};

#endif // HELLOWORLD_SCENE_H_
