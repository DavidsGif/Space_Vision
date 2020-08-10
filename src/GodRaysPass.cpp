#include "GodRaysPass.h"
#include "ofMain.h"

namespace itg
{
    GodRaysPass::GodRaysPass(const ofVec2f& aspect, bool arb, const ofVec3f & lightPositionOnScreen, float lightDirDOTviewDir) :
    lightPositionOnScreen(lightPositionOnScreen), lightDirDOTviewDir(lightDirDOTviewDir), RenderPass(aspect, arb, "godrays")
    {
        // get result value as string
        string vertShaderSrc = STRINGIFY(
                                         void main(void)
                                         {
                                             gl_TexCoord[0] = gl_MultiTexCoord0;
                                             gl_FrontColor = gl_Color;
                                             gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                                         }
                                         );
        
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2D rtex;
                                         uniform sampler2D otex;
                                         
                                         const int NUM_SAMPLES = 50;
                                         
                                         uniform vec2 lightPositionOnScreen;
                                         uniform float lightDirDOTviewDir;
                                         
                                         void main(void)
                                         {
                                             // create vec4 values
                                             vec4 origColor = texture2D(otex, gl_TexCoord[0].st);
                                             vec4 raysColor = texture2D(rtex, gl_TexCoord[0].st);
                                             
                                             // if larger than 0, textCoord will be created by lightPositionOnScreen
                                             if (lightDirDOTviewDir>0.0){
                                                 float exposure    = 0.1/float(NUM_SAMPLES);
                                                 float decay        = 1.0 ;
                                                 float density    = 0.5;
                                                 float weight    = 6.0;
                                                 float illuminationDecay = 1.0;
                                                 
                                                 vec2 deltaTextCoord = vec2( gl_TexCoord[0].st - lightPositionOnScreen);
                                                 vec2 textCoo = gl_TexCoord[0].st;
                                                 deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
                                                 
                                                 
                                                 // update raysColor and illuminationDecay
                                                 for(int i=0; i < NUM_SAMPLES ; i++)
                                                 {
                                                     textCoo -= deltaTextCoord;
                                                     vec4 tsample = texture2D(rtex, textCoo );
                                                     tsample *= illuminationDecay * weight;
                                                     raysColor += tsample;
                                                     illuminationDecay *= decay;
                                                 }
                                                 raysColor *= exposure * lightDirDOTviewDir;
                                                 float p = 0.3 *raysColor.g + 0.59*raysColor.r + 0.11*raysColor.b;
                                                 // set fragcolor as origncolor + p
                                                 gl_FragColor = origColor + p;
                                             } else {
                                                 gl_FragColor = origColor;
                                             }
                                         }
                                         );
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertShaderSrc);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    void GodRaysPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex)
    {
        writeFbo.begin();
        
        shader.begin();
        shader.setUniformTexture("otex", readFbo.getTexture(), 0);
        shader.setUniformTexture("rtex", readFbo.getTexture(), 1);
        shader.setUniform2f("lightPositionOnScreen", lightPositionOnScreen.x, lightPositionOnScreen.y);
        shader.setUniform1f("lightDirDOTviewDir", lightDirDOTviewDir);
        
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}
