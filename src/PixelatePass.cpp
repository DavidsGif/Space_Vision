
#include "PixelatePass.h"

namespace itg
{
    PixelatePass::PixelatePass(const ofVec2f& aspect, bool arb, const ofVec2f& resolution) :
    resolution(resolution), RenderPass(aspect, arb, "pixelate")
    {
        // get result value as string
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2D tex;
                                         uniform float xPixels;
                                         uniform float yPixels;
                                         
                                         void main()
                                         {
                                             // create new vec2 value
                                             vec2 texCoords = vec2(floor(gl_TexCoord[0].s * xPixels) / xPixels, floor(gl_TexCoord[0].t * yPixels) / yPixels);
                                             // get texture 2D
                                             gl_FragColor = texture2D(tex, texCoords);
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
#ifdef _ITG_TWEAKABLE
        addParameter("x", this->resolution.x, "min=1 max=1000");
        addParameter("y", this->resolution.y, "min=1 max=1000");
#endif
    }
    
    void PixelatePass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        
        shader.begin();
        shader.setUniformTexture("tex", readFbo.getTexture(), 0);
        shader.setUniform1f("xPixels", resolution.x);
        shader.setUniform1f("yPixels", resolution.y);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
