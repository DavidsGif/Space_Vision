
#pragma once

#include "RenderPass.h"

namespace itg
{
    class KaleidoscopePass : public RenderPass
    {
    public:
        typedef shared_ptr<KaleidoscopePass> Ptr;
        
        KaleidoscopePass(const ofVec2f& aspect, bool arb, float segments = 2.f);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        
        float getSegments() const { return segments; }
        void setSegments(float segments) { this->segments = segments; }
        
    private:
        ofShader shader;
        float segments;
    };
}
