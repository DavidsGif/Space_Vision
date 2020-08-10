
#include "RGBShiftPass.h"
#include "ofMain.h"

namespace itg
{
    RGBShiftPass::RGBShiftPass(const ofVec2f& aspect, bool arb, float amount, float angle) :
        amount(amount), angle(angle), RenderPass(aspect, arb, "RGBShift")
    {
        
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2D tDiffuse;
                                         uniform float amount;
                                         uniform float angle;
                        
                                         void main() {
                                         vec2 vUv = gl_TexCoord[0].st;
                                         vec2 offset = amount * vec2( cos(angle), sin(angle));
                                         vec4 cr = texture2D(tDiffuse, vUv + offset);
                                         vec4 cga = texture2D(tDiffuse, vUv);
                                         vec4 cb = texture2D(tDiffuse, vUv - offset);
                                         gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);
                                         
                                         }
        );
        
    
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    

    void RGBShiftPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex)
    {
        writeFbo.begin();
        
        
        shader.begin();
        
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 0);
        shader.setUniform1f("amount", amount);
        shader.setUniform1f("angle", angle);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}