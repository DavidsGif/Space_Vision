
#include "PostProcessing.h"
#include "ofMain.h"

namespace itg
{
    void PostProcessing::init(unsigned width, unsigned height, bool arb)
    {
        this->width = width;
        this->height = height;
        this->arb = arb;
        
        ofFbo::Settings s;
        
        // if arb is true, width and height will be width and height of s value
        if (arb)
        {
            s.width = width;
            s.height = height;
            s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        }
        // else, width * width and height^2 will be width and height of s value
        else
        {
            s.width = ofNextPow2(width);
            s.height = ofNextPow2(height);
            s.textureTarget = GL_TEXTURE_2D;
        }
        
        for (int i = 0; i < 2; ++i)
        {
            pingPong[i].allocate(s);
        }
        
        // set s values
        s.useDepth = true;
        s.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
        s.depthStencilAsTexture = true;
        // allocate for s value
        raw.allocate(s);
        
        numProcessedPasses = 0;
        currentReadFbo = 0;
        flip = true;
    }
    
    void PostProcessing::begin()
    {
        raw.begin(OF_FBOMODE_NODEFAULTS);
        
        // set matrix mode as matrix_projection
        ofMatrixMode(OF_MATRIX_PROJECTION);
        ofPushMatrix();
        
        // set matrix mode as matrix_modelview
        ofMatrixMode(OF_MATRIX_MODELVIEW);
        ofPushMatrix();
        
        ofViewport(0, 0, raw.getWidth(), raw.getHeight());
        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        ofPushStyle();
        glPushAttrib(GL_ENABLE_BIT);
    }
    
    void PostProcessing::begin(ofCamera& cam)
    {
        cam.begin();
        cam.end();
        
        raw.begin(OF_FBOMODE_NODEFAULTS);
        
        // set matrix mode as matrix_projection
        ofMatrixMode(OF_MATRIX_PROJECTION);
        ofPushMatrix();
        ofLoadMatrix(cam.getProjectionMatrix(ofRectangle(0, 0, width, height)));
        
        // set matrix mode as matrix_modelview
        ofMatrixMode(OF_MATRIX_MODELVIEW);
        ofPushMatrix();
        ofLoadMatrix(cam.getModelViewMatrix());
        
        ofViewport(0, 0, raw.getWidth(), raw.getHeight());
        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        ofPushStyle();
        glPushAttrib(GL_ENABLE_BIT);
    }
    
    void PostProcessing::end(bool autoDraw)
    {
        glPopAttrib();
        ofPopStyle();
        
        ofViewport(0, 0, ofGetWidth(), ofGetHeight());
        
        // set matrix mode as matrix_projection
        ofMatrixMode(OF_MATRIX_PROJECTION);
        ofPopMatrix();
        
        // set matrix mode as matrix_modelview
        ofMatrixMode(OF_MATRIX_MODELVIEW);
        ofPopMatrix();
        
        raw.end();
        
        ofPushStyle();
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        ofSetColor(255, 255, 255);
        process();
        if (autoDraw) draw();
        glPopAttrib();
        ofPopStyle();
    }
    
    void PostProcessing::debugDraw()
    {
        // draw as uer settings
        raw.getTexture().draw(10, 10, 300, 300);
        raw.getDepthTexture().draw(320, 10, 300, 300);
        pingPong[currentReadFbo].draw(630, 10, 300, 300);
    }
    
    void PostProcessing::draw(float x, float y) const
    {
        draw(x, y, width, height);
    }
    
    void PostProcessing::draw(float x, float y, float w, float h) const
    {
        // if flip, draw to translate and scale
        if (flip)
        {
            ofPushMatrix();
            ofTranslate(x, y + h, 0);
            ofScale(1, -1, 1);
        }
        else glTranslatef(x, y, 0);
        if (numProcessedPasses == 0) raw.draw(0, 0, w, h);
        else pingPong[currentReadFbo].draw(0, 0, w, h);
        if (flip) ofPopMatrix();
    }
    
    ofTexture& PostProcessing::getProcessedTextureReference()
    {
        // get texture reference
        if (numProcessedPasses) return pingPong[currentReadFbo].getTexture();
        else return raw.getTexture();
    }
    
    void PostProcessing::process(ofFbo& raw, bool hasDepthAsTexture)
    {
        
        numProcessedPasses = 0;
        for (int i = 0; i < passes.size(); ++i)
        {
            if (passes[i]->getEnabled())
            {
                if (arb && !passes[i]->hasArbShader()) ofLogError() << "Arb mode is enabled but pass " << passes[i]->getName() << " does not have an arb shader.";
                else
                {
                    // if depth texture, render as current position and raw depth
                    if (hasDepthAsTexture)
                    {
                        if (numProcessedPasses == 0) passes[i]->render(raw, pingPong[1 - currentReadFbo], raw.getDepthTexture());
                        else passes[i]->render(pingPong[currentReadFbo], pingPong[1 - currentReadFbo], raw.getDepthTexture());
                    }
                    // else, render as current position
                    else
                    {
                        if (numProcessedPasses == 0) passes[i]->render(raw, pingPong[1 - currentReadFbo]);
                        else passes[i]->render(pingPong[currentReadFbo], pingPong[1 - currentReadFbo]);
                    }
                    currentReadFbo = 1 - currentReadFbo;
                    numProcessedPasses++;
                }
            }
        }
    }
    
    void PostProcessing::process()
    {
        process(raw);
    }
}
