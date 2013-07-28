#include "cocos2d.h"
#include "Box2D.h"

#define kMaxHillKeyPoints 100
#define kMaxHillVertices 1000
#define kHillSegmentWidth 5
#define kMaxBorderVertices 5000

using namespace cocos2d;

class Terrain : public cocos2d::CCNode {
    
    CCPoint hillKeyPoints[kMaxHillKeyPoints];
	int nHillKeyPoints;
    int fromKeyPointI;
	int toKeyPointI;
	CCPoint hillVertices[kMaxHillVertices];
	CCPoint hillTexCoords[kMaxHillVertices];
	int nHillVertices;
    ccColor4F wireColor;
    CCPoint borderVertices[kMaxBorderVertices];
	int nBorderVertices;
	bool scrolling;
    
    b2World *world;
    b2Body *body;
    int screenW;
    int screenH;
    
    int textureSize;
    
protected: float offsetX;
public: virtual float getOffsetX(void) const { return offsetX; }
public: virtual void setOffsetX(float var);
    
public:
    CC_SYNTHESIZE_RETAIN(CCSprite *, stripes, Stripes);
    ~Terrain();
    void draw(void);
    
    static Terrain * terrainWithWorld(b2World* w);
    bool initWithWorld(b2World* w);
    
private:
    void generateHillKeyPoints();
    void resetHillVertices();
    void generateStripes();
    ccColor4F generateDarkColor();
    ccColor4F generateLightColorFrom(ccColor4F c);
    ccColor4F noColorWithAlpha(float alpha);
    void generateBorderVertices();
    void createBox2DBody();
    CCSprite* getStrippedTexture(ccColor4F c1, ccColor4F c2, float textureWidth, float textureHeight, int nStripes);
};
