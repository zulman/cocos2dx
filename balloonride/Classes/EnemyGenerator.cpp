#include "EnemyGenerator.h"
#include "GameObjects/AnimatedObject.h"

USING_NS_CC;

EnemyGenerator::EnemyGenerator() :
	__pLayer(NULL),
	__pEnemies(NULL),
	__winSize(),
	__enemySpeed(0)
{
}

/**
 * Set balloon properties and schedule movement update.
 * @param layer Layer that will hold sprites of generated objects.
 */
void
EnemyGenerator::init(CCLayer* layer)
{
	__pEnemies = new CCArray();
	__winSize = CCDirector::sharedDirector()->getWinSize();
	__pLayer = layer;
	__pLayer->addChild(this);
	__enemySpeed = 200.0f;
//	rand_r((unsigned)time(NULL));
}

/**
 * Schedule object generation.
 */
void
EnemyGenerator::start()
{
	schedule(schedule_selector(EnemyGenerator::generate), 0.5f);
}

/**
 * This callback will create
 */
void
EnemyGenerator::generate(float dt)
{
	//create new enemy object
	Enemy* enemy = new Enemy(__enemySpeed);
	__enemySpeed += 1.0f;

	CCString textureFileName = "plane.png";
	AnimatedObject::AnimTag tag = AnimatedObject::ANIM_PLANE_SPRITESHEET;

	int r = rand() % 100;
	if(r > 50)
	{
		// Change enemy
		textureFileName = "twitter.png";
		tag = AnimatedObject::ANIM_TWITTER_SPRITESHEET;
	}

	enemy->init(textureFileName, __pLayer, tag);

	// Choose random starting position
	CCSize enemySize = enemy->getSize();
	r = rand() % (int)(__winSize.height - enemySize.height) + enemySize.height / 2;
	float x = __winSize.width + enemySize.width;
	float y = r;
	enemy->setPosition(ccp(x, y));

	__pEnemies->addObject(enemy);
	__pLayer->addChild(enemy);
	enemy->go();
}

CCArray* EnemyGenerator::getEnemies()
{
	return __pEnemies;
}

/**
 * Remove off screen or colliding objects.
 * @param pEnemies Array of enemy objects that should be removed.
 */
void EnemyGenerator::remove(CCArray* pEnemies)
{
    CCObject* pObj;
    CCARRAY_FOREACH(pEnemies, pObj)
    {
        Enemy* pEnemy = (Enemy*)pObj;
		pEnemy->destroy();
    }
	__pEnemies->removeObjectsInArray(pEnemies);
}
