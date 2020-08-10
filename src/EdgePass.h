
#pragma once

#include "RenderPass.h"

namespace itg
{
    
    class EdgePass : public RenderPass
    {
    public:
        typedef shared_ptr<EdgePass> Ptr;
        
        EdgePass(const ofVec2f& aspect, bool arb);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
        float getHue() const { return hue; }
        void setHue(float hue) { this->hue = hue; }
        
        float getSaturation() const { return saturation; }
        void setSaturation(float saturation) { this->saturation = saturation; }
        
        bool hasArbShader() { return true; }
        
    private:
        ofShader shader;
        float hue, saturation;
    };
}