#pragma once

//#define _ITG_TWEAKABLE
#include "ofMain.h"
#include "ofFbo.h"
#include "ofVec3f.h"
#include "ofShader.h"
#ifdef _ITG_TWEAKABLE
    #include "Tweakable.h"
#endif

#define STRINGIFY(A) #A

namespace itg
{
    using namespace std;
    class RenderPass
#ifdef _ITG_TWEAKABLE
        : public Tweakable
#endif
    {
    public:
        typedef shared_ptr<RenderPass> Ptr;
        
        RenderPass(const ofVec2f& aspect, bool arb, const string& name);
        
        virtual void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        virtual void render(ofFbo& readFbo, ofFbo& writeFbo) {}
        
        void setEnabled(bool enabled) { this->enabled = enabled; }
        bool getEnabled() const { return enabled; }
        
        void enable() { enabled = true; }
        void disable() { enabled = false; }
        
        // for GUI
        bool& getEnabledRef() { return enabled; }
        
        void setAspect(const ofVec2f& aspect){ this->aspect = aspect; }
        
        void setArb(bool arb) { this->arb = arb; }
        
        virtual bool hasArbShader() { return false; }

#ifndef _ITG_TWEAKABLE
        string getName() const { return name; }
#endif

    protected:
        void texturedQuad(float x, float y, float width, float height, float s = 1.0, float t = 1.0);
        
        ofVec2f aspect;
        
        bool arb;
    
    private:
#ifndef _ITG_TWEAKABLE
        string name;
#endif
        bool enabled;
    };
}
