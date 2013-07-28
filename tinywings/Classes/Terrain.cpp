#include "Terrain.h"
#include "shaders/CCShaderCache.h"

//#define DRAW_WIREFRAME

#define PTM_RATIO 32 // pixel to metre ratio
Terrain * Terrain::terrainWithWorld(b2World* w) {
    Terrain *result = new Terrain();
    
    result->initWithWorld(w);
    result->autorelease();
    
    return result;
}
bool Terrain::initWithWorld(b2World* w) {
    world = w;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    screenW = size.width;
    screenH = size.height;
    
    textureSize = 256;
    
    generateStripes();
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
    generateHillKeyPoints();
    
    generateBorderVertices();
    createBox2DBody();
    
    offsetX = 0;
    return true;
}

Terrain::~Terrain() {
    stripes->release();
    stripes = NULL;
}

void Terrain::draw() {
    CC_NODE_DRAW_SETUP();
    ccGLBindTexture2D(stripes->getTexture()->getName());
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    
    ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, hillVertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, hillTexCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nHillVertices);
    
#ifdef DRAW_WIREFRAME
	glDisable(GL_TEXTURE_2D);
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, 0, 0, hillVertices);
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)nHillVertices);
    glEnable(GL_TEXTURE_2D);
#endif
}

ccColor4F Terrain::generateDarkColor() {
    srand(time(NULL));
    float cap = 1.0f;
    float r = (float)rand()/(float)RAND_MAX;
    float g = (float)rand()/(float)RAND_MAX;
    float b = (float)rand()/(float)RAND_MAX;
    if(r+g+b > 1)
    {
        float over = (cap - (r+g+b))/3;
        r-=over;
        g-=over;
        b-=over;
    }
    return ccc4f(r, g, b, 1.0f);
}

ccColor4F Terrain::noColorWithAlpha(float alpha) {
    return ccc4f(0,0,0,alpha);
}

ccColor4F Terrain::generateLightColorFrom(ccColor4F c) {
    srand(time(NULL));
    const float addon = 30/255;
    float r = min(c.r + addon, 1.0f);
    float g = min(c.r + addon, 1.0f);
    float b = min(c.r + addon, 1.0f);
    return ccc4f(r,g,b,1.0f);
}

CCSprite* Terrain::getStrippedTexture(ccColor4F c1, ccColor4F c2, float textureWidth, float textureHeight, int nStripes)
{
    CCRenderTexture *rt = CCRenderTexture::create(textureWidth,textureHeight);
    rt->beginWithClear(c1.r, c1.g, c1.b, c1.a);
    
    CCPoint* vertices = new CCPoint[nStripes*6];
    ccColor4F* colors = new ccColor4F[nStripes*6];
    int nVertices = 0;
    float x1 = -textureWidth;
    float x2;
	float y1 = textureHeight;
    float y2 = 0;
    float dx = textureWidth / nStripes * 2;
    float stripeWidth = dx/2;
    for (int i=0; i<nStripes; i++) {
        x2 = x1 + textureWidth;
        vertices[nVertices++] = ccp(x1, y1);
        vertices[nVertices++] = ccp(x1+stripeWidth, y1);
        vertices[nVertices++] = ccp(x2, y2);
        vertices[nVertices++] = vertices[nVertices-3];
        vertices[nVertices++] = vertices[nVertices-3];
        vertices[nVertices++] = ccp(x2+stripeWidth, y2);
        x1 += dx;
    }
    for (int i=0; i<nVertices; i++) {
        colors[i] = c2;
    }
    
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
    CCSprite *noise = CCSprite::create("noise.png");
    noise->setBlendFunc((ccBlendFunc){GL_DST_COLOR, GL_ZERO});
    noise->setPosition(ccp(textureWidth/2,textureHeight/2));
    noise->visit();
    
    CC_NODE_DRAW_SETUP();
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);
    
    //bottom border
    ccColor4F noColor = ccc4f(0,0,0,0);
    float gradientAlpha = 0.7f;
    nVertices = 0;
    
    vertices[nVertices] = ccp(0, 0);
    colors[nVertices++] = noColor;
    vertices[nVertices] = ccp(textureWidth, 0);
    colors[nVertices++] = noColor;
    vertices[nVertices] = ccp(0, textureHeight);
    colors[nVertices++] = noColorWithAlpha(gradientAlpha);
    vertices[nVertices] = ccp(textureWidth, textureHeight);
    colors[nVertices++] = noColorWithAlpha(gradientAlpha);
	
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
    
    // top border
    float borderHeight = textureHeight/20;
    float borderAlpha = 0.3f;
    ccColor4F borderColor = ccc4f(0,0,0,borderAlpha);
    
    nVertices = 0;
    vertices[nVertices] = ccp(0, 0);
    colors[nVertices++] = borderColor;
    vertices[nVertices] = ccp(textureWidth, 0);
    colors[nVertices++] = borderColor;
    vertices[nVertices] = ccp(0, borderHeight);
    colors[nVertices++] = noColor;
    vertices[nVertices] = ccp(textureWidth, borderHeight);
    colors[nVertices++] = noColor;
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
        
    rt->end();
    return rt->getSprite();
}


void Terrain::generateStripes() {
    const int minStripes = 2;
    const int maxStripes = 10;
    int nStripes = arc4random()%(maxStripes-minStripes)+minStripes;
    if (nStripes%2 != 0) {
        nStripes++;
    }

    ccColor4F bgColor = generateDarkColor();
    ccColor4F stripeColor = generateLightColorFrom(bgColor);
	
    CCSprite* stripesSprite = getStrippedTexture(bgColor, stripeColor, textureSize, textureSize, nStripes);
    stripesSprite->removeFromParentAndCleanup(true);
    ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    stripesSprite->getTexture()->setTexParameters(&tp);
    setStripes(stripesSprite);
}

void Terrain::generateHillKeyPoints() {
    // random key points
    nHillKeyPoints = 0;
    
    float x, y, dx, dy, ny;
    
    x = -screenW/4;
    y = screenH*3/4;
    hillKeyPoints[nHillKeyPoints++] = CCPointMake(x, y);
    
    // right under the starting point
    x = 0;
    y = screenH/2;
    hillKeyPoints[nHillKeyPoints++] = CCPointMake(x, y);
    
    srand(1);

    int minDX = 160, rangeDX = 80;
    int minDY = 60,  rangeDY = 60;
    float sign = -1; // +1 - going up, -1 - going  down
    float paddingTop = 20;
    float paddingBottom = 20;
 
    while (nHillKeyPoints < kMaxHillKeyPoints - 1) {
        dx = rand()%rangeDX+minDX;
        x += dx;
 
        dy = rand()%rangeDY+minDY;
        ny = y + dy*sign;
        if(ny > screenH-paddingTop) ny = screenH-paddingTop;
        if(ny < paddingBottom) ny = paddingTop;
        y = ny;
        sign *= -1;
        hillKeyPoints[nHillKeyPoints++] = CCPointMake(x, y);
    }
    
    // cliff
    x += minDX+rangeDX;
    y = 0;
    hillKeyPoints[nHillKeyPoints++] = ccp(x, y);
    
    fromKeyPointI = 0;
    toKeyPointI = 0;
}

void Terrain::generateBorderVertices() {
    nBorderVertices = 0;
    CCPoint p0, p1, pt0, pt1;
    p0 = hillKeyPoints[0];
    for (int i=1; i<nHillKeyPoints; i++) {
        p1 = hillKeyPoints[i];
        
        int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
        float dx = (p1.x - p0.x) / hSegments;
        float da = M_PI / hSegments;
        float ymid = (p0.y + p1.y) / 2;
        float ampl = (p0.y - p1.y) / 2;
        pt0 = p0;
        borderVertices[nBorderVertices++] = pt0;
        for (int j=1; j<hSegments+1; j++) {
            pt1.x = p0.x + j*dx;
            pt1.y = ymid + ampl * cosf(da*j);
            borderVertices[nBorderVertices++] = pt1;
            pt0 = pt1;
        }
        
        p0 = p1;
    }
}

void Terrain::createBox2DBody() {
    b2BodyDef bd;
    bd.position.Set(0, 0);
    
    body = world->CreateBody(&bd);
    
    b2Vec2 b2vertices[kMaxBorderVertices];
    int nVertices = 0;
    for (int i=0; i<nBorderVertices; i++) {
        b2vertices[nVertices++].Set(borderVertices[i].x/PTM_RATIO,borderVertices[i].y/PTM_RATIO);
    }
    b2vertices[nVertices++].Set(borderVertices[nBorderVertices-1].x/PTM_RATIO,0);
    b2vertices[nVertices++].Set(-screenW/4,0);
    
    b2ChainShape shape;
    shape.CreateChain(b2vertices, nVertices);
    body->CreateFixture(&shape, 0);
}

void Terrain::resetHillVertices() {
	
    static int prevFromKeyPointI = -1;
    static int prevToKeyPointI = -1;
    
    // key points interval for drawing
    float leftSideX = offsetX-screenW/8/getScale();
    float rightSideX = offsetX+screenW*7/8/getScale();
    while (hillKeyPoints[fromKeyPointI+1].x < leftSideX) {
        fromKeyPointI++;
    }
    while (hillKeyPoints[toKeyPointI].x < rightSideX) {
        toKeyPointI++;
    }
    
    if (prevFromKeyPointI != fromKeyPointI || prevToKeyPointI != toKeyPointI) {
        // vertices for visible area
        nHillVertices = 0;
        nBorderVertices = 0;
        CCPoint p0, p1, pt0, pt1;
        p0 = hillKeyPoints[fromKeyPointI];
        for (int i=fromKeyPointI+1; i<toKeyPointI+1; i++) {
            p1 = hillKeyPoints[i];
            
            // triangle strip between p0 and p1
            int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
            int vSegments = 1;
            float dx = (p1.x - p0.x) / hSegments;
            float da = M_PI / hSegments;
            float ymid = (p0.y + p1.y) / 2;
            float ampl = (p0.y - p1.y) / 2;
            pt0 = p0;
            borderVertices[nBorderVertices++] = pt0;
            for (int j=1; j<hSegments+1; j++) {
                pt1.x = p0.x + j*dx;
                pt1.y = ymid + ampl * cosf(da*j);
                //borderVertices[nBorderVertices++] = pt1;
                for (int k=0; k<vSegments+1; k++) {
                    hillVertices[nHillVertices] = ccp(pt0.x, pt0.y-(float)textureSize/vSegments*k);
                    hillTexCoords[nHillVertices++] = ccp(pt0.x/(float)textureSize, (float)(k)/vSegments);
                    hillVertices[nHillVertices] = ccp(pt1.x, pt1.y-(float)textureSize/vSegments*k);
                    hillTexCoords[nHillVertices++] = ccp(pt1.x/(float)textureSize, (float)(k)/vSegments);
                }
                pt0 = pt1;
            }
            
            p0 = p1;
        }
        
        prevFromKeyPointI = fromKeyPointI;
        prevToKeyPointI = toKeyPointI;
    }
}


void Terrain::setOffsetX(float newOffsetX) {
	float minOffsetX = 0;
	float maxOffsetX = hillKeyPoints[nHillKeyPoints-1].x-screenW;
	if (newOffsetX < minOffsetX) {
		newOffsetX = minOffsetX;
	}
	if (newOffsetX > maxOffsetX) {
		newOffsetX = maxOffsetX;
	}
    static bool firstTime = true;
    if (offsetX != newOffsetX || firstTime) {
        firstTime = false;
        offsetX = newOffsetX;
        setPosition(CCPointMake(screenW/8-offsetX*getScale(), 0));
        resetHillVertices();
    }
}