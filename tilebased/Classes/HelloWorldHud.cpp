#include "HelloWorldHud.h"
#include "HelloWorldScene.h"

USING_NS_CC;

bool HelloWorldHud::init()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_label = CCLabelTTF::create(
                                "0", "Verdana-Bold", 18.0,
                                CCSizeMake(50, 20), kCCTextAlignmentRight);
	_label->setColor(ccc3(0, 0, 0));
	
	int margin = 10;
	
	_label->setPosition(ccp(winSize.width - (_label->getContentSize().width / 2) - margin, 
							_label->getContentSize().height / 2 + margin));
	
	this->addChild(_label);
	
    CCMenuItem *on = CCMenuItemImage::create("projectile-button-on.png", "projectile-button-on.png", NULL, NULL);
	CCMenuItem *off = CCMenuItemImage::create("projectile-button-off.png", "projectile-button-off.png", NULL, NULL);
	
	CCMenuItemToggle *toggleItem = CCMenuItemToggle::createWithTarget(this, menu_selector(HelloWorldHud::projectileButtonTapped), off, on, NULL);
	
	CCMenu *toggleMenu = CCMenu::create(toggleItem, NULL);
	toggleMenu->setPosition(ccp(100, 32));
	this->addChild(toggleMenu);
	return true;
}

void HelloWorldHud::numCollectedChanged(int numCollected)
{
	char *str = (char *)malloc(sizeof(str) * 10);
	sprintf(str, "%d", numCollected);
	_label->setString(str);
	free(str);
}

void HelloWorldHud::projectileButtonTapped(CCObject *sender)
{
	if (_gameLayer->getMode() == 0)
	{
		_gameLayer->setMode(1);
	}
	else {
		_gameLayer->setMode(0);
	}
    
}