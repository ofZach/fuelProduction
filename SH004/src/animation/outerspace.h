

#include "ofMain.h"
#include "particle.h"
#include "spring.h"
#include "rhondaLineRenderer.h"


class particlePlayer :  public particle{
    
public:
    
    float pctThrough;
    float pctSpeed;
    
    void updatePlay( int nFrames) {
        pctThrough = pctSpeed * (nFrames);
        if (pctThrough > 1.0){
            pctThrough -= 1.0;
        }
    }
    
};



class outerspacePlayer {
    
public:
    
    particlePlayer PP;
    particle P;
    spring s;
    ofPolyline LL;
    
    int lengthToRecord;
    
    void setup(){
        PP.pctThrough = ofRandom(0,0.1);
        PP.pctSpeed = ofRandom(0.1, 0.2);
        PP.bFixed = true;
        P.setInitialCondition(500, 500, 0, 0);
        s.particleA = &PP;
        s.particleB = &P;
        s.distance = ofRandom(20,200);
        s.springiness = ofRandom(0.04, 0.07);
        lengthToRecord = powf(ofRandom(0,1), 3) * 80;
        if (lengthToRecord < 4) lengthToRecord  = 4;

    }
    
    void update( ofPolyline line, int currFrame){
        
        while ( PP.pctThrough > 1.0)  PP.pctThrough -= 1.0;
        
        ofPolyline resamp = line.getResampledByCount(500);
        PP.updatePlay(currFrame);
        int whichVert = resamp.getVertices().size() * PP.pctThrough;
        whichVert %=resamp.getVertices().size();
        PP.pos.set( resamp.getVertices()[whichVert]);
        
        P.resetForce();
        s.update();
        P.addDampingForce();
        P.update();
        
        
        LL.addVertex(ofPoint(P.pos.x, P.pos.y, P.pos.z));
        //cout << ofPoint(P.pos.x, P.pos.y, P.pos.z) << endl;
        
        if (LL.getVertices().size() > lengthToRecord){
            LL.getVertices().erase(LL.getVertices().begin());
        }
        
    }
    
    void draw(){
        //PP.draw();
        //P.draw();
        //s.draw();
        ofSetColor(255);
        LL.draw();
    }
    
};


class outerspace {
    
public:
    
    
    
    vector < outerspacePlayer * > players;
    rhondaLineRenderer RLR;
    ofCamera * cam;
    
    void setup(){
        
        for (int i = 0; i < 30; i++){
            outerspacePlayer * p;
            p = new outerspacePlayer();
            p->setup();
            players.push_back(p);
            players[players.size()-1]->PP.pctSpeed = ofRandom(0.01, 0.018);
            players[players.size()-1]->PP.pctThrough = ofRandom(0, 2.0);
            
            
            //players[players.size()-1].P.damping = ofRandom(0.05, 0.13);
            //players[players.size()-1].s.distance = ofRandom(20,150);
        }
        
        
        RLR.setup();
        
    }
    
    
    
    void update( ofPolyline line, int currFrame){
        
        if (currFrame == 0){
            for (int i = 0; i < 30; i++){
                players[i]->PP.pctThrough = 0;
                players[i]->update(line, currFrame);
                players[i]->P.pos = players[i]->PP.pos;
                players[i]->LL.clear();
            }
        } else {
            for (int i = 0; i < 30; i++){
                players[i]->update(line, currFrame);
            }
        }
    }
    void draw(){
        
        
        ofMatrix4x4 mat = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
        
        
        //cam.setupPerspective();
        //cam.setTransformMatrix(mat.getInverse());
        
        
        // draw depth on these pixels
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        ofEnableDepthTest();
        for (int i = 0; i < 30; i++){
            //players[i]->LL.draw();
            RLR.draw(players[i]->LL, *cam);
        }
        
        
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        ofEnableAlphaBlending();
        ofDisableDepthTest();
        glEnable(GL_BLEND);
        glBlendEquation(GL_MIN_EXT);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_CONSTANT_COLOR);
        
        //ofSetColor(ofGetMouseX());
        for (int i = 0; i < 30; i++){
            players[i]->LL.draw();
            RLR.draw(players[i]->LL, *cam);
        }
        
        ofEnableDepthTest();
        ofEnableAlphaBlending();
        
        
    }
 
   
    
};