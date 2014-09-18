#include "testApp.h"





//--------------------------------------------------------------
void testApp::setup(){	
	
	ofBackground(0,0,0);
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	
	for (int i = 0; i < 100; i++){
		particle myParticle;
		
		float x = 500 + 100 * cos ( (i / 100.0) * TWO_PI);
		float y = 500 + 100 * sin ( (i / 100.0) * TWO_PI);
		
		myParticle.setInitialCondition(x,y ,0,0);
		particles.push_back(myParticle);
	}
	
	// change this to
	// for (int i = 0; i < 10; i++){
	// to see a chain
	
	for (int i = 0; i < particles.size(); i++){
		spring mySpring;
		mySpring.distance		= 5;
		mySpring.springiness	= 0.1f;
		mySpring.particleA = & (particles[i  ]);
		mySpring.particleB = & (particles[(i+1) % particles.size()]);
		springs.push_back(mySpring);
	}
	
	bSavingLastFrame = false;
    bSaving = false;;
    frameNum = 0;
}

static int saveFrame = 0;
//--------------------------------------------------------------
void testApp::update(){

	// on every frame 
	// we reset the forces
	// add in any forces on the particle
	// perfom damping and
	// then update
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){
		
        if (ofGetMousePressed()){
            particles[i].addRepulsionForce(mouseX, mouseY, 100, 0.3f);
		}
		for (int j = 0; j < i; j++){
			particles[i].addRepulsionForce(particles[j], 20, 0.3);
		}
	}
	
	for (int i = 0; i < springs.size(); i++){
		springs[i].update();
	}
	
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].bounceOffWalls();
		particles[i].addDampingForce();
		particles[i].update();
	}
    
    
    if (!bSavingLastFrame && bSaving){
        string name =ofGetTimestampString() + ".abc";;
        message = "saving "  + name;
        writer.open(name);
    } else if (bSavingLastFrame && !bSaving){
        writer.close();
    }
    
    if (bSaving == true){
        
        vector<ofPolyline> curves;
        
            ofPolyline poly;
            
            for (int n = 0; n < particles.size(); n++)
            {
//                ofVec3f v;
//                v.x = ofSignedNoise(1, 0, 0, n * 0.01 + f * 10 + i) * 300;
//                v.y = ofSignedNoise(0, 1, 0, n * 0.01 + f * 10 + i) * 300;
//                v.z = ofSignedNoise(0, 0, 1, n * 0.01 + f * 10 + i) * 300;
//                poly.addVertex(v);
                poly.addVertex(particles[n].pos);
            }
            
            curves.push_back(poly);
        
        writer.addCurves("/blob", curves);
        
        saveFrame++;

        
    }
    
    
    bSavingLastFrame = bSaving;
    
}


//--------------------------------------------------------------
void testApp::draw(){
	

	ofSetColor(255);
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
	
	for (int i = 0; i < springs.size(); i++){
		springs[i].draw();
	}
    
    if (bSaving == true){
        ofSetColor(255,0,0);
        ofFill();
        ofTriangle(0,0,100,0,0,100);
        ofNoFill();
    }
    
    ofDrawBitmapStringHighlight(message, 20, 20);

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
			
		case ' ':
			// reposition everything: 
			for (int i = 0; i < particles.size(); i++){
				particles[i].setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
			}
			break;
	}
    
    
    if (key == 's'){
        
        
        bSaving = !bSaving;
        
        if (bSaving == true){
            frameNum = 0;
        }
        
    }
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
   
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	//particles[0].pos.set(mouseX, mouseY);
	/*particles.erase(particles.begin());
	particle myParticle;
	myParticle.setInitialCondition(x,y,0,0);
	particles.push_back(myParticle);*/
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//particles[0].bFixed = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
	//particles[0].bFixed = false;
}

