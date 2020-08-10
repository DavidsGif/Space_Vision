
#pragma once

#include "RenderPass.h"
#include "ofShader.h"

namespace itg
{
    class BleachBypassPass : public RenderPass
    {
    public:
                typedef shared_ptr<BleachBypassPass> Ptr;
        
        BleachBypassPass(const ofVec2f& aspect, bool arb, float opacity = 1);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        
        void setOpacity(float v){ opacity = v; }
        float getOpacity() { return opacity; }
    private:
        
        ofShader shader;
        
        float opacity;
        
    };
}
