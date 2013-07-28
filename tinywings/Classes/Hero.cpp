#include "Hero.h"

#define PTM_RATIO 32 // pixel to metre ratio

Hero::~Hero() {
    getSprite()->release();
    setSprite(NULL);
}

void Hero::createBox2DBody() {
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int screenH = size.height;
    
    CCPoint startPosition = ccp(0, screenH/2+radius);

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.linearDamping = 0.05f;
    bd.position.Set(startPosition.x/PTM_RATIO, startPosition.y/PTM_RATIO);
    body = world->CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_radius = radius/PTM_RATIO;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.restitution = 0; // bounce
    fd.friction = 0;
    
    body->CreateFixture(&fd);
}

Hero * Hero::heroWithWorld(b2World * w) {
    Hero *result = new Hero();
    result->initWithWorld(w);
    result->autorelease();
    return result;
}

bool Hero::initWithWorld(b2World * w) {
    setSprite(CCSprite::create("hero.png"));
    addChild(getSprite());
    
    world = w;
    radius = 16.0f;
    awake = false;

    createBox2DBody();
    updateNodePosition();
    sleep();
    
    return true;
}

void Hero::sleep() {
    awake = false;
    body->SetActive(false);
}

void Hero::wake() {
    awake = true;
    body->SetActive(true);
    body->ApplyLinearImpulse(b2Vec2(1,2), body->GetPosition());
}


void Hero::dive() {
    body->ApplyForce(b2Vec2(0,-40),body->GetPosition());
}


void Hero::limitVelocity() {
    const float minVelocityX = 2;
    const float minVelocityY = -40;
    b2Vec2 vel = body->GetLinearVelocity();
    if (vel.x < minVelocityX) {
        vel.x = minVelocityX;
    }
    if (vel.y < minVelocityY) {
        vel.y = minVelocityY;
    }
    
    body->SetLinearVelocity(vel);
}

void Hero::updateNodePosition() {
    float x = body->GetPosition().x*PTM_RATIO;
    float y = body->GetPosition().y*PTM_RATIO;
    if (y < radius) {
        y = radius;
    }

    setPosition(ccp(x, y));
    b2Vec2 vel = body->GetLinearVelocity();
    float angle = atan2f(vel.y, vel.x);
    setRotation(-1 * CC_RADIANS_TO_DEGREES(angle));
}





