

#include "ofMain.h"
#include "particle.h"
#include "spring.h"


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




class outerspace {
    
public:
    
    particlePlayer PP;
    particle P;
    spring s;
    
    ofPolyline LL;
    
    
    
    void setup(){
        PP.pctThrough = 0;
        PP.pctSpeed = 0.02;
        PP.bFixed = true;
        P.setInitialCondition(500, 500, 0, 0);
        s.particleA = &PP;
        s.particleB = &P;
        s.distance = 20;
        s.springiness = 0.05;
        
    }
 
    void update( ofPolyline line, int currFrame){
    
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
        
        if (LL.getVertices().size() > 20){
            LL.getVertices().erase(LL.getVertices().begin());
        }
        
    }

    void draw(){
        PP.draw();
        P.draw();
        s.draw();
        ofSetColor(255);
        LL.draw();
    }
    
};