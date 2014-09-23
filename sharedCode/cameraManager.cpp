
#include "cameraManager.h"

#define STRINGIFY(s) #s

void cameraManager::setup(){
    
	useSideCamera = false;
	currentCamera = &easyCam;
    
    string fragShaderSrc = STRINGIFY(
                                     
                                     uniform sampler2DRect tex0; // line image
                                     uniform sampler2DRect tex1; // rock image
                                     
                                     void main(void){
                                         vec2 st = gl_TexCoord[0].st;
                                         vec4 colorA = texture2DRect(tex0, st);
                                         vec4 colorB = texture2DRect(tex1, st);
                                         
                                         if (colorA.x > 0.01){
                                             gl_FragColor = vec4(colorB.x, colorB.y, colorB.z, colorA.x);
                                         } else {
                                             discard;
                                         }
                                     }
                                     
                                     );
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
    shader.linkProgram();


}

void cameraManager::update(){
    if(!useSideCamera){
		currentCamera = &baseCamera;
	}
	else{
		//update camera positions;
		ofVec3f center = baseCamera.screenToWorld( ofPoint(1920/2,1080) / 2 );
		ofVec3f camP = baseCamera.getPosition();
        
		center = camP + (center - camP).normalize() * 351*2;
        
		sideCam.setPosition(center + ofVec3f(1920,0,0));
		sideCam.lookAt(center,ofVec3f(0,1,0));
        
		topCamera.setPosition(center + ofVec3f(0,1080, 0));
		topCamera.lookAt(center,ofVec3f(1,0,0));
        
	}

}

void cameraManager::drawCameraInternals(ofImage &person, ofImage &mask, ofImage &backplate){
    
    
    ofPoint a,b,c,d, e;
    
    ofRectangle window;
    window.set(0,0,1920, 1080);
    
    a = baseCamera.screenToWorld( ofPoint(0,1080), window);
    b = baseCamera.screenToWorld( ofPoint(1920,1080), window);
    c = baseCamera.screenToWorld( ofPoint(0,0), window);
    d = baseCamera.screenToWorld( ofPoint(1920,0), window);
    
    ofPoint camP = baseCamera.getPosition();
    
    ofPoint aFar = camP + (a - camP).normalize() * 351*7;
    ofPoint bFar = camP + (b - camP).normalize() * 351*7;
    ofPoint cFar = camP + (c - camP).normalize() * 351*7;
    ofPoint dFar = camP + (d - camP).normalize() * 351*7;
    
    a = camP + (a - camP).normalize() * 351*5;
    b = camP + (b - camP).normalize() * 351*5;
    c = camP + (c - camP).normalize() * 351*5;
    d = camP + (d - camP).normalize() * 351*5;
    e = camP + (e - camP).normalize() * 351*4;
    
    
    
    
    
    // draw the backplate:
    
     ofMesh mesh;
    /*
    // I want to draw the backplay flat with no depth b/c of how the line renderer needs to fuck with depth.
    backplate.bind();
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.addVertex( aFar) ;
    mesh.addTexCoord(   ofPoint(0,backplate.getHeight()));
    mesh.addVertex(  bFar) ;
    mesh.addTexCoord(   ofPoint(backplate.getWidth(),backplate.getHeight()));
    mesh.addVertex(  cFar) ;
    mesh.addTexCoord(   ofPoint(0,0));
    mesh.addVertex(  dFar ) ;
    mesh.addTexCoord(   ofPoint(backplate.getWidth(), 0));
    mesh.draw();
    backplate.unbind();
    */
    // figure out a Z distance.
    
    shader.begin();
    shader.setUniformTexture("tex0", mask, 1);
    shader.setUniformTexture("tex1", person, 2);
    
    //frame.mask.bind();
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.addVertex( a) ;
    mesh.addTexCoord(   ofPoint(0,person.getHeight()));
    mesh.addVertex(  b) ;
    mesh.addTexCoord(   ofPoint(person.getWidth(),person.getHeight()));
    mesh.addVertex(  c) ;
    mesh.addTexCoord(   ofPoint(0,0));
    mesh.addVertex(  d ) ;
    mesh.addTexCoord(   ofPoint(person.getWidth(), 0));
    mesh.draw();
    //backplate.unbind();
    shader.end();
    
    ofLine( baseCamera.getPosition(), a);
    ofLine( baseCamera.getPosition(), b);
    ofLine( baseCamera.getPosition(), c);
    ofLine( baseCamera.getPosition(), d);
    
   
    
}

void cameraManager::cameraStart(){
    
    if(useSideCamera){
		currentCamera->begin( ofRectangle(0,0,1920, 1080));
        
		ofPushStyle();
		ofPushMatrix();
		ofNoFill();
		ofColor(255,0,0);
        //		ofDrawSphere(depthToRGBTranslation, 10);
		ofNode n;
		n.setPosition(CCM.depthToRGBTranslation);
		n.draw();
		ofColor(0,250,0);
		ofSphere(0,0,0,10);
		ofFill();
		ofSetColor(255,0,0);
		if(ofGetKeyPressed('m')){
			ofMultMatrix(CCM.extrinsics);
		}
		ofSetLineWidth(5);
		ofLine(ofVec3f(0,0,0), ofVec3f(0,0,-100));
		ofPopMatrix();
		ofPopStyle();
        ofEnableDepthTest();
	}
	else{
		ofVec3f camPos(0,0,0);
		camPos = CCM.extrinsics * camPos;
		baseCamera.setTransformMatrix(CCM.extrinsics);
        baseCamera.setFov( CCM.rgbCalibration.getDistortedIntrinsics().getFov().y );
		baseCamera.begin(ofRectangle(0,0,1920, 1080));
        ofEnableDepthTest();
	}

    
    
    // this helps for easy cam, bring the scene nearer to 0,0,0
    
    if (currentCamera == &easyCam){
        
        ofRectangle window;
        window.set(0,0,1920, 1080);
        
        
        ofPoint e = baseCamera.screenToWorld( ofPoint(1920,1080) / 2.0, window);
        ofPoint camP = baseCamera.getPosition();
        e = camP + (e - camP).normalize() * 351*4;
        ofTranslate(-e.x, -e.y, -e.z);
    }
    
    
}

void cameraManager::cameraEnd(){
    if(useSideCamera){
		currentCamera->end();
	}
	else{
		baseCamera.end();
	}
    
}

void cameraManager::keyPressed(int key){
    if(key == ' '){
        useSideCamera = !useSideCamera;
        if (useSideCamera){
            //currentCamera = &easyCam;
        }
    }
    
    if(useSideCamera){
        if(key == OF_KEY_LEFT){
            if(currentCamera == &easyCam){
                currentCamera = &sideCam;
            }
            else if(currentCamera == &sideCam){
                currentCamera = &topCamera;
            }
            else {
                currentCamera = &easyCam;
            }
        }
        else if(key == OF_KEY_RIGHT){
            if(currentCamera == &easyCam){
                currentCamera = &topCamera;
            }
            else if(currentCamera == &topCamera){
                currentCamera = &sideCam;
            }
            else {
                currentCamera = &easyCam;
            }
        }
    }
}
