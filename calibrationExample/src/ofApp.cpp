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
	
    
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(adjustments.set("adjustments", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(showWireframe.set("showWireframe", false));
    gui.add(showFilled.set("showFilled", false));
    gui.add(scaleFac.set("scaleFac", 1, 0.1, 2.0));
    gui.add(playback.set("playback", false));
    gui.add(playbackAudio.set("playbackAudio", false));
    gui.loadFromFile("adjustments.xml");
    

    //sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_1-2.aif");
    //sndPlayer.setLoop(true);
    //sndPlayer.setVolume(0);
    //sndPlayer.play();
    

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
//    /Users/zachlieberman/Desktop/of_v0.8.3_osx_release/apps/fuelProduction/sharedData/Background Plates/A-Cam_BackgroundPlate_360p.png
//    for (int j = 0; j < FDM.numFrames; j++){
//        
//        FDM.loadFrame(j, frame);            // load frame 0
//        ofMatrix4x4 mm;
//        mm.glScale(-scaleFac,scaleFac,scaleFac);
//        mm.glTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
//
//        ofMesh temp = frame.head;
//        cout << temp.getNumVertices() << endl;
//        ofxAlembic::transform(temp, mm);
//        writer.addPolyMesh("/head", temp);
//
//        ofNode n;
//        FDM.getOrientation(frame, n);
//
//        ofMatrix4x4 mat = n.getGlobalTransformMatrix();
//        ofMatrix4x4 newMat =  mat * mm;
//        newMat.glScale(-1, 1,1);
//        writer.addXform("/box", newMat);
//        if (j == 0){
//            ofBoxPrimitive box;
//            box.set(40);
//            writer.addPolyMesh("/box/boxShape", box.getMesh());
//        }
//        
//        // draw the box of orientation using new alexmbic style
//        
//        
//    }
//    
//    writer.close();
//
    
    
    
    
    
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
        
        //float totalTime = FDM.numFrames / 24.0;
        //float t = ofGetElapsedTimef();
        //while (t > totalTime) t -= totalTime;
        currentFrame = (int)(time * 24.0);
        
    }
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
