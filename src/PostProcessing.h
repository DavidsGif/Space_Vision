
#pragma once

#include "RenderPass.h"
#include "ofCamera.h"

namespace itg
{
    class PostProcessing : public ofBaseDraws
    {
    public:
        typedef shared_ptr<PostProcessing> Ptr;
        
        void init(unsigned width = ofGetWidth(), unsigned height = ofGetHeight(), bool arb = false);
        void begin();
        void begin(ofCamera& cam);
        void end(bool autoDraw = true);
        
                void draw(float x = 0.f, float y = 0.f) const;
        void draw(float x, float y, float w, float h) const;
        float getWidth() const { return width; }
        float getHeight() const { return height; }
        
        void debugDraw();
        
        template<class T>
        shared_ptr<T> createPass()
        {
            shared_ptr<T> pass = shared_ptr<T>(new T(ofVec2f(width, height), arb));
            passes.push_back(pass);
            return pass;
        }
        
        ofTexture& getProcessedTextureReference();
        
                void process(ofFbo& raw, bool hasDepthAsTexture = true);
        
        
        void setFlip(bool flip) { this->flip = flip; }
        
        unsigned size() const { return passes.size(); }
        RenderPass::Ptr operator[](unsigned i) const { return passes[i]; }
        vector<RenderPass::Ptr>& getPasses() { return passes; }
        unsigned getNumProcessedPasses() const { return numProcessedPasses; }
        
        ofFbo& getRawRef() { return raw; }
        
    private:
        void process();
        
        unsigned currentReadFbo;
        unsigned numProcessedPasses;
        unsigned width, height;
        bool flip;
        bool arb;
        
        ofFbo raw;
        ofFbo pingPong[2];
        vector<RenderPass::Ptr> passes;
    };
}
