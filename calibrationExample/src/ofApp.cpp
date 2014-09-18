#include "ofApp.h"
#include "ofxBinaryMesh.h"
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include "ofxAlembic.h"

using namespace Alembic::AbcGeom;

static string dataPath = "../../../sharedData/";


//void listDirs();


#ifndef NO_ALEMBIC
#include "ofxAlembic.h"
ofxAlembic::Reader abc;
#endif

void ofApp::setup() {
	
    
    
    // THIS IS SUPER WEIRD FOR ME -- adjustments gets NANs on OSX, and weirdness.
    
    // adjustments.set(0,0,0);
    // adjustGui = new ofxUISuperCanvas("ADJUST", 0,0, 300,500);
    // adjustGui->addMinimalSlider("ADJUST X", -50, 50, &adjustments->x);
    // adjustGui->addMinimalSlider("ADJUST Y", -50, 50, &adjustments->y);
    // adjustGui->addMinimalSlider("ADJUST Z", -50, 50, &adjustments->z);
    // scaleFac = 1.0;
    // adjustGui->addMinimalSlider("SCALE", 0.7, 1.3, &scaleFac);
    // adjustGui->loadSettings("adjustments->xml");
       
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(adjustments.set("adjustments", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(showWireframe.set("showWireframe", false));
    gui.add(showFilled.set("showFilled", false));
    gui.add(scaleFac.set("scaleFac", 1, 0.1, 2.0));
    gui.loadFromFile("adjustments.xml");
    

    bSaving = false;

	shotManager.footageBasePath = "/Volumes/CHOPPER/_ToyotaXpopTech_/GOLD_Footage/";
	shotManager.setup();

//	shotManager.loadShot("SH001", FDM); //jackie portrait
//	shotManager.loadShot("SH002", FDM); //craig portrait
//	shotManager.loadShot("SH003", FDM); //matt portrait
	//NOT ALIGNED AFTER CUT -- NO EYES AFTER CUT
//	shotManager.loadShot("SH004", FDM); //craig scifi
//	shotManager.loadShot("SH005", FDM); //Jackie SMOG
//	shotManager.loadShot("SH006", FDM); //jackie "made fuel cells important to me"
//	shotManager.loadShot("SH007", FDM); //MATT "Innovative technology"
//	shotManager.loadShot("SH008", FDM); //JACKiE change the way communities
//	shotManager.loadShot("SH009", FDM); //JACKIE 'states, nations, the world'
//	shotManager.loadShot("SH010", FDM); //CRAIG mental models;
	shotManager.loadShot("SH011", FDM); //CRAIG "we did it"
	
	
	//zach you can uncomment this to switch to your directory
//	string footagePath = "/Users/zachlieberman/Desktop/"
	
//    FDM.setup( footagePath + "SH002/Footage_smallsize_proxy",
//               footagePath + "SH002/SH002_Craig_003_OBM" );
    
    //JACKIE: timing is off?
//    FDM.setup(footagePath + "SH001/Footage_smallsize_proxy",
//              footagePath + "SH001/SH001_Jackie_002_OBM");

//    FDM.setup(footagePath + "SH004/Footage_smallsize_proxy",
//              footagePath + "SH004/SH004a_Craig_003_OBM");

	//JACKIE B CAM
//	FDM.setup(footagePath + "SH006/Footage_smallsize_proxy",
//			  footagePath + "SH006/SH006_Jackie_001_OBM");

	//JACKIE B CAM
//	FDM.setup(footagePath + "SH008/Footage_smallsize_proxy",
//              footagePath + "SH008/SH008_Jackie_001_OBM");

//	FDM.setup(footagePath + "SH009/Footage_smallsize_proxy",
//              footagePath + "SH009/SH009_Jackie_001_OBM");

//	FDM.setup("/Users/zachlieberman/Desktop/GOLD_Footage/SH003/Footage_smallsize_proxy",
//              "/Users/zachlieberman/Desktop/GOLD_Footage/SH003/SH003_Matt_004_OBM");

//	FDM.setup("/Users/zachlieberman/Desktop/GOLD_Footage/SH008/Footage_360p_proxy",
//              "/Users/zachlieberman/Desktop/GOLD_Footage/SH008/SH008_Jackie_001_OBM");

//	FDM.setup("/Users/zachlieberman/Desktop/GOLD_Footage/SH011/Footage_360p_proxy",
//          "/Users/zachlieberman/Desktop/GOLD_Footage/SH011/SH011_Craig_001_OBM");
    
    
    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
    
    cout << FDM.numFrames << endl;
    
   
	ofSetVerticalSync(true);
	
	useSideCamera = false;
	currentCamera = &easyCam;

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

   


    /*
     
    this saves out head and transform.
     
    ofxAlembic::Writer writer;
    
    string path = ofGetTimestampString() + ".abc";
    writer.open(path, 24);
    
    cout << FDM.numFrames << endl;
    
    for (int j = 0; j < FDM.numFrames; j++){
        
        FDM.loadFrame(j, frame);            // load frame 0
        ofMatrix4x4 mm;
        mm.glScale(-scaleFac,scaleFac,scaleFac);
        mm.glTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));

        ofMesh temp = frame.head;
        cout << temp.getNumVertices() << endl;
        ofxAlembic::transform(temp, mm);
        writer.addPolyMesh("/head", temp);

        ofNode n;
        FDM.getOrientation(frame, n);

        ofMatrix4x4 mat = n.getGlobalTransformMatrix();
        ofMatrix4x4 newMat =  mat * mm;
        newMat.glScale(-1, 1,1);
        writer.addXform("/box", newMat);
        if (j == 0){
            ofBoxPrimitive box;
            box.set(40);
            writer.addPolyMesh("/box/boxShape", box.getMesh());
        }
        
        // draw the box of orientation using new alexmbic style
        
        
    }
    
    writer.close();
*/
    
    
    
    
    
}



void ofApp::update() {
    
    currentFrame = mouseX;
    if (lastFrame != currentFrame){
        FDM.loadFrame(currentFrame, frame);
        
        // do the transform estimation:
        vector<ofVec3f> from;
        vector<ofVec3f> to;
        for (int i = 0; i < frame.head.getIndices().size(); i+= 1){
            to.push_back(frame.head.getVertices()[frame.head.getIndices()[i]]);
            from.push_back(firstFrame.head.getVertices()[firstFrame.head.getIndices()[i]]);
        }
        ofMatrix4x4 rigidEstimate = ofxCv::estimateAffine3D(from, to);
        XformSample samp;
        XformOp matop( kMatrixOperation, kMatrixHint );
        rigidEstimate.decompose(decompTranslation, decompRotation, decompScale, decompSo);
        //cout << " ? ? " << decompScale << endl;
    }
    lastFrame = currentFrame;
    
    
    
    if(!useSideCamera){
		currentCamera = &baseCamera;
	}
	else{
		//update camera positions;
		ofVec3f center = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth()/2,targetFbo.getHeight()) / 2 );
		ofVec3f camP = baseCamera.getPosition();
        
		center = camP + (center - camP).normalize() * 351*2;
    
		sideCam.setPosition(center + ofVec3f(targetFbo.getWidth(),0,0));
		sideCam.lookAt(center,ofVec3f(0,1,0));
        
		topCamera.setPosition(center + ofVec3f(0,targetFbo.getHeight(), 0));
		topCamera.lookAt(center,ofVec3f(1,0,0));
        
	}
    
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
    
	if(useSideCamera){
		currentCamera->begin();
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
//            cout << "cam position " << baseCamera.getPosition() << endl;
//            cout << "cam roll " << baseCamera.getRoll() << endl;
//            cout << "cam pitch " << baseCamera.getPitch() << endl;
//            cout << "cam heading " << baseCamera.getHeading() << endl;
//            cout << "cam fov " << baseCamera.getFov() << endl;
            
		}
		baseCamera.setFov( CCM.rgbCalibration.getDistortedIntrinsics().getFov().y );
		baseCamera.begin(ofRectangle(0,0,1920, 1080));
        ofEnableDepthTest();
	}
    
    
    ofPoint a,b,c,d, e;
    
    a = baseCamera.screenToWorld( ofPoint(0,targetFbo.getHeight()));
    b = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth(),targetFbo.getHeight()));
    c = baseCamera.screenToWorld( ofPoint(0,0));
    d = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth(),0));
    e = baseCamera.screenToWorld( ofPoint(targetFbo.getWidth(),targetFbo.getHeight()) / 2.0  + ofPoint(50,-50));
    
    ofPoint camP = baseCamera.getPosition();
    
    a = camP + (a - camP).normalize() * 351*5;
    b = camP + (b - camP).normalize() * 351*5;
    c = camP + (c - camP).normalize() * 351*5;
    d = camP + (d - camP).normalize() * 351*5;
    e = camP + (e - camP).normalize() * 351*4;
    
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
	ofTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));

	drawMesh(frame.head, ofColor::darkGoldenRod);
	drawMesh(frame.rightEye, ofColor::red);
	drawMesh(frame.leftEye, ofColor::blue);

	ofPushStyle();

	//ofScale(10,10,10);
	ofNoFill();
	ofNode n;
	FDM.getOrientation(frame, n);
	n.draw();
	ofSetColor(255);
    
ofPopStyle();

    ofPopMatrix();

#ifndef NO_ALEMBIC
    vector<ofPolyline> curvesMe;
    abc.get("SplineSpline", curvesMe);
#endif
    
    
    ofSetColor(ofColor::white);

	////////////////
    
	ofDisableDepthTest();

    
     
	if(useSideCamera){
		currentCamera->end();
	}
	else{
		baseCamera.end();
	}
    
    ofEnableAlphaBlending();
    
	targetFbo.end();
    
    targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);
    
    
    
    gui.draw();
    
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
	//adjustGui->saveSettings("adjustments->xml");
}

void ofApp::keyPressed(ofKeyEventArgs& args){
	if(args.key == ' '){
		useSideCamera = !useSideCamera;
        if (useSideCamera){
            currentCamera == &easyCam;
        }
	}

	if(useSideCamera){
		if(args.key == OF_KEY_LEFT){
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
		else if(args.key == OF_KEY_RIGHT){
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
    
    if (args.key == 's'){
        gui.saveToFile("adjustments.xml");
    }
    
}
