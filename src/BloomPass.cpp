
#include "BloomPass.h"
#include "ofMain.h"

namespace itg
{
    BloomPass::BloomPass(const ofVec2f& aspect, bool arb, const ofVec2f& xBlur, const ofVec2f& yBlur, unsigned resolution, bool aspectCorrect) : RenderPass(aspect, arb, "bloom")
    {
        currentReadFbo = 0;
        if (resolution != ofNextPow2(resolution)) ofLogWarning() << "Resolution " << resolution << " is not a power of two, using " << ofNextPow2(resolution);
        
        xConv = ConvolutionPass::Ptr(new ConvolutionPass(aspect, arb, xBlur));
        yConv = ConvolutionPass::Ptr(new ConvolutionPass(aspect, arb, (aspectCorrect?aspect.x / aspect.y:1.f) * yBlur));
        
        ofFbo::Settings s;
        if (arb)
        {
            s.width = resolution;
            s.height = resolution * aspect.y / aspect.x;
            s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        }
        else
        {
            s.width = ofNextPow2(resolution);
            s.height = ofNextPow2(resolution);
            s.textureTarget = GL_TEXTURE_2D;
            
        }
        s.useDepth = true;
        
        for (int i = 0; i < 2; ++i) fbos[i].allocate(s);
    }
    
    void BloomPass::allocateSelectiveGlow(unsigned w, unsigned h)
    {
        this->w = w;
        this->h = h;
        
        ofFbo::Settings s;
        s.textureTarget = GL_TEXTURE_2D;
        s.width = ofNextPow2(w);
        s.height = ofNextPow2(h);
        s.useDepth = true;
        selectiveGlow.allocate(s);
        selectiveGlow.begin();
        ofClear(0,0,0,255);
        selectiveGlow.end();
    }
    
    void BloomPass::beginSelectiveGlow(bool clear)
    {
        selectiveGlow.begin();
        glPushMatrix();
        glScalef(1, -1, 1);
        glTranslatef(0, -ofNextPow2(h), 0);
        if (clear) ofClear(0,0,0, 255);
    }
    
    void BloomPass::endSelectiveGlow()
    {
        glPopMatrix();
        selectiveGlow.end();
    }
    
    void BloomPass::debugDraw()
    {
        glPushMatrix();
        glScalef(1, -1, 1);
        selectiveGlow.draw(0, -selectiveGlow.getHeight());
        glPopMatrix();
    }
    
    void BloomPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        if (selectiveGlow.isAllocated()) xConv->render(selectiveGlow, fbos[0]);
        else xConv->render(readFbo, fbos[0]);
        yConv->render(fbos[0], fbos[1]);
        
        writeFbo.begin();
        ofClear(0, 0, 0, 255);
        ofSetColor(255, 255, 255);
        readFbo.draw(0, 0);
        ofEnableAlphaBlending();
        glBlendFunc(GL_ONE, GL_ONE);
        fbos[1].draw(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ofDisableAlphaBlending();
        writeFbo.end();
    }
}