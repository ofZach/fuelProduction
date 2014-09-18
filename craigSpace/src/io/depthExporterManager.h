
#pragma once


#define GLSL(version, shader)  "#version " #version "\n" #shader

#include "ofMain.h"


class depthExporterManager {
    
    
    
public:
    
    void setup() {
        
        string shaderSource = GLSL(120,
                                   
                                   float LinearizeDepth(float zoverw)
                                   {
                                       float n = 18.016; // camera z near
                                       float f = 18016.0; // camera z far
                                       return (2.0 * n) / (f + n - zoverw * (f - n));
                                   }
                                   
                                   
                                   uniform sampler2D depthImg;
                                   uniform float depthValModifier;
                                   
                                   void main()
                                   {
                                       float depth = texture2D(depthImg, gl_TexCoord[0].xy).r;
                                       depth = LinearizeDepth(depth) * depthValModifier;
                                       
                                       
                                       gl_FragColor = vec4(depth, depth, depth, 1.0);
                                   }
                                   );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource);
        shader.linkProgram();
        
       int  ppWidth = ofNextPow2(1920);
        int  ppHeight = ofNextPow2(1080);
    
        ofFbo::Settings settings;
        settings.width = ppWidth;
        settings.height = ppHeight;
        settings.textureTarget = GL_RGBA32F;
        settings.textureTarget = GL_TEXTURE_2D;
        settings.depthStencilAsTexture = true;
        settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
        settings.useDepth = true;
        targetFbo.allocate(settings);
        
        output.allocate(1920,
                        1080,GL_RGBA32F);

        
    }
    
    
    ofFbo targetFbo;
    ofFbo output;
    ofShader shader;
    
    
    
    void startDraw(){
        
        targetFbo.begin();
        ofClear(0,0,0, 0);
        glClear(GL_DEPTH_BITS);
        
        
    }
    
    
    void endDraw(bool bSavingFrame = false, string frameName = ""){
        
        targetFbo.end();
        
        shader.begin();
        shader.setUniform1f("depthValModifier", 4);
        targetFbo.getDepthTexture().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);
        shader.end();
        
        targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - ofGetMouseX(), 1920, 1080);

        if (bSavingFrame){

            string fileName = frameName;

            ofSetColor(255,255,255);

            output.begin();
            ofClear(0,0,0,0);
            targetFbo.getTextureReference().drawSubsection(0, 0, 1920, 1080, 0, targetFbo.getHeight() - 1080, 1920, 1080);
            //targetFbo.draw(0,0);
            output.end();
            ofImage temp;
            temp.allocate(output.getWidth(), output.getHeight(), OF_IMAGE_COLOR_ALPHA);
            ofPixels tempPix;
            output.readToPixels(tempPix);

            ofSaveImage(tempPix, (fileName + "color" + ".png"));
            //temp.setFromPixels(tempPix);
            //temp.saveImage(fileName + "color" + ".png");

            output.begin();
            ofClear(0,0,0,0);
            shader.begin();
            shader.setUniform1f("depthValModifier", 4);
            targetFbo.getDepthTexture().drawSubsection(0, 0, 1920, 1080, 0, targetFbo.getHeight() - 1080, 1920, 1080);

            shader.end();
            output.end();
            output.readToPixels(tempPix);

            ofSaveImage(tempPix, fileName + "depth" + ".png");
            //temp.setFromPixels(tempPix);
            //temp.saveImage(fileName + "depth" + ".png");


        }
        
    }
    
    
    
};

//




//cout << "mouse x " << mouseX << endl;
