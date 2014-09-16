#include "ofApp.h"
#include "ofxBinaryMesh.h"


static string dataPath = "../../../sharedData/";


void listDirs();

float scaleFac;
#ifndef NO_ALEMBIC
#include "ofxAlembic.h"
ofxAlembic::Reader abc;
#endif

void ofApp::setup() {
	
    
#ifndef NO_ALEMBIC
    abc.open("craig_02_test_fromEmmett_0912a.abc");
    abc.dumpNames();
#endif    
    
    
    bSaving = false;
    
    FDM.setup("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/");
    FDM.loadFrame(0, frame);            // load frame 0
    
	ofSetVerticalSync(true);
	
	useEasyCam = false;
	
	light.enable();
	light.setPosition(+500, +500, +500);

	string testSequenceFolder = dataPath + "aCam/";

	CCM.loadCalibration(
			testSequenceFolder + "matrices/rgbCalib.yml", 
			testSequenceFolder + "matrices/depthCalib.yml", 
			testSequenceFolder + "matrices/rotationDepthToRGB.yml", 
			testSequenceFolder + "matrices/translationDepthToRGB.yml");
	
    int ppWidth = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width);
    int ppHeight = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height);
    

    targetFbo.allocate(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width,
                       CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGBA32F);
   

	
    adjustGui = new ofxUISuperCanvas("ADJUST", 0,0, 300,500);
	adjustGui->addMinimalSlider("ADJUST X", -50, 50, &adjustments.x);
	adjustGui->addMinimalSlider("ADJUST Y", -50, 50, &adjustments.y);
	adjustGui->addMinimalSlider("ADJUST Z", -50, 50, &adjustments.z);
    scaleFac = 1.0;
    adjustGui->addMinimalSlider("SCALE", 0.7, 1.3, &scaleFac);
	adjustGui->addIntSlider("TIME OFFSET", -700, 700, &offsetShiftMillis);
	adjustGui->addToggle("SHOW OBJ SEQ", &showObjSequence);
	adjustGui->addToggle("SHOW BLEND SHAPE", &showBlendShape);
	adjustGui->addSpacer();
	adjustGui->addToggle("DRAW WIREFRAME", &showWireframe);
	adjustGui->addToggle("DRAW FILLED", &showFilled);
	adjustGui->addMinimalSlider("ONION SKIN ALPHA", 0, 1.0, &videoAlpha);
	adjustGui->loadSettings("adjustments.xml");

    
}





void ofApp::update() {

    currentFrame = mouseX;
    if (lastFrame != currentFrame){
        FDM.loadFrame(currentFrame, frame);
    }
    lastFrame = currentFrame;

}





void ofApp::draw(){
    
    
    //void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
	//ofSetupScreen();
#ifndef NO_ALEMBIC
    float t = currentFrame / 24.0;
    if (t > abc.getMaxTime()){
        t = abc.getMaxTime();
    }

	// update alemblic reader with time in sec
	abc.setTime(t);
#endif
    
	targetFbo.begin();
    ofViewport(ofRectangle(0,0,1920, 1080));
    
    
	ofClear(0,0,0,0);
    glClear(GL_DEPTH);
    

	float videoScale = targetFbo.getWidth() / frame.img.getWidth();
    
	if(useEasyCam){
		cam.begin();
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
		if(ofGetKeyPressed('m')){
			baseCamera.setPosition( ofVec3f(0,0,0) );
			baseCamera.lookAt(ofVec3f(0,0,-1));
		}
		else{
			baseCamera.setTransformMatrix(CCM.extrinsics);
		}
		baseCamera.setFov( CCM.rgbCalibration.getDistortedIntrinsics().getFov().y );
		baseCamera.begin(ofRectangle(0,0,1920, 1080));
	}
    
    //cout << baseCamera.getRoll() << " " << baseCamera.getPitch() << endl;
    
    
    //ofSetLineWidth(4);
    
    ofVec3f up(0,1,0);
    ofVec3f side(1,0,0);
    
    up = baseCamera.getLocalTransformMatrix() * up;
    up.normalize();
    //cout << up << endl;
    side = baseCamera.getLocalTransformMatrix() * side;
    side.normalize();
    
    ofVec3f out(0,0,1);
    out =baseCamera.getLocalTransformMatrix() * out;

    //cout << side << endl;
    
    ofSetColor(255,255,255);
    
    
    cout << baseCamera.getFov() << endl;
    cout << baseCamera.getAspectRatio() << endl;
    cout << baseCamera.getNearClip()<< endl;
    cout << baseCamera.getFarClip() << endl;
    cout << baseCamera.getPosition() << endl;
    

    // let's do something with height

    ofPoint a,b,c,d, e;
    
    a =baseCamera.screenToWorld( ofPoint(0,targetFbo.getHeight()));
    b = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth(),targetFbo.getHeight()));
    c = baseCamera.screenToWorld( ofPoint(0,0));
    d = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth(),0));
    e = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth()/2,targetFbo.getHeight()) / 2 );
    
    cout << baseCamera.getPosition() - (e - baseCamera.getPosition())  << endl;

    
    ofPoint camP = baseCamera.getPosition();
    
    a = camP + (a - camP).normalize() * 351*5;
    b = camP + (b - camP).normalize() * 351*5;
    c = camP + (c - camP).normalize() * 351*5;
    d = camP + (d - camP).normalize() * 351*5;
    e = camP + (e - camP).normalize() * 351*5;

    
    
    // figure out a Z distance.
    frame.img.bind();
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.addVertex( a) ;
    mesh.addTexCoord(   ofPoint(0,frame.img.getHeight()));
    mesh.addVertex(  b) ;
    mesh.addTexCoord(   ofPoint(frame.img.getWidth(),frame.img.getHeight()));
    mesh.addVertex(  c) ;
    mesh.addTexCoord(   ofPoint(0,0));
    mesh.addVertex(  d ) ;
    mesh.addTexCoord(   ofPoint(frame.img.getWidth(), 0));
    mesh.draw();
    frame.img.unbind();
    

	
    ofLine( baseCamera.getPosition(), a);
    ofLine( baseCamera.getPosition(), b);
    ofLine( baseCamera.getPosition(), c);
    ofLine( baseCamera.getPosition(), d);
 

    ofPushMatrix();
    ofScale(-scaleFac,scaleFac,scaleFac);
	ofTranslate(ofVec3f(-adjustments.x,adjustments.y,adjustments.z));
    drawMesh(frame.mesh, ofColor::darkGoldenRod);
	ofPopMatrix();
    
    
    

#ifndef NO_ALEMBIC
    vector<ofPolyline> curvesMe;
    abc.get("SplineSpline", curvesMe);
    
    ofEnableDepthTest();
    ofSetLineWidth(5);
    ofSetColor(255,255, 255);
    for (int i = 0; i < curvesMe.size(); i++)
        curvesMe[i].draw();
    ofSetLineWidth(1);
    
    abc.get("line10_PLASpline", curvesMe);

    ofSetColor(0, 0, 255);
    for (int i = 0; i < curvesMe.size(); i++)
        curvesMe[i].draw();
#endif

    
    ofSetColor(255,255,255);
    
    
    ofPushMatrix();

    ofMatrix4x4 mm;
    
    mm.glScale(-scaleFac,scaleFac,scaleFac);
	mm.glTranslate(ofVec3f(-adjustments.x,adjustments.y,adjustments.z));
    
    
    ofEnableDepthTest();
    ofPoint midPt (0,0,0);
    
    ofMesh m = frame.mesh;
    for (int i = 0; i < m.getNumIndices(); i++){
        midPt += m.getVertices()[m.getIndices()[i]];
    }
    
    midPt /= (int)m.getNumIndices();
    
    midPt = midPt * mm;
    
    vector<ofPolyline> curves;
    int count = 0;
    for (int i = 0; i < ofGetWidth(); i+= 3){
        int midNose = i;
        int who = ofMap(midNose, 0, ofGetWidth(), 0, m.getNumIndices()-1, true);
        //cout << mouseX << endl;
        //int who = midNose;
        ofVec3f pt = m.getVertices()[m.getIndices()[who]];
        
        pt = pt * mm;
        //cout << pt << endl;
        ofSetColor(255);
        
        
        ofPoint diff = ofPoint(pt.x, pt.y, pt.z) - midPt;
        
        float dist = 5 + 5 * sin(ofGetElapsedTimef());
        if (dist  < 1.5) dist = 1.5;
        //ofLine( (midPt + diff * (dist*0.8)), ( midPt + diff * dist));

        
        count++;
        
        
        //ofCircle( midPt + diff * dist, 2 + dist);
    }
    
    
    ofPopMatrix();
    
    
    
    
	////////////////

	ofDisableDepthTest();

	if(useEasyCam){
		cam.end();
	}
	else{
		baseCamera.end();
	}
    
    ofEnableAlphaBlending();

	targetFbo.end();
    
    targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);
    

    
}






void ofApp::drawMesh(ofMesh& m, ofFloatColor color){
	if(showFilled){
		ofSetColor(color);
		ofEnableDepthTest();
		ofEnableLighting();
		m.draw();
	}
	if(showWireframe){
		ofDisableLighting();
		ofSetColor(0);
		glDepthFunc(GL_LEQUAL);
		m.drawWireframe();
	}
	glDepthFunc(GL_LESS);
    ofDisableLighting();
}

void ofApp::exit(){
	//adjustGui->saveSettings("adjustments.xml");
}

void ofApp::keyPressed(ofKeyEventArgs& args){
	if(args.key == ' '){
		useEasyCam = !useEasyCam;
	}

    if (args.key == 's'){
        adjustGui->saveSettings("adjustments.xml");
    }
    
}
