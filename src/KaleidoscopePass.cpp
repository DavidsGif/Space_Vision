
#include "KaleidoscopePass.h"

namespace itg
{
    KaleidoscopePass::KaleidoscopePass(const ofVec2f& aspect, bool arb, float segments) :
    segments(segments), RenderPass(aspect, arb, "kaleido")
    {
        // get result value as string
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2D tex;
                                         uniform float segments;
                                         
                                         void main()
                                         {
                                             // create vec2 values
                                             vec2 uv = gl_TexCoord[0].st;
                                             vec2 normed = 2.0 * uv - 1.0;
                                             float r = length(normed);
                                             // arctangen function using vec2
                                             float theta = atan(normed.y / abs(normed.x));
                                             theta *= segments;
                                             
                                             // create new vec2 value
                                             vec2 newUv = (vec2(r * cos(theta), r * sin(theta)) + 1.0) / 2.0;
                                             
                                             // get texture 2D value from tex and new vec2
                                             gl_FragColor = texture2D(tex, newUv);
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
#ifdef _ITG_TWEAKABLE
        addParameter("segs", this->segments, "min=-20 max=20");
#endif
    }
    
    void KaleidoscopePass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth)
    {
        writeFbo.begin();
        
        shader.begin();
        
        shader.setUniformTexture("tex", readFbo.getTexture(), 0);
        shader.setUniform1f("segments", segments);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
