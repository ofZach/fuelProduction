


#include "ofMain.h"




class rhondaLineRenderer {
    
    public:

    
    ofShader shader;
    ofImage ink;
    float fakeDepthAdder;       // this helps me "hit" around the matte framing, etc.    I am doing depth super weird and funky and weird
    
    
    void setup(){
        
        shader.load("rhonda/depth");
        ink.loadImage("rhonda/images/inkBig2.png");
    }

    void startDraw(){
        
    }
    
    void endDraw(){
        
    }

    void draw( ofPolyline line, ofCamera cam){
        
        //ofEnableDepthTest();
        
        
      
        //how can we pass gl_FragDepth ??????
        
        ofPoint top = ink.getTextureReference().getCoordFromPercent(0, 0);
        ofPoint bottom = ink.getTextureReference().getCoordFromPercent(1, 1);
        
        
        shader.begin();
        shader.setUniform1f("fakeDepthAdder", fakeDepthAdder);        // this is a kludge the helps get things depth testing OK against z=0....
        shader.setUniformTexture("ink", ink.getTextureReference(), 1);
        
        
        float dist = 0;
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        

        for (int i = 0; i < line.getVertices().size(); i++){
            
            int i_m_1 = MAX(0, i-1);
            int i_p_1 = MIN(line.size()-1, i+1);
            
            float pct =  sin( dist * 0.01) * 0.5 + 0.5;
            
            ofPoint onScreenA = cam.worldToScreen(line[i_m_1]);
            ofPoint onScreenB = cam.worldToScreen(line[i_p_1]);
            
            //cout << onScreenA << " " << onScreenB << endl;
            dist += (line[i] - line[i_m_1]).length();
            
            ofPoint screenPosA(onScreenA.x, onScreenA.y, 0);            // remove z components
            ofPoint screenPosB(onScreenB.x, onScreenB.y, 0);
            ofPoint dif = screenPosB - screenPosA;
            dif.normalize();
            ofPoint difCross = dif.cross(ofPoint(0,0,1));
            
            ofPoint a = cam.worldToScreen(line[i]) + (6 + 5 * sin( dist * 0.01)) * difCross;
            ofPoint b = cam.worldToScreen(line[i]) - (6 + 5 * sin( dist * 0.01)) * difCross;
            
            //a.y = 1080  - a.y;
            //b.y = 1080  - b.y;
            
            mesh.addVertex(a);
            mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
            mesh.addVertex(b);
            mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
            
            
        }
        
        
        //ink.bind();
        mesh.draw();
        //ink.unbind();
        
        shader.end();
    }
    
   
    
    
};




//float far=gl_DepthRange.far; float near=gl_DepthRange.near;
//vec4 eye_space_pos = gl_ModelViewMatrix * /*something*/
//vec4 clip_space_pos = gl_ProjectionMatrix * eye_space_pos;
//float ndc_depth = clip_space_pos.z / clip_space_pos.w;
//float depth = (((far-near) * ndc_depth) + near + far) / 2.0;
//gl_FragDepth = depth;
