
#include "RenderPass.h"

namespace itg
{
    RenderPass::RenderPass(const ofVec2f& aspect, bool arb, const string& name) :
#ifdef _ITG_TWEAKABLE
        aspect(aspect), enabled(true), arb(arb), Tweakable(name)
    {
        addParameter("enable", enabled);
#else
        aspect(aspect), enabled(true), arb(arb), name(name)
    {
#endif
    }
    
    void RenderPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth)
    {
        render(readFbo, writeFbo);
    }
    
    void RenderPass::texturedQuad(float x, float y, float width, float height, float s, float t)
    {
        // TODO: change to triangle fan/strip
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, 0);
        
        glTexCoord2f(s, 0);
        glVertex3f(x + width, y, 0);
        
        glTexCoord2f(s, t);
        glVertex3f(x + width, y + height, 0);
        
        glTexCoord2f(0, t);
        glVertex3f(x, y + height, 0);
        glEnd();
    }
}
