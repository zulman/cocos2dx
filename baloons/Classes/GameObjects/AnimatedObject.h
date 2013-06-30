#ifndef ANIMATEDOBJECT_H_
#define ANIMATEDOBJECT_H_

#include "cocos2d.h"

/**
 * Base class for GameCharacters, holds sprite and animation.
 */
class AnimatedObject :
	public cocos2d::CCNode
{
public:
	/**
	 * Tags used to indicate a sprite sheet.
	 */
	enum AnimTag
	{
		ANIM_BALLOON_SPRITESHEET,
		ANIM_PLANE_SPRITESHEET,
		ANIM_TWITTER_SPRITESHEET
	};

	AnimatedObject();
	void init(
			cocos2d::CCString textureFileName,
			cocos2d::CCString frameNamePrefix,
			int frameCount,
			float delay,
			cocos2d::CCLayer* layer,
			AnimTag spriteSheetTag);

	void init(
			cocos2d::CCString textureFileName,
			cocos2d::CCLayer* layer,
			AnimTag spriteTag);

	void setPosition(const cocos2d::CCPoint& pos);
	cocos2d::CCRect getRect();
	const cocos2d::CCSize& getSize();
	void destroy();

protected:
	/**
	 * Array holding animation frames of game object sprite.
	 */
	cocos2d::CCArray* 	__pAnimationFrames;

	/**
	 * Sprite sheet holding game object animation frames.
	 */
	cocos2d::CCSpriteBatchNode* 						__pSpriteSheet;

	/**
	 * Game object sprite.
	 */
	cocos2d::CCSprite*									__pSprite;

	/**
	 * Action describing game object animation.
	 */
	cocos2d::CCAction* 									__pAnimationAction;
};

#endif /* ANIMATEDOBJECT_H_ */
