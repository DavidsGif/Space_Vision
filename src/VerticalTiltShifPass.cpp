
#include "VerticalTiltShifPass.h"

namespace itg
{
    VerticalTiltShifPass::VerticalTiltShifPass(const ofVec2f& aspect, bool arb) :
    RenderPass(aspect, arb, "verticaltiltshift"), v(2.0/512.0), r(0.5)
    {
        // get result value as string
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2D tDiffuse;
                                         uniform float v;
                                         uniform float r;
                                         
                                         void main() {
                                             // create vec2 values
                                             vec2 vUv = gl_TexCoord[0].st;
                                             // create vec4 as 0
                                             vec4 sum = vec4( 0.0 );
                                             
                                             // use y value of vec2
                                             float vv = v * abs( r - vUv.y );
                                             
                                             // change texture 2D value following this rules
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 4.0 * vv ) ) * 0.051;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 3.0 * vv ) ) * 0.0918;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 2.0 * vv ) ) * 0.12245;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y - 1.0 * vv ) ) * 0.1531;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y ) ) * 0.1633;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 1.0 * vv ) ) * 0.1531;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 2.0 * vv ) ) * 0.12245;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 3.0 * vv ) ) * 0.0918;
                                             sum += texture2D( tDiffuse, vec2( vUv.x, vUv.y + 4.0 * vv ) ) * 0.051;
                                             
                                             gl_FragColor = sum;
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
#ifdef _ITG_TWEAKABLE
        addParameter("f", this->v, "min=0 max=1");
        addParameter("r", this->r, "min=0 max=1");
#endif
    }
    
    void VerticalTiltShifPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        
        shader.begin();
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
        shader.setUniform1f("v", v);
        shader.setUniform1f("r", r);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
