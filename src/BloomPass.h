#pragma once

#include "RenderPass.h"
#include "ConvolutionPass.h"

namespace itg
{
    class BloomPass : public RenderPass
    {
    public:
        typedef shared_ptr<BloomPass> Ptr;
        
        BloomPass(const ofVec2f& aspect, bool arb, const ofVec2f& xBlur = ofVec2f(0.001953125, 0.0), const ofVec2f& yBlur = ofVec2f(0.0, 0.001953125), unsigned resolution = 256, bool aspectCorrect = true);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
        void allocateSelectiveGlow(unsigned w, unsigned h);
        void beginSelectiveGlow(bool clear = true);
        void endSelectiveGlow();
        
        void debugDraw();
        
        bool hasArbShader() { return true; }
        
    private:
        ConvolutionPass::Ptr xConv;
        ConvolutionPass::Ptr yConv;
        
        ofFbo selectiveGlow;
                ofFbo fbos[2];
        
        unsigned currentReadFbo;
        unsigned w, h;
    };
}