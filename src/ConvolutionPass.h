#pragma once

#include "RenderPass.h"
#include "ofShader.h"

namespace itg
{

    class ConvolutionPass : public RenderPass
    {
    public:
        static const int MAX_KERNEL_SIZE = 25;
        
        typedef shared_ptr<ConvolutionPass> Ptr;
        
        ConvolutionPass(const ofVec2f& aspect, bool arb, const ofVec2f& imageIncrement = ofVec2f(0.001953125, 0), float sigma = 4, unsigned kernelSize = 25);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
        bool hasArbShader() { return true; }
    
    private:
        float gauss(float x, float sigma);
        void buildKernel(float sigma);
        vector<float> kernel;
        ofShader shader;
        ofVec2f imageIncrement;
    };
}
