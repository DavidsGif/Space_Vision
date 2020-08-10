
#include "NoiseWarpPass.h"

namespace itg
{
    NoiseWarpPass::NoiseWarpPass(const ofVec2f& aspect, bool arb, float frequency, float amplitude, float speed) :
        frequency(frequency), amplitude(amplitude), speed(speed), RenderPass(aspect, arb, "noisewarp")
    {
        string fragShaderSrc = STRINGIFY(
            uniform sampler2D tex;

            uniform float frequency;
            uniform float amplitude;
            uniform float time;
            uniform float speed;

                                                                                                                        
            vec3 mod289(vec3 x) {
             return x - floor(x * (1.0 / 289.0)) * 289.0;
            }

            vec4 mod289(vec4 x) {
             return x - floor(x * (1.0 / 289.0)) * 289.0;
            }

            vec4 permute(vec4 x) {
             return mod289(((x*34.0)+1.0)*x);
            }

            vec4 taylorInvSqrt(vec4 r)
            {
                return 1.79284291400159 - 0.85373472095314 * r;
            }
                                             
            float snoise(vec3 v)
            {
                const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
                const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
                
                                vec3 i  = floor(v + dot(v, C.yyy) );
                vec3 x0 =   v - i + dot(i, C.xxx) ;
                
                                vec3 g = step(x0.yzx, x0.xyz);
                vec3 l = 1.0 - g;
                vec3 i1 = min( g.xyz, l.zxy );
                vec3 i2 = max( g.xyz, l.zxy );
                
                                                                                vec3 x1 = x0 - i1 + C.xxx;
                vec3 x2 = x0 - i2 + C.yyy;                 vec3 x3 = x0 - D.yyy;                      
                                i = mod289(i);
                vec4 p = permute( permute( permute(
                                                   i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
                                          + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
                                 + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
                
                                                float n_ = 0.142857142857;                 vec3  ns = n_ * D.wyz - D.xzx;
                
                vec4 j = p - 49.0 * floor(p * ns.z * ns.z);                  
                vec4 x_ = floor(j * ns.z);
                vec4 y_ = floor(j - 7.0 * x_ );                    
                vec4 x = x_ *ns.x + ns.yyyy;
                vec4 y = y_ *ns.x + ns.yyyy;
                vec4 h = 1.0 - abs(x) - abs(y);
                
                vec4 b0 = vec4( x.xy, y.xy );
                vec4 b1 = vec4( x.zw, y.zw );
                
                                                vec4 s0 = floor(b0)*2.0 + 1.0;
                vec4 s1 = floor(b1)*2.0 + 1.0;
                vec4 sh = -step(h, vec4(0.0));
                
                vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
                vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
                
                vec3 p0 = vec3(a0.xy,h.x);
                vec3 p1 = vec3(a0.zw,h.y);
                vec3 p2 = vec3(a1.xy,h.z);
                vec3 p3 = vec3(a1.zw,h.w);
                
                                vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
                p0 *= norm.x;
                p1 *= norm.y;
                p2 *= norm.z;
                p3 *= norm.w;
                
                                vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
                m = m * m;
                return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                             dot(p2,x2), dot(p3,x3) ) );
            }
                                                     
            void main()
            {
                vec2 texCoords = gl_TexCoord[0].st + vec2(
                    amplitude * (snoise(vec3(frequency * gl_TexCoord[0].s, frequency * gl_TexCoord[0].t, speed * time))),
                    amplitude * (snoise(vec3(frequency * gl_TexCoord[0].s + 17.0, frequency * gl_TexCoord[0].t, speed * time)))
                );
                gl_FragColor = texture2D(tex, texCoords);
            }
        );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
#ifdef _ITG_TWEAKABLE
        addParameter("amplitude", this->amplitude, "min=0 max=10");
        addParameter("frequency", this->frequency, "min=0 max=20");
        addParameter("speed", this->speed, "min=0 max=10");
#endif
    }
    
    void NoiseWarpPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth)
    {
        writeFbo.begin();
        shader.begin();
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniformTexture("tex", readFbo.getTexture(), 0);
        shader.setUniform1f("frequency", frequency);
        shader.setUniform1f("amplitude", amplitude);
        shader.setUniform1f("speed", speed);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}