//
//  HelloWorldScene.cpp
//  firstbox2d
//
//  Created by andy on 11-9-28.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//
#include "HelloWorldScene.h"

using namespace cocos2d;

#define PTM_RATIO 32
#define FLOOR_HEIGHT 62.0f
enum 
{
	kTagTileMap = 1,
	kTagSpriteManager = 1,
	kTagAnimation1 = 1,
}; 

HelloWorld::HelloWorld()
{
	setTouchEnabled( true );
	
	releasingArm = false;
	mouseJoint = NULL;
	currentBullet= 0;
	bulletBody = NULL;
	bulletJoint = NULL;
	gameover = false;
	//setIsAccelerometerEnabled( true );
    
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	//UXLOG(L"Screen width %0.2f screen height %0.2f",screenSize.width,screenSize.height);
    
	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
    
	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
    
	world->SetContinuousPhysics(true);
    
     
	
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner
	
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	groundBody = world->CreateBody(&groundBodyDef);
	
	// Define the ground box shape.
//	b2PolygonShape groundBox;		
	
    float widthInMeters = screenSize.width*2.0f / PTM_RATIO;
    float maxHeightInMeters = screenSize.height / PTM_RATIO;
    float minHeightInMeters = FLOOR_HEIGHT / PTM_RATIO;
    b2Vec2 lowerLeftCorner = b2Vec2(0, minHeightInMeters);
    b2Vec2 lowerRightCorner = b2Vec2(widthInMeters, minHeightInMeters);
    b2Vec2 upperLeftCorner = b2Vec2(0, maxHeightInMeters);
    b2Vec2 upperRightCorner = b2Vec2(widthInMeters, maxHeightInMeters);
    
    // Define the static container body, which will provide the collisions at screen borders.
    b2BodyDef screenBorderDef;
    screenBorderDef.position.Set(0, 0);
    b2Body* screenBorderBody = world->CreateBody(&screenBorderDef);
    b2EdgeShape screenBorderShape;
    
    // Create fixtures for the four borders (the border shape is re-used)
    screenBorderShape.Set(lowerLeftCorner, lowerRightCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(lowerRightCorner, upperRightCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(upperRightCorner, upperLeftCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(upperLeftCorner, lowerLeftCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
	
	//Set up sprite
	CCSprite *sprite = CCSprite::create("bg.png");
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointMake(0,0));
	addChild(sprite,-1);
	
	sprite = CCSprite::create("catapult_base_2.png");
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointMake(181.0f,FLOOR_HEIGHT));
	addChild(sprite, 0);
	
	sprite = CCSprite::create("squirrel_1.png");
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointMake(18.0f,FLOOR_HEIGHT));
	addChild(sprite, 0);
	
	sprite = CCSprite::create("catapult_base_1.png");
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointMake(181.0f,FLOOR_HEIGHT));
	addChild(sprite, 9);
	
	sprite = CCSprite::create("squirrel_2.png");
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointMake(240.0f,FLOOR_HEIGHT));
	addChild(sprite, 9);

	sprite = CCSprite::create("fg.png");
	sprite->setAnchorPoint(CCPointZero);
	addChild(sprite, 10);
	

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	//pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(  ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
	this->addChild(pMenu, 15 ,kTagTileMap);


	//Creat arm
	CCSprite *arm = CCSprite::create("catapult_arm.png");
	addChild(arm, 1);
	
	b2BodyDef armBodyDef;
	
	armBodyDef.type = b2_dynamicBody;
	armBodyDef.linearDamping = 1;
	armBodyDef.angularDamping = 1;
	armBodyDef.position.Set(230.0f/PTM_RATIO,(FLOOR_HEIGHT+91.0f)/PTM_RATIO);
	armBodyDef.userData = arm;
	armBody = world->CreateBody(&armBodyDef);
	
	b2PolygonShape armBox;
	b2FixtureDef armBoxDef;
	armBoxDef.shape = &armBox;
	armBoxDef.density = 0.3f;
	armBox.SetAsBox(11.0/PTM_RATIO, 91.0f/PTM_RATIO);
	armFixture = armBody->CreateFixture(&armBoxDef);
	
	b2RevoluteJointDef armJointDef;
	
	armJointDef.Initialize(groundBody,armBody,b2Vec2(233.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
	armJointDef.enableMotor = true;
	armJointDef.enableLimit = true;
	armJointDef.motorSpeed = -10;
	armJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(9);
	armJointDef.upperAngle = CC_DEGREES_TO_RADIANS(75);
	armJointDef.maxMotorTorque = 700;
	
	armJoint = (b2RevoluteJoint*)world->CreateJoint(&armJointDef);
	
	bullets = new vector<b2Body*>;
	targets = new vector<b2Body*>;
	enemies = new vector<b2Body*>;
//    CCSequence::cocos2d::CCActionInterval::create(<#float d#>)
	
	runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFunc::create(this, callfunc_selector(HelloWorld::resetGame)),NULL));
	
	contactListener = new MyContactListener();
	world->SetContactListener(contactListener);

	//this->runAction(CCScaleTo::actionWithDuration(1.5f,1.5f));
	
	schedule( schedule_selector(HelloWorld::tick) );
}

HelloWorld::~HelloWorld()
{
//	bullets->release();
	delete bullets;
	bullets = NULL;
	delete targets;
	targets = NULL;
	delete enemies;
	enemies = NULL;
	delete world;
	world = NULL;
	
	//delete m_debugDraw;
}

void HelloWorld::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//world->DrawDebugData();
	
	// restore default GL states
	glEnable(GL_TEXTURE_2D);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void HelloWorld::createTarget(const char* imageName ,CCPoint position ,float rotation ,bool isCircle ,bool isStatic ,bool isEnemy)
{
	CCSprite *sprite = CCSprite::create(imageName);
	addChild(sprite, 1);
	
	b2BodyDef bodyDef;
    bodyDef.type = isStatic?b2_staticBody:b2_dynamicBody;
    bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
                         (position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
    bodyDef.angle = CC_DEGREES_TO_RADIANS(rotation);
    bodyDef.userData = sprite;
	
    b2Body *body = world->CreateBody(&bodyDef);
	
	b2FixtureDef boxDef;
	if (isEnemy)
    {
        boxDef.userData = (void*)1;
        enemies->push_back(body);
    }

    if (isCircle)
    {
        b2CircleShape circle;
        circle.m_radius = sprite->getContentSize().width/2.0f/PTM_RATIO;
        boxDef.shape = &circle;
		
		boxDef.density = 0.5f;
		body->CreateFixture(&boxDef);
    }
    else
    {
        b2PolygonShape box;
        box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
        boxDef.shape = &box;
		
		boxDef.density = 0.5f;
		body->CreateFixture(&boxDef);
    }
    
 //   boxDef.density = 0.5f;
	
   // body->CreateFixture(&boxDef);
	
	targets->push_back(body);
}

void HelloWorld::createTargets()
{
	//[self createTarget:@"brick_2.png" atPosition:CGPointMake(675.0, FLOOR_HEIGTH) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_2.png",CCPointMake(675.0f, FLOOR_HEIGHT),0.0f,false,false,false);
	
	//[self createTarget:@"brick_1.png" atPosition:CGPointMake(741.0, FLOOR_HEIGTH) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_1.png",CCPointMake(741.0f, FLOOR_HEIGHT),0.0f,false,false,false);
	
	//[self createTarget:@"brick_1.png" atPosition:CGPointMake(741.0, FLOOR_HEIGTH+23.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_1.png",CCPointMake(741.0f, FLOOR_HEIGHT+23.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_3.png" atPosition:CGPointMake(672.0, FLOOR_HEIGTH+46.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_3.png",CCPointMake(672.0f, FLOOR_HEIGHT+46.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_1.png" atPosition:CGPointMake(707.0, FLOOR_HEIGTH+58.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_1.png",CCPointMake(707.0f, FLOOR_HEIGHT+58.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_1.png" atPosition:CGPointMake(707.0, FLOOR_HEIGTH+81.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_1.png",CCPointMake(707.0f, FLOOR_HEIGHT+81.0f),0.0f,false,false,false);
    
	//[self createTarget:@"head_dog.png" atPosition:CGPointMake(702.0, FLOOR_HEIGTH) rotation:0.0f isCircle:YES isStatic:NO isEnemy:YES];
    createTarget("head_dog.png",CCPointMake(702.0f, FLOOR_HEIGHT),0.0f,true,false,true);
	
	//[self createTarget:@"head_cat.png" atPosition:CGPointMake(680.0, FLOOR_HEIGTH+58.0f) rotation:0.0f isCircle:YES isStatic:NO isEnemy:YES];
    createTarget("head_cat.png",CCPointMake(680.0f, FLOOR_HEIGHT+58.0f),0.0f,true,false,true);
	
	//[self createTarget:@"head_dog.png" atPosition:CGPointMake(740.0, FLOOR_HEIGTH+58.0f) rotation:0.0f isCircle:YES isStatic:NO isEnemy:YES];
    createTarget("head_dog.png",CCPointMake(740.0f, FLOOR_HEIGHT+58.0),0.0f,true,false,true);
	
    // 2 bricks at the right of the first block
    //[self createTarget:@"brick_2.png" atPosition:CGPointMake(770.0, FLOOR_HEIGTH) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_2.png",CCPointMake(770.0f, FLOOR_HEIGHT),0.0f,false,false,false);
	
	//[self createTarget:@"brick_2.png" atPosition:CGPointMake(770.0, FLOOR_HEIGTH+46.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_2.png",CCPointMake(770.0f, FLOOR_HEIGHT+46.0f),0.0f,false,false,false);
	
    // The dog between the blocks
    //[self createTarget:@"head_dog.png" atPosition:CGPointMake(830.0, FLOOR_HEIGTH) rotation:0.0f isCircle:YES isStatic:NO isEnemy:YES];
    createTarget("head_dog.png",CCPointMake(830.0f, FLOOR_HEIGHT),0.0f,true,false,true);
	
    // Second block
    //[self createTarget:@"brick_platform.png" atPosition:CGPointMake(839.0, FLOOR_HEIGTH) rotation:0.0f isCircle:NO isStatic:YES isEnemy:NO];
    createTarget("brick_platform.png",CCPointMake(839.0f, FLOOR_HEIGHT),0.0f,false,true,false);
	
	//[self createTarget:@"brick_2.png"  atPosition:CGPointMake(854.0, FLOOR_HEIGTH+28.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_2.png",CCPointMake(854.0f, FLOOR_HEIGHT+28.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_2.png"  atPosition:CGPointMake(854.0, FLOOR_HEIGTH+28.0f+46.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_2.png",CCPointMake(854.0f, FLOOR_HEIGHT+28.0f+46.0f),0.0f,false,false,false);
	
	//[self createTarget:@"head_cat.png" atPosition:CGPointMake(881.0, FLOOR_HEIGTH+28.0f) rotation:0.0f isCircle:YES isStatic:NO isEnemy:YES];
    createTarget("head_cat.png",CCPointMake(881.0f, FLOOR_HEIGHT+28.0f),0.0f,true,false,true);
	
	//[self createTarget:@"brick_2.png"  atPosition:CGPointMake(909.0, FLOOR_HEIGTH+28.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_2.png",CCPointMake(909.0f, FLOOR_HEIGHT+28.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_1.png"  atPosition:CGPointMake(909.0, FLOOR_HEIGTH+28.0f+46.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_1.png",CCPointMake(909.0f, FLOOR_HEIGHT+74.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_1.png"  atPosition:CGPointMake(909.0, FLOOR_HEIGTH+28.0f+46.0f+23.0f) rotation:0.0f isCircle:NO isStatic:NO isEnemy:NO];
    createTarget("brick_1.png",CCPointMake(909.0f, FLOOR_HEIGHT+74.0f+23.0f),0.0f,false,false,false);
	
	//[self createTarget:@"brick_2.png"  atPosition:CGPointMake(882.0, FLOOR_HEIGTH+108.0f) rotation:90.0f isCircle:NO isStatic:NO isEnemy:NO];
	createTarget("brick_2.png",CCPointMake(882.0f, FLOOR_HEIGHT+108.0f),90.0f,false,false,false);

}

void HelloWorld::tick(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	int velocityIterations = 8;
	int positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);
	
	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
		}	
	}
	
	// Arm is being released.
    if (releasingArm && bulletJoint)
    {
        // Check if the arm reached the end so we can return the limits
        if (armJoint->GetJointAngle() <= CC_DEGREES_TO_RADIANS(10))
        {
            releasingArm = false;
            
            // Destroy joint so the bullet will be free
            world->DestroyJoint(bulletJoint);
            bulletJoint = NULL;
            
			runAction(CCSequence::create(CCDelayTime::create(5.0f),
										  CCCallFunc::create(this, callfunc_selector(HelloWorld::resetBullet)),NULL));
        }
    }
	
	// Bullet is moving.
    if (bulletBody && bulletJoint == NULL)
    {
        b2Vec2 position = bulletBody->GetPosition();
        CCPoint myPosition = getPosition();
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        // Move the camera.
        if (position.x > screenSize.width / 2.0f / PTM_RATIO)
        {
            myPosition.x = -MIN(screenSize.width * 2.0f - screenSize.width, position.x * PTM_RATIO - screenSize.width / 2.0f);
            //getPosition().x = myPosition.x;
			setPosition(myPosition);
			getChildByTag(kTagTileMap)->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20-myPosition.x, 20));
        }
    }
	
	// Check for impacts
    std::set<b2Body*>::iterator pos;
    for(pos = contactListener->contacts.begin(); 
        pos != contactListener->contacts.end(); ++pos)
    {
        b2Body *body = *pos;
        
        CCNode *contactNode = (CCNode*)body->GetUserData();
        CCPoint position = contactNode->getPosition();
        
		removeChild(contactNode, true);
        world->DestroyBody(body);
        
		std::vector<b2Body*>::iterator iter;
		for(iter = targets->begin() ; iter != targets->end() ; ++iter)
		{
			if(*iter == body)
			{
				targets->erase(iter);
				break;
			}
		}
		
		std::vector<b2Body*>::iterator it;
		for(it = enemies->begin() ; it!=enemies->end() ; ++it)
		{
			if(*it == body)
			{
				enemies->erase(it);
				break;
			}
		}
       // [targets removeObject:[NSValue valueWithPointer:body]];
       // [enemies removeObject:[NSValue valueWithPointer:body]];
		
        CCParticleSun* explosion = CCParticleSun::create();
		explosion->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
//		explosion->initWithTotalParticles(200);
        explosion->setAutoRemoveOnFinish(true);
        explosion->setStartSize(10.0f);
        explosion->setSpeed(50.0f);
        explosion->setAnchorPoint(ccp(0.5f,0.5f));
        explosion->setPosition(position);
        explosion->setDuration(1.0f);
        explosion->retain();
		addChild(explosion, 11);
        explosion->release(); 
    }
    
    // remove everything from the set
    contactListener->contacts.clear();
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent)
{
	int num = pTouches->count();
	CCLog("touchcount...%d",num);

	if(mouseJoint != NULL) return;
	
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	
	CCPoint location = touch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	if(gameover)
	{
		CCDirector::sharedDirector()->end();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	}
	
	b2Vec2 locationWorld = b2Vec2(convertedLocation.x/PTM_RATIO,convertedLocation.y/PTM_RATIO);
	
	if(locationWorld.x<armBody->GetWorldCenter().x + 50.0f/PTM_RATIO)
	{
		b2MouseJointDef md;
		md.bodyA = groundBody;
		md.bodyB = armBody;
		md.target = locationWorld;
		md.maxForce = 2000;
		
		mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
	}
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if(mouseJoint == NULL) return;
	
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	
	CCPoint location = touch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
	
	b2Vec2 locationWorld = b2Vec2(convertedLocation.x/PTM_RATIO,convertedLocation.y/PTM_RATIO);
	
	mouseJoint->SetTarget(locationWorld);
	
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if(mouseJoint != NULL)
	{
		if(armJoint->GetJointAngle()>=CC_DEGREES_TO_RADIANS(20))//检查弹臂是否松开并且恢复
		{
			releasingArm = true;
		}
		world->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}
}

void HelloWorld::createBullets(int count)
{
	currentBullet = 0;
    float pos = 62.0f;
    
    if (count > 0)
    {
        // delta is the spacing between corns
        // 62 is the position o the screen where we want the corns to start appearing
        // 165 is the position on the screen where we want the corns to stop appearing
        // 30 is the size of the corn
        float delta = (count > 1)?((165.0f - 62.0f - 30.0f) / (count - 1)):0.0f;
		
        bullets->reserve(count);
        for (int i=0; i<count; i++, pos+=delta)
        {
            // Create the bullet
            //
            CCSprite *sprite = CCSprite::create("acorn.png");
            addChild(sprite, 1);
            
            b2BodyDef bulletBodyDef;
            bulletBodyDef.type = b2_dynamicBody;
            bulletBodyDef.bullet = true;
            bulletBodyDef.position.Set(pos/PTM_RATIO,(FLOOR_HEIGHT+15.0f)/PTM_RATIO);
            bulletBodyDef.userData = sprite;
            b2Body *bullet = world->CreateBody(&bulletBodyDef);
            bullet->SetActive(false);
            
            b2CircleShape circle;
            circle.m_radius = 15.0/PTM_RATIO;
            
            b2FixtureDef ballShapeDef;
            ballShapeDef.shape = &circle;
            ballShapeDef.density = 0.8f;
            ballShapeDef.restitution = 0.2f;
            ballShapeDef.friction = 0.99f;
            bullet->CreateFixture(&ballShapeDef);
            
            bullets->push_back(bullet);
        }
    }
}

bool HelloWorld::attachBullet()
{
	if(currentBullet < bullets->capacity())
	{
		
		//bulletBody = (b2Body*)[[bullets objectAtIndex:currentBullet++] pointerValue];
        bulletBody = bullets->at(currentBullet++);
		bulletBody->SetTransform(b2Vec2(230.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO), 0.0f);
        bulletBody->SetActive(true);
		
        b2WeldJointDef weldJointDef;
        weldJointDef.Initialize(bulletBody, armBody, b2Vec2(240.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO));
        weldJointDef.collideConnected = false;
        
        bulletJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);
		return true;
	}
	return false;
}

void HelloWorld::attachBullet_v()
{
	if(currentBullet < bullets->capacity())
	{
		
		//bulletBody = (b2Body*)[[bullets objectAtIndex:currentBullet++] pointerValue];
        bulletBody = bullets->at(currentBullet++);
		bulletBody->SetTransform(b2Vec2(230.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO), 0.0f);
        bulletBody->SetActive(true);
		
        b2WeldJointDef weldJointDef;
        weldJointDef.Initialize(bulletBody, armBody, b2Vec2(240.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO));
        weldJointDef.collideConnected = false;
        
        bulletJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);
		
	}
}

void HelloWorld::resetBullet()
{
	if(enemies->size() == 0)
	{
		gameover = true;
	}
	else if(attachBullet())
	{
		runAction(CCMoveTo::create(2.0f, CCPointZero));
		getChildByTag(kTagTileMap)->runAction(CCMoveTo::create(2.0f, CCPointMake(CCDirector::sharedDirector()->getWinSize().width - 20, 20)));
	}
	else
	{
		gameover = true;
	}

}

void HelloWorld::resetGame()
{
	createBullets(4);
	//attachBullet();
	createTargets();
	runAction(CCSequence::create(CCMoveTo::create(1.5f, CCPointMake(-480.0f,0.0f)) ,
								  CCCallFunc::create(this, callfunc_selector(HelloWorld::attachBullet_v)),
								  CCDelayTime::create(1.0f),
								  CCMoveTo::create(1.5f, CCPointZero),
								  NULL));
	getChildByTag(kTagTileMap)->runAction(CCSequence::create(CCMoveTo::create(1.5f, CCPointMake(CCDirector::sharedDirector()->getWinSize().width + 460,20)) ,
								  CCDelayTime::create(1.0f),
								  CCMoveTo::create(1.5f, CCPointMake(CCDirector::sharedDirector()->getWinSize().width - 20, 20)),
								  NULL));
}
/*void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;
    
	for( it = touches->begin(); it != touches->end(); it++) 
	{
		touch = (CCTouch*)(*it);
        
		if(!touch)
			break;
        
		CCPoint location = touch->locationInView(touch->view());
		
		location = CCDirector::sharedDirector()->convertToGL(location);
        
		addNewSpriteWithCoords( location );
	}
}*/

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
