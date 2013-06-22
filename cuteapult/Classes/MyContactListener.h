/*
 *  MyContactListener.h
 *  firstbox2d
 *
 *  Created by andy on 11-9-29.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Box2D/Box2D.h"
#include <set>
#include <algorithm>

class MyContactListener : public b2ContactListener {
    
public:
    std::set<b2Body*>contacts;
    
    MyContactListener();
    ~MyContactListener();
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
};