
#include "DofPass.h"
#include "ofMain.h"

namespace itg
{
    DofPass::DofPass(const ofVec2f& aspect, bool arb, float focus, float aperture, float maxBlur) :
    focus(focus), aperture(aperture), maxBlur(maxBlur), RenderPass(aspect, arb, "dof")
    {
        // get result value as string
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2D tColor;
                                         uniform sampler2D tDepth;
                                         
                                         uniform float maxBlur;              uniform float aperture;
                                         uniform float focus;
                                         uniform float aspect;
                                         
                                         void main() {
                                             // new vec2 and vec4 values
                                             vec2 vUv = gl_TexCoord[0].st;
                                             
                                             vec2 aspectcorrect = vec2( 1.0, aspect );
                                             
                                             vec4 depth1 = texture2D( tDepth, vUv );
                                             
                                             float factor = depth1.x - focus;
                                             
                                             vec2 dofblur = vec2 ( clamp( factor * aperture, -maxBlur, maxBlur ) );
                                             
                                             vec2 dofblur9 = dofblur * 0.9;
                                             vec2 dofblur7 = dofblur * 0.7;
                                             vec2 dofblur4 = dofblur * 0.4;
                                             
                                             vec4 col = vec4( 0.0 );
                                             
                                             // update texture 2D values
                                             col += texture2D( tColor, vUv.xy );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.0,   0.4  ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.15,  0.37 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.29,  0.29 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.37,  0.15 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.40,  0.0  ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.37, -0.15 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.29, -0.29 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.15, -0.37 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.0,  -0.4  ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.15,  0.37 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.29,  0.29 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.37,  0.15 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.4,   0.0  ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.37, -0.15 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.15, -0.37 ) * aspectcorrect ) * dofblur );
                                             
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.15,  0.37 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.37,  0.15 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.37, -0.15 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.15, -0.37 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.15,  0.37 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.37,  0.15 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.37, -0.15 ) * aspectcorrect ) * dofblur9 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.15, -0.37 ) * aspectcorrect ) * dofblur9 );
                                             
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.29,  0.29 ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.40,  0.0  ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.29, -0.29 ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.0,  -0.4  ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.29,  0.29 ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.4,   0.0  ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur7 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.0,   0.4  ) * aspectcorrect ) * dofblur7 );
                                             
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.29,  0.29 ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.4,   0.0  ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.29, -0.29 ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.0,  -0.4  ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.29,  0.29 ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.4,   0.0  ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur4 );
                                             col += texture2D( tColor, vUv.xy + ( vec2(  0.0,   0.4  ) * aspectcorrect ) * dofblur4 );
                                             
                                             // get fragcolor from col value
                                             gl_FragColor = col / 41.0;
                                             gl_FragColor.a = 1.0;
                                             
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
#ifdef _ITG_TWEAKABLE
        addParameter("focus", this->focus, "min=0.95 max=1");
        addParameter("aperture", this->aperture, "min=0 max=1");
        addParameter("maxBlur", this->maxBlur, "min=0 max=1");
#endif
    }
    
    void DofPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex)
    {
        writeFbo.begin();
        
        shader.begin();
        
        shader.setUniformTexture("tColor", readFbo.getTexture(), 0);
        shader.setUniformTexture("tDepth", depthTex, 1);
        shader.setUniform1f("aperture", aperture);
        shader.setUniform1f("focus", focus);
        shader.setUniform1f("maxBlur", maxBlur);
        shader.setUniform1f("aspect", aspect.x / aspect.y);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
