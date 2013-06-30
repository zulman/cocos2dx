#ifndef STARTSCENE_H_
#define STARTSCENE_H_

#include "cocos2d.h"

/**
 * This scene contains start button.
 * It inherits after CCLayer because it has only one layer.
 */
class StartScene :
	public cocos2d::CCLayer
{
public:
	StartScene();
	bool init();
	static cocos2d::CCScene* scene();
	void menuStartCallback(CCObject* pSender);
	void update(float dt);
	void onEnter();

	// implement the "static node()" method manually
	CREATE_FUNC(StartScene);

private:
	cocos2d::CCSprite* __pBg1;
	cocos2d::CCSprite* __pBg2;
	cocos2d::CCSprite* __pBg3;

	void updateBgPosition(cocos2d::CCSprite* pBg);
};

#endif /* STARTSCENE_H_ */
