
#pragma once

#include "RenderPass.h"
#include "ofShader.h"

namespace itg
{
    class GodRaysPass : public RenderPass
    {
   
        typedef shared_ptr<GodRaysPass> Ptr;
        
        GodRaysPass(const ofVec2f& aspect, bool arb, const ofVec3f & lightPositionOnScreen = ofVec3f(0.5,0.5,0.5), float lightDirDOTviewDir = 0.3 );
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        
        void setLightPositionOnScreen(const ofVec3f & val) { lightPositionOnScreen = val; }
        const ofVec3f getlightPositionOnScreen() { return lightPositionOnScreen; }
        
        void setLightDirDOTviewDir(float val) { lightDirDOTviewDir = val; }
        float getLightDirDOTviewDir() { return lightDirDOTviewDir; }
    private:
        
        ofShader shader;
        
        ofVec3f lightPositionOnScreen;
        float lightDirDOTviewDir;
    };
}
