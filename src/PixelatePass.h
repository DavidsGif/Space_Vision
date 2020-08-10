
#pragma once

#include "RenderPass.h"

namespace itg
{
    class PixelatePass : public RenderPass
    {
    public:
        typedef shared_ptr<PixelatePass> Ptr;
        
        PixelatePass(const ofVec2f& aspect, bool arb, const ofVec2f& resolution = ofVec2f(100.f, 100.f));
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
    private:
        ofShader shader;
        ofVec2f resolution;
    };
}
