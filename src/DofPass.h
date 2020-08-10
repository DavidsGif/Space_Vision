
#pragma once

#include "RenderPass.h"
#include "ofShader.h"

namespace itg
{
    
    class DofPass : public RenderPass
    {
    public:
        typedef shared_ptr<DofPass> Ptr;
        
        DofPass(const ofVec2f& aspect, bool arb, float focus = 0.985, float aperture = 0.8, float maxBlur = 0.6);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex);
        
        float getFocus() const { return focus; }
        void setFocus(float focus) { this->focus = focus; }
        
        float getAperture() const { return aperture; }
        void setAperture(float aperture) { this->aperture = aperture; }
        
        float getMaxBlur() const { return maxBlur; }
        void setMaxBlur(float maxBlur) { this->maxBlur = maxBlur; }
        
        float& getFocusRef() { return focus; }
        float& getApertureRef() { return aperture; }
        float& getMaxBlurRef() { return maxBlur; }
        
    private:
        ofShader shader;
        
        float focus;
        float aperture;
        float maxBlur;
    };
}