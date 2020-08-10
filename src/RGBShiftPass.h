
#pragma once

#include "RenderPass.h"
#include "ofShader.h"

namespace itg
{
    
    class RGBShiftPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<RGBShiftPass> Ptr;
        
        RGBShiftPass(const ofVec2f& aspect, bool arb, float amount = 0.005, float angle = 0.0);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        
        float getAmount(){ return amount; }
        void setAmount(float v) { amount = v; }
        
        float getAngle() { return angle; }
        void setAngle(float v) { angle = v; }
    private:
        
        ofShader shader;
        
        float amount, angle;
        
    };
}