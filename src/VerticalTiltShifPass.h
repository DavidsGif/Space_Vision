
#pragma once

#include "RenderPass.h"

namespace itg
{
    
    class VerticalTiltShifPass : public RenderPass
    {
    public:
        typedef shared_ptr<VerticalTiltShifPass> Ptr;
        
        VerticalTiltShifPass(const ofVec2f& aspect, bool arb);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);

        float getV() { return v; }
        void setV(float val) { v = val; }

        float getR() { return r; }
        void setR(float val) { r = val; }
        
    private:
        ofShader shader;
        float v, r;
    };
}