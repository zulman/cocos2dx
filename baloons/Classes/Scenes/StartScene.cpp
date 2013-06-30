#include "StartScene.h"
#include "MainScene.h"

USING_NS_CC;

StartScene::StartScene():
		__pBg1(NULL),
		__pBg2(NULL),
		__pBg3(NULL)
{
}

CCScene*
StartScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	StartScene *layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

/**
 * This method is called by node() method defined in LAYER_NODE_FUNC macro.
 */
bool
StartScene::init()
{
	// Init super first.
	if ( !CCLayer::init() )
	{
		return false;
	}

	// Create start button.
	CCMenuItemFont* pStartButton = CCMenuItemFont::create(
										"Start",
										this,
										menu_selector(StartScene::menuStartCallback));

	pStartButton->setFontSizeObj(50);
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
	pStartButton->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pStartButton, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	// Add background to cache.
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("background.plist", "bg_duplicate.png");

	// Create the background sprites.
	__pBg1 = CCSprite::createWithSpriteFrameName("bg.png");
	//Change the anchor point for convenience.
	__pBg1->setAnchorPoint(ccp(0, 0));
	__pBg1->setPosition(ccp(0, 0));

	float bgWidth = __pBg1->getContentSize().width;
	__pBg2 = CCSprite::createWithSpriteFrameName("bg.png");
	__pBg2->setAnchorPoint(ccp(0, 0));
	//Position the second sprite directly after the first (from left to right).
	__pBg2->setPosition(ccp(bgWidth - 1, 0));

	__pBg3 = CCSprite::createWithSpriteFrameName("bg.png");
	__pBg3->setAnchorPoint(ccp(0, 0));
	//Position the third sprite directly after the second.
	__pBg3->setPosition(ccp(2 * (bgWidth - 1), 0));

	this->addChild(__pBg1, 0);
	this->addChild(__pBg2, 0);
	this->addChild(__pBg3, 0);

	return true;
}

void
StartScene::menuStartCallback(CCObject* pSender)
{
	CCScene *pScene = MainScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void
StartScene::onEnter()
{
	// Remember to call the parent method.
	CCLayer::onEnter();
	// This method will schedule updates with every frame. This will call update() callback method.
	scheduleUpdate();
}

/**
 * Update the background.
 */
void
StartScene::update(float dt)
{
	updateBgPosition(__pBg1);
	updateBgPosition(__pBg2);
	updateBgPosition(__pBg3);
}

/**
 * Move the background sprites.
 * If a part is moved off the screen to left,
 * it is moved back off the screen to the right.
 *
 * @param pBg The background sprite that should be moved.
 */
void StartScene::updateBgPosition(CCSprite* bg)
{
	if(bg)
	{
		CCPoint bgPos = bg->getPosition();
		bgPos.x -= 1;
		float bgWidth = bg->getContentSize().width;
		if(bgPos.x < -bgWidth)
		{
			bgPos.x = 2 * bgWidth - 4;
			// To remove a black line between third and first part of the background,
			// after the first part has been moved to the right,
			// the part needs to be detached from the layer and added again.
			// This will move the first part on top of the others.
			// The object needs to be retained as removeChild would delete the object.
			bg->retain();
			this->removeChild(bg, false);
			this->addChild(bg);
		};
		bg->setPosition(bgPos);
	}
}
