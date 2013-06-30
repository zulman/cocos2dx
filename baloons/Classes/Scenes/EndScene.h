#ifndef ENDSCENE_H_
#define ENDSCENE_H_

#include "cocos2d.h"

/**
 * This scene contains restart and exit buttons.
 * It inherits after CCLayer because it has only one layer.
 */
class EndScene :
	public cocos2d::CCLayer
{
public:
	/**
	 * This tag helps to find the layer when scene changes.
	 */
	static const int endLayerTag = 1;

	EndScene();
	bool init();
	static cocos2d::CCScene* scene();
	void menuCloseCallback(CCObject* pSender);
	void menuRestartCallback(CCObject* pSender);
	void setFinalScore(int finalScore);
	void update(float dt);
	void onEnter();

	// implement the "static node()" method manually
	CREATE_FUNC(EndScene);

private:
	/**
	 * This label will show the final score.
	 */
	cocos2d::CCLabelTTF* 	__pFinalScoreLabel;

	/**
	 * First element of a moving background.
	 */
	cocos2d::CCSprite* 		__pBg1;

	/**
	 * Second element of the moving background.
	 */
	cocos2d::CCSprite* 		__pBg2;

	/**
	 * Third element of the moving background.
	 */
	cocos2d::CCSprite* 		__pBg3;

	void updateBgPosition(cocos2d::CCSprite* pBg);
};

#endif /* ENDSCENE_H_ */
