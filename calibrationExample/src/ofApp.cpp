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
   
#ifdef TARGET_WIN32
    FDM.setup("../../../sharedData/SH002_Craig_test/");
#else
    FDM.setup("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/");
#endif
    
    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
    
    
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

   
    
    
    
    //
    //
    //
    
    
    
//    std::string name = "someOpsXform.abc";
//    OArchive archive( Alembic::AbcCoreHDF5::WriteArchive(), name );
//    TimeSampling Ts(1.0 / 24.0, 0);
//    Alembic::Util::uint32_t tsidx = archive.addTimeSampling(Ts);
//    OXform a( OObject( archive, kTop ), "a");
//    a.getSchema().setTimeSampling(tsidx);
//
//
//    for (int j = 0; j < FDM.numFrames; j++){
//
//        FDM.loadFrame(j, frame);            // load frame 0
//
//        vector<ofVec3f> from;
//        vector<ofVec3f> to;
//        for (int i = 0; i < frame.head.getIndices().size(); i+= 10){
//            to.push_back(frame.head.getVertices()[frame.head.getIndices()[i]]);
//            from.push_back(firstFrame.head.getVertices()[firstFrame.head.getIndices()[i]]);
//        }
//
//        ofMatrix4x4 rigidEstimate = ofxCv::estimateAffine3D(from, to);
//
//        XformSample samp;
//        XformOp matop( kMatrixOperation, kMatrixHint );
//
//        
//        ofPoint midPt (0,0,0);
//        ofMesh m = frame.head;
//        for (int i = 0; i < m.getNumIndices(); i++){
//            midPt += m.getVertices()[m.getIndices()[i]];
//        }
//        midPt /= (int)m.getNumIndices();
//        midPt = midPt;
//
//
//        
//        rigidEstimate.decompose(decompTranslation, decompRotation, decompScale, decompSo);
//
//        float angle;
//        float x, y,z;
//        decompRotation.getRotate(angle, x, y, z);
//
//        XformOp transop( kTranslateOperation, kTranslateHint );
//        XformOp rotatop( kRotateOperation, kRotateHint );
//        XformOp scaleop( kScaleOperation, kScaleHint );
//        const float *p = rigidEstimate.getPtr();
//        Imath::M44d md = Imath::M44d(p[0], p[1], p[2], p[3],
//                                     p[4], p[5], p[6], p[7],
//                                     p[8], p[9], p[10], p[11],
//                                     p[12], p[13], p[14], p[15]);
//
//        //samp.addOp( XformOp( kMatrixOperation, kMatrixHint ), md );
//
////        mm.glTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
////        mm.glTranslate( ofPoint(-midPt.x, midPt.y, midPt.z) + ofPoint(100,100,-100));
////        mm.glScale(-scaleFac,scaleFac,scaleFac);
//        // mm.glScale(1,1,1);
//        //
//        //mm.glRotate(mouseY, 1,0,0);
//        //
//        
//        ofMatrix4x4 rot;
//        rot.glRotate(angle, x,y,z);
//        
//        
//        samp.addOp( transop, V3d(-adjustments->x,adjustments->y,adjustments->z));
//        samp.addOp( transop, V3d(-midPt.x, midPt.y, midPt.z) );
//        //samp.addOp( scaleop, V3d(1.0, 1.0, 1.0) );
//        samp.addOp( rotatop, V3d(x,y,z), angle);
//
//
//        a.getSchema().set(samp);
//    }
    
    
    ofxAlembic::Writer writer;
    
    
    string path = ofGetTimestampString() + ".abc";
    writer.open(path, 24);
    
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
//
    
    
    
    
    
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
    

    ofPushMatrix();
    
    ofMatrix4x4 mat = n.getGlobalTransformMatrix();
    ofMultMatrix(mat);
    ofBoxPrimitive(100, 100, 100).draw();
    ofPopMatrix();


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
