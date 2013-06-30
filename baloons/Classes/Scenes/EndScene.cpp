#include "EndScene.h"
#include "MainScene.h"

USING_NS_CC;

EndScene::EndScene() :
		__pFinalScoreLabel(NULL),
		__pBg1(NULL),
		__pBg2(NULL),
		__pBg3(NULL)
{
}

/**
 * Call this method to create the EndScene.
 */
CCScene*
EndScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	EndScene *layer = EndScene::create();
	layer->setTag(endLayerTag);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

/**
 * This method is called by node() method defined in LAYER_NODE_FUNC macro.
 */
bool
EndScene::init()
{
	// Init super first.
	if ( !CCLayer::init() )
	{
		return false;
	}

	// Ask director the window size.
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// Create final score label.
	__pFinalScoreLabel = CCLabelTTF::create("Final score: 0!", "Arial", 50);
	__pFinalScoreLabel->setPosition( ccp(winSize.width / 2, winSize.height - 20) );
	this->addChild(__pFinalScoreLabel, 1);

	// Create restart button.
	CCMenuItemFont* pRestartButton = CCMenuItemFont::create(
										"Restart",
										this,
										menu_selector(EndScene::menuRestartCallback));
	pRestartButton->setFontSizeObj(50);
	pRestartButton->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// Create close button.
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(EndScene::menuCloseCallback) );
	pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pRestartButton, pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	// Create the background sprites.
	__pBg1 = CCSprite::createWithSpriteFrameName("bg.png");
	//Change the anchor point for convenience.
	__pBg1->setAnchorPoint(ccp(0, 0));
	__pBg1->setPosition(ccp(0, 0));

	float bgWidth = __pBg1->getContentSize().width;
	__pBg2 = CCSprite::createWithSpriteFrameName("bg.png");
	__pBg2->setAnchorPoint(ccp(0, 0));
	//Position the second sprite directly after the first.
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
EndScene::setFinalScore(int finalScore)
{
	const int labelLength = 100;
	char finalScoreLabelText[labelLength];
	snprintf(finalScoreLabelText, labelLength, "Final score: %d!", finalScore);
	__pFinalScoreLabel->setString(finalScoreLabelText);
}

void
EndScene::menuRestartCallback(CCObject* pSender)
{
	CCScene *pScene = MainScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void
EndScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

void
EndScene::onEnter()
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
EndScene::update(float dt)
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
void
EndScene::updateBgPosition(CCSprite* pBg)
{
	if(pBg)
	{
		CCPoint bgPos = pBg->getPosition();
		bgPos.x -= 1;
		float bgWidth = pBg->getContentSize().width;
		if(bgPos.x < -bgWidth)
		{
			bgPos.x = 2 * bgWidth - 4;
			// To remove a black line between third and first part of the background,
			// after the first part has been moved to the right,
			// the part needs to be detached from the layer and added again.
			// This will move the first part on top of the others.
			// The object needs to be retained as removeChild would delete the object.
			pBg->retain();
			this->removeChild(pBg, false);
			this->addChild(pBg);
		};
		pBg->setPosition(bgPos);
	}
}
