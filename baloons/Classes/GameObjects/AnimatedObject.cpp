#include "AnimatedObject.h"

USING_NS_CC;

AnimatedObject::AnimatedObject():
		__pAnimationFrames(NULL),
		__pSpriteSheet(NULL),
		__pSprite(NULL),
		__pAnimationAction(NULL)
{
}

/**
 * This initializing method should be used if game object has animation frames.
 * It loads a new sprite sheet for the game object, if it isn't already loaded.
 * Next it creates animation frames using the sprite sheet.
 * After that, it creates a new sprite and adds it as a child of the sprite sheet.
 *
 * @param textureFileName Name of a file containing game object textures.
 * @param frameNamePrefix Frames are described in plist file in "prefixNumber" format, e.g. "balloon2.png".
 * 						  This param describes the prefix, e.g. "balloon".
 * @param frameCount	  The number of animation frames.
 * @param delay			  Time in seconds between animation frames.
 * @param layer			  Layer that will hold the sprite sheet.
 * @param spriteSheetTag  Tag describing the sprite sheet.
 */
void AnimatedObject::init(
		CCString textureFileName,
		CCString frameNamePrefix,
		int frameCount,
		float delay,
		CCLayer* layer,
		AnimTag spriteSheetTag)
{
	//Get the sprite sheet.
	__pSpriteSheet = (CCSpriteBatchNode*)layer->getChildByTag(spriteSheetTag);
	if(__pSpriteSheet == NULL)
	{
		//Create a new sprite sheet if it dosen't exit.
		const char* textureFileNameC = textureFileName.getCString();
		__pSpriteSheet = CCSpriteBatchNode::create(textureFileNameC);
		__pSpriteSheet->setTag(spriteSheetTag);
		layer->addChild(__pSpriteSheet, 1);
	}

	//Get animation frames.
	__pAnimationFrames = new CCArray();
	for(int i = 1; i <= frameCount; ++i)
	{
        CCString f = "%s%d.png";
		CCString frameName = "";
        frameName.initWithFormat(f.getCString(), frameNamePrefix.getCString(), i);
		const char* frameNameC = (const char*)frameName.getCString();
		CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameNameC);
		__pAnimationFrames->addObject(spriteFrame);
//		__pAnimationFrames->addObject(frameName);

	}

	//Create animation
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(__pAnimationFrames, delay);
	__pAnimationAction = CCRepeatForever::create(CCAnimate::create(animation));

	CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCString f = "%s1.png";
    CCString frameName = "";
    frameName.initWithFormat(f.getCString(), frameNamePrefix.getCString());
	//Create game object sprite and add it to the sprite sheet.
	const char* frameNameC = frameName.getCString();
	__pSprite = CCSprite::createWithSpriteFrameName(frameNameC);
	__pSprite->setPosition(ccp(size.width / 2, size.height / 2));
	__pSprite->runAction(__pAnimationAction);
	__pSpriteSheet->addChild(__pSprite);
}

/**
 * This initializing method should be used if game object dosen't have animation frames.
 * It loads a new sprite sheet for the game object, if it isn't already loaded.
 * After that, it creates a new sprite and adds it as a child of the sprite sheet.
 *
 * @param textureFileName Name of a file containing game object textures.
 * @param layer			  Layer that will hold the sprite sheet.
 * @param spriteSheetTag  Tag describing the sprite sheet.
 */
void
AnimatedObject::init(
		CCString textureFileName,
		CCLayer* layer,
		AnimTag spriteSheetTag)
{
	//Get the sprite sheet.
	__pSpriteSheet = (CCSpriteBatchNode*)layer->getChildByTag(spriteSheetTag);
	if(__pSpriteSheet == NULL)
	{
		//Create a new sprite sheet if it dosen't exit.
//		const char* textureFileNameC = (const char*)StringUtil::StringToUtf8N(textureFileName)->GetPointer();
		__pSpriteSheet = CCSpriteBatchNode::create(textureFileName.getCString());
		__pSpriteSheet->setTag(spriteSheetTag);
		layer->addChild(__pSpriteSheet, 1);
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	//Create game object sprite and add it to the sprite sheet.
//	const char* textureFileNameC = (const char*)StringUtil::StringToUtf8N(textureFileName)->GetPointer();
	__pSprite = CCSprite::create(textureFileName.getCString());
	__pSprite->setPosition(ccp(size.width / 2, size.height / 2));
	__pSpriteSheet->addChild(__pSprite);
}

/**
 * Sets game objects sprite position.
 * @param pos The new position.
 */
void
AnimatedObject::setPosition(const CCPoint& pos)
{
	__pSprite->setPosition(pos);
}

/**
 * @return game object sprite size.
 */
const CCSize&
AnimatedObject::getSize()
{
	return __pSprite->boundingBox().size;
}

/**
 * Stop animation and remove the sprite.
 */
void
AnimatedObject::destroy()
{
	__pSprite->stopAllActions();
	__pSpriteSheet->removeChild(__pSprite, true);
}

/**
 * @return the sprite rect.
 */
CCRect
AnimatedObject::getRect()
{
	//The rectangle width is reduced by 15 because textures have blank space around the object.
	return CCRectMake(__pSprite->getPosition().x - __pSprite->getContentSize().width / 2 + 15,
					  __pSprite->getPosition().y - __pSprite->getContentSize().height / 2,
					  __pSprite->getContentSize().width - 15,
					  __pSprite->getContentSize().height);
}
