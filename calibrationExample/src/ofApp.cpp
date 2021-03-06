#include "ofApp.h"
#include "ofxBinaryMesh.h"
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include "ofxAlembic.h"

using namespace Alembic::AbcGeom;

static string dataPath = "../../../sharedData/";


#ifndef NO_ALEMBIC
#include "ofxAlembic.h"
ofxAlembic::Reader abc;
#endif

void ofApp::setup() {
	
    drawFaceBox = false;
	
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(adjustments.set("adjustments", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(showWireframe.set("showWireframe", false));
    gui.add(showFilled.set("showFilled", false));
    gui.add(scaleFac.set("scaleFac", 1, 0.1, 2.0));
    gui.add(playback.set("playback", false));
    gui.add(playbackAudio.set("playbackAudio", false));
	gui.add(drawFaceBox.set("draw face box", false));
	
    gui.loadFromFile("adjustments.xml");
    

    //sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_1-2.aif");
    //sndPlayer.setLoop(true);
    //sndPlayer.setVolume(0);
    //sndPlayer.play();
    

    bSaving = false;

    
#ifdef JAMES
	shotManager.footageBasePath = "/Volumes/CHOPPER/_ToyotaXpopTech_/GOLD_Footage/";
#else 
    shotManager.footageBasePath = "/Users/zachlieberman/Desktop/GOLD_Footage/";
#endif 
    
	shotManager.setup();

//	shotManager.loadShot("SH001", FDM); //jackie portrait
//	shotManager.loadShot("SH002", FDM); //craig portrait
    shotManager.loadShot("SH011", FDM); //matt portrait
	//NOT ALIGNED AFTER CUT -- NO EYES AFTER CUT
//	shotManager.loadShot("SH004", FDM); //craig scifi
//	shotManager.loadShot("SH005", FDM); //Jackie SMOG
//	shotManager.loadShot("SH006", FDM); //jackie "made fuel cells important to me"
//	shotManager.loadShot("SH007", FDM); //MATT "Innovative technology"
//	shotManager.loadShot("SH008", FDM); //JACKiE change the way communities
//	shotManager.loadShot("SH009", FDM); //JACKIE 'states, nations, the world'
//	shotManager.loadShot("SH010", FDM); //CRAIG mental models;
//shotManager.loadShot("SH011", FDM); //CRAIG "we did it"
//	shotManager.loadShot("SH011", FDM); //CRAIG "we did it"


    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
    
    cout << FDM.numFrames << endl;
    
	ofSetVerticalSync(true);
	
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

   

    CM.CCM = CCM;
	CM.setup();
    
    

//    // save head and tranform
//    
//    //CCM.extrinsics
//    
////    ofxAlembic::Writer writer;
////    string path = "cam.abc";
////    writer.open(path, 24);
////
////    writer.addXform("/box", CCM.extrinsics);
////    //if (j == 0){
////        ofBoxPrimitive box;
////        box.set(40);
////        writer.addPolyMesh("/box/boxShape", box.getMesh());
////    //}
////
////        // draw the box of orientation using new alexmbic style
////        
////        
////    //}
//    
//    
//    
//    writer.close();
    
    
    abc.open("SH04_Spline_01c.abc");
    abc.dumpNames();
    
    
    backgroundPlate.loadImage("../../../sharedData/Background Plates/A-Cam_BackgroundPlate_360p.png");

    
//    ofxAlembic::Writer writer;
//    
//    
//    string path = ofGetTimestampString() + ".abc";
//    writer.open(path, 24);
//    for (int j = 0; j < FDM.numFrames; j+=10){
//        
//        FDM.loadFrame(j, frame);            // load frame 0
//        ofMatrix4x4 mm;
//        mm.glScale(scaleFac,scaleFac,scaleFac);
//        mm.glTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
//
//        ofMesh temp = frame.head;
//        ofxAlembic::transform(temp, mm);
//        writer.addPolyMesh("/head", temp);
//
//        ofNode n;
//        FDM.getOrientation(frame, n);
//        
//        mm.preMult(n.getGlobalTransformMatrix());
//        
//        writer.addXform("/box", mm);
//        if (j == 0){
//            ofBoxPrimitive a;
//            a.set(100);
//            writer.addPolyMesh("/box/boxShape", a.getMesh());
//        }
//        
//        // draw the box of orientation using new alexmbic style
//        
//        
//    }
//    
//    writer.close();
////
    
    
    
    
    
}



void ofApp::update() {
    
    
    if (!playback){
        currentFrame = mouseX;
    } else {
        
        float time = sndPlayer.getPositionMS() / 1000.0;
        if (playbackAudio){
            sndPlayer.setVolume(1);
        } else {
            sndPlayer.setVolume(0);
        }
        
        currentFrame = (int)(time * 24.0);
        
    }
    if (lastFrame != currentFrame){
        FDM.loadFrame(currentFrame, frame);
        

    }
    lastFrame = currentFrame;
    
    // TODO: here
    
    CM.update();
    
}


void ofApp::draw(){
    
    
#ifndef NO_ALEMBIC
    float t = currentFrame / 24.0;
    if (t > abc.getMaxTime()){
        t = abc.getMaxTime();
    }
    abc.setTime(t);
#endif
    
	targetFbo.begin();
    ofViewport(ofRectangle(0,0,1920, 1080));
    
    
	ofClear(0,0,0,0);
    glClear(GL_DEPTH);
    
    
    CM.cameraStart();
    
    
    CM.drawCameraInternals(frame.img, frame.mask, backgroundPlate);

   
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
        ofMatrix4x4 mat = n.getGlobalTransformMatrix();
        ofMultMatrix(mat);
		if(drawFaceBox){
			ofBoxPrimitive(100, 100, 100).draw();
		}

        
        ofPopStyle();
	ofPopMatrix();

    
    ofPolyline curve;

#ifndef NO_ALEMBIC
    vector<ofPolyline> curvesMe;
    abc.get("SplineSpline", curvesMe);
#endif
    
    
    
    
    
    ofSetColor(ofColor::white);

	////////////////
    
	ofDisableDepthTest();
    CM.cameraEnd();
    ofEnableAlphaBlending();
	targetFbo.end();
    targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);
    gui.draw();
    
    
    //FDM.maskStandIn.draw(mouseX, mouseY);
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
    
    if (args.key == 's'){
        gui.saveToFile("adjustments.xml");
    }
    
    CM.keyPressed(args.key);
    
}
