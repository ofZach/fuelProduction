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

	gui.add(line.startPointX.set("start x",  0, -200,  200));
	gui.add(line.startPointY.set("start y",  0, -200,  200));
	gui.add(line.startPointZ.set("start z",-900, -700, -1200));

	gui.add(line.extrudeAmount.set("extrude", 200, 0, 400));
	gui.add(line.arcRadius.set("arc radius", 200, 0, 400));
	gui.add(line.arcAngle.set("arc angle", 360, 180, 720));
	gui.add(line.rotationAmount.set("rotation amount", 360, 360, 360*10));
	
    gui.loadFromFile("adjustments.xml");
    
    //sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_1-2.aif");
    //sndPlayer.setLoop(true);
    //sndPlayer.setVolume(0);
    //sndPlayer.play();
    
    bSaving = false;
    
//#ifdef JAMES
	shotManager.footageBasePath = "/Volumes/CHOPPER/_ToyotaXpopTech_/GOLD_Footage/";
//#else 
//    shotManager.footageBasePath = "/Users/zachlieberman/Desktop/GOLD_Footage/";
//#endif 
    
	shotManager.setup();

	shotManager.loadShot("SH001", FDM); //jackie portrait
	
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
//	shotManager.loadShot("SH011", FDM); //CRAIG "we did it"

    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
    
    cout << FDM.numFrames << endl;
    
	ofSetVerticalSync(true);
	
	light.enable();
	light.setPosition(+500, +500, +500);
    
	string testSequenceFolder = dataPath + "aCam/";
    
	CCM.loadCalibration(testSequenceFolder + "matrices/rgbCalib.yml",
                        testSequenceFolder + "matrices/depthCalib.yml",
                        testSequenceFolder + "matrices/rotationDepthToRGB.yml",
                        testSequenceFolder + "matrices/translationDepthToRGB.yml");
	
    int ppWidth = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width);
    int ppHeight = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height);
    
    
    targetFbo.allocate(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width,
                       CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGBA32F);

   

    CM.CCM = CCM;
	CM.setup();
    
//    abc.open("SH04_Spline_01c.abc");
//    abc.dumpNames();
    
    line.setup();
	line.generateLine(2000);
	
    backgroundPlate.loadImage(dataPath + "Background Plates/A-Cam_BackgroundPlate_360p.png");
    
}

void ofApp::update() {
    
	line.update();
    
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
    
    CM.drawCameraInternals(frame.img, frame.img, backgroundPlate);

   
    ofPushMatrix();
        ofScale(-scaleFac,scaleFac,scaleFac);
        ofTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
        
        drawMesh(frame.head, ofColor::darkGoldenRod);
        drawMesh(frame.rightEye, ofColor::red);
        drawMesh(frame.leftEye, ofColor::blue);
        
        ofPushStyle();
    
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
	
	//line.drawArc();
    line.draw();
    
    
    
    
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

}

void ofApp::keyPressed(ofKeyEventArgs& args){
    
    if (args.key == 's'){
        gui.saveToFile("adjustments.xml");
    }
	
	if(args.key == 'l'){
		line.generateArcPoints();
		line.generateLinePoints(2000);
	}
    
    CM.keyPressed(args.key);
    
}
