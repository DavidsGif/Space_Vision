
#include "BleachBypassPass.h"
#include "ofMain.h"

namespace itg
{
    BleachBypassPass::BleachBypassPass(const ofVec2f& aspect, bool arb, float opacity) :
        opacity(opacity), RenderPass(aspect, arb, "bleachbypass")
    {
        
        string fragShaderSrc = STRINGIFY(
                                         uniform float opacity;
                                         uniform sampler2D tDiffuse;
                                         void main() {
                                             vec2 vUv = gl_TexCoord[0].st;
                                             vec4 base = texture2D( tDiffuse, vUv );
                                             
                                             vec3 lumCoeff = vec3( 0.25, 0.65, 0.1 );
                                             float lum = dot( lumCoeff, base.rgb );
                                             vec3 blend = vec3( lum );
                                             
                                             float L = min( 1.0, max( 0.0, 10.0 * ( lum - 0.45 ) ) );
                                             
                                             vec3 result1 = 2.0 * base.rgb * blend;
                                             vec3 result2 = 1.0 - 2.0 * ( 1.0 - blend ) * ( 1.0 - base.rgb );
                                             
                                             vec3 newColor = mix( result1, result2, L );
                                             
                                             float A2 = opacity * base.a;
                                             vec3 mixRGB = A2 * newColor.rgb;
                                             mixRGB += ( ( 1.0 - A2 ) * base.rgb );
                                             
                                             gl_FragColor = vec4( mixRGB, base.a );
                                             
                                         }
        );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    void BleachBypassPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex)
    {
        writeFbo.begin();
        
        
        shader.begin();
        
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
        shader.setUniform1f("opacity", opacity);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
