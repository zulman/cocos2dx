#include "Balloon.h"

USING_NS_CC;

Balloon::Balloon() :
		__xMove(0.0f),
		__yMove(0.0f),
		__pMoveAction(NULL),
		__speed(0.0f),
		__winSize()
{
}

/**
 * Set balloon properties and schedule movement update.
 * @param layer Layer that will hold the sprite sheet.
 */
void
Balloon::init(cocos2d::CCLayer& layer)
{
	//If you don't add this object to the layer,
	//scheduler callback won't be able to access the object properties directly.
	layer.addChild(this);

    CCString frameNamePrefix = "balloon";
	CCString textureFileName = "balloons.png";
	int frameNumber = 3;
	float delay = 0.5f;
	__speed = 1000.0f;
	__winSize = CCDirector::sharedDirector()->getWinSize();

	AnimatedObject::init(textureFileName, frameNamePrefix, frameNumber, delay, &layer, ANIM_BALLOON_SPRITESHEET);
	schedule(schedule_selector(Balloon::move), 0.03f);
}

/**
 * Callback controlling balloon movement.
 */
void
Balloon::move(float dt)
{
	const int speedLimit = 50;
	if(__pSprite)
	{
		if(__pMoveAction)
		{
			// Stop the previous movement action.
			__pSprite->stopAction(__pMoveAction);
		}
		CCPoint currentPosition = __pSprite->getPosition();
		CCPoint newPosition = currentPosition;

		//Don't move too fast
		if(__xMove > speedLimit)
		{
			__xMove = speedLimit;
		}
		else if(__xMove < -speedLimit)
		{
			__xMove = -speedLimit;
		}

		if(__yMove > speedLimit)
		{
			__yMove = speedLimit;
		}
		else if(__yMove < -speedLimit)
		{
			__yMove = -speedLimit;
		}

		// Multiplying by dt should improve movement smoothness.
		float xMove = __xMove * 1000 * dt;
		float yMove = __yMove * 1000 * dt;

		//Dead zone for movement
		if(abs(xMove * 10) < 5)
		{
			xMove = 0;
		}

		if(abs(yMove * 10) < 5)
		{
			yMove = 0;
		}

		newPosition.x += xMove;
		newPosition.y += yMove;

		float hMargin = __pSprite->boundingBox().size.width / 2;
		float vMargin = __pSprite->boundingBox().size.height / 2;

		//Don't move too far
		if(newPosition.x < hMargin)
		{
			newPosition.x = hMargin;
		}
		else if(newPosition.x > __winSize.width - hMargin)
		{
			newPosition.x = __winSize.width - hMargin;
		}

		if(newPosition.y < vMargin)
		{
			newPosition.y = vMargin;
		}
		else if(newPosition.y > __winSize.height - vMargin)
		{
			newPosition.y = __winSize.height - vMargin;
		}

		//Count movement movement duration
		CCPoint moveDifference = ccpSub(currentPosition, newPosition);
		float distanceToMove = ccpLength(moveDifference);
		float moveDuration = distanceToMove / __speed;

		// Create the movement action
		__pMoveAction = CCMoveTo::create(moveDuration, newPosition);
		__pSprite->runAction(__pMoveAction);
	}
}

void
Balloon::setMove(float x, float y)
{
	__xMove = y;
	__yMove = x;
}
