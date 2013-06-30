#ifndef ENEMY_H_
#define ENEMY_H_

#include "cocos2d.h"
#include "AnimatedObject.h"

/**
 * Handles planes and birds movement.
 */
class Enemy : public AnimatedObject {
public:
	Enemy(float speed);
	void go();
private:
	/**
	 * Movement speed of the enemy.
	 */
	float __speed;
};

#endif /* ENEMY_H_ */
