
#pragma once

#include "RenderPass.h"

namespace itg
{
    class NoiseWarpPass : public RenderPass
    {
    public:
        typedef shared_ptr<NoiseWarpPass> Ptr;
        
        NoiseWarpPass(const ofVec2f& aspect, bool arb, float frequency = 4.f, float amplitude = .1f, float speed = .1f);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        
        float getFrequency() const { return frequency; }
        void setFrequency(float frequency) { this->frequency = frequency; }
        
        float getAmplitude() const { return amplitude; }
        void setAmplitude(float amplitude) { this->amplitude = amplitude; }
        
        float getSpeed() const { return speed; }
        void setSpeed(float speed) { this->speed = speed; }
        
    private:
        ofShader shader;
        float frequency;
        float amplitude;
        float speed;
    };
    
}
