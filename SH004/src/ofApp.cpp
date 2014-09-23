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
    
    gui.add(playbackSpeed.set("playbackSpeed", 1, 0.1, 2.0));
    gui.add(exporting.set("exporting", false));
    
	gui.add(drawFaceBox.set("draw face box", false));
	
    
    gui.add(depthOffsetForLines.set("depthOffsetForLines", 0.06, 0, 0.3));
    
    
    
    gui.add(transformCurve.set("transformCurve", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(scaleCurve.set("scaleCurve", ofPoint(1, 1, 1), ofPoint(0.3,0.3,0.3), ofPoint(2,2,2)));
    gui.add(rotateCurve.set("rotateCurve", ofPoint(0, 0, 0), -ofPoint(180,180,180), ofPoint(180,180,180)));
    
  
    
    gui.loadFromFile("adjustments.xml");
   

    sndPlayer.loadSound("/Users/zachlieberman/Desktop/GOLD_Footage/SH004/SH004_1-2.aif");
    sndPlayer.setLoop(true);
    sndPlayer.setVolume(0);
    sndPlayer.play();
    
    bSaving = false;


    ///Users/zachlieberman/Dropbox/+PopTech_Footage (2)

    
#ifdef JAMES
	shotManager.footageBasePath = "/Volumes/CHOPPER/_ToyotaXpopTech_/GOLD_Footage/";
#else
    shotManager.footageBasePath = "/Users/zachlieberman/Dropbox/+PopTech_Footage (2)";
#endif 
    
	shotManager.setup();

//	shotManager.loadShot("SH001", FDM); //jackie portrait
//	shotManager.loadShot("SH002", FDM); //craig portrait
//shotManager.loadShot("SH004", FDM); //matt portrait
	shotManager.loadShot("SH004", FDM); //craig scifi
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
    
    
    DEM.setup();
    

    
    abc.open("SH04_Spline_01c.abc");
    abc.dumpNames();
    
    
    backgroundPlate.loadImage("../../../sharedData/Background Plates/A-Cam_BackgroundPlate_360p.png");

    

    SPACE.setup();
}



bool bExportingLastFrame = false;
int exportingFrameCount = 0;
void ofApp::update() {
    
    
    SPACE.RLR.fakeDepthAdder = depthOffsetForLines;
    
    if (exporting){
        
        if (bExportingLastFrame == false){
            exportingFrameCount = 0;
            
            //string path = "outputTest.abc";
            //outputWriter.open(path);
            
            ////    string path = "cam.abc";
            ////    writer.open(path, 24);
            
        } else {
            
            exportingFrameCount++;
        }
        
        if (currentFrame > FDM.numFrames){
            exporting = false;
        }
        
        
        currentFrame = exportingFrameCount;
        if (lastFrame != currentFrame){
            FDM.loadFrame(currentFrame, frame);
            
        }
    }
    
    
    if (!exporting){
    
        if (bExportingLastFrame){
            //outputWriter.close();
        }

        if (!playback){
            currentFrame = mouseX;
        } else {
            
            float time = sndPlayer.getPositionMS() / 1000.0;
            if (playbackAudio){
                sndPlayer.setVolume(1);
            } else {
                sndPlayer.setVolume(0);
            }
            sndPlayer.setSpeed(playbackSpeed);
            
            currentFrame = (int)(time * 24.0);
            
        }
        if (lastFrame != currentFrame){
            FDM.loadFrame(currentFrame, frame);
        }
    }
    lastFrame = currentFrame;
    
    // TODO: here
    
    CM.update();
    
    bExportingLastFrame = exporting;
    
    
}


int lastFrameDraw = -1;

void ofApp::draw(){
    
    
    
#ifndef NO_ALEMBIC
    float t = currentFrame / 24.0;
    if (t > abc.getMaxTime()){
        t = abc.getMaxTime();
    }
    abc.setTime(t);
#endif
    
	DEM.startDraw();
    
    
	ofClear(0,0,0,0);
    glClear(GL_DEPTH);

    
    ofViewport(ofRectangle(0,0,1920, 1080));

    
    ofDisableDepthTest();
    //backgroundPlate.draw(0,0,1920,1080);
    ofEnableDepthTest();
    
    CM.cameraStart();

    ofMatrix4x4 inside;
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
        //n.draw();
        ofSetColor(255);
        ofMatrix4x4 mat = n.getGlobalTransformMatrix();
        ofMultMatrix(mat);
    
        inside = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    
		if(drawFaceBox){
			ofBoxPrimitive(100, 100, 100).draw();
		}

        
        ofPopStyle();
    
    
    
    
	ofPopMatrix();

    
    ofPolyline curve;

    vector<ofPolyline> curvesMe;
    abc.get("SplineSpline", curvesMe);
    ofPolyline copy = curvesMe[0];
    copy.addVertex(copy.getVertices()[0]);
    copy = copy.getResampledBySpacing(4);
    copy = copy.getSmoothed(11);
    copy.addVertex(copy.getVertices()[0]);
    copy = copy.getResampledBySpacing(4);
    copy = copy.getSmoothed(11);
    copy.addVertex(copy.getVertices()[0]);
    copy = copy.getResampledBySpacing(4);
    copy = copy.getSmoothed(80);
    copy.addVertex(copy.getVertices()[0]);
    copy = copy.getResampledBySpacing(4);
    copy = copy.getSmoothed(80);
    copy.addVertex(copy.getVertices()[0]);
    copy = copy.getResampledBySpacing(4);
    //copy.draw();
    
    ofPoint midPt;
    for (int i = 0; i < copy.size(); i++){
        midPt += copy[i];
    }
    midPt /= (float)copy.size();
//
//    
    ofMatrix4x4 rot;
    n.getOrientationQuat().get(rot);
    rot.rotate(rotateCurve->x,1,0,0);
    rot.rotate(rotateCurve->y,0,1,0);
    rot.rotate(rotateCurve->z,0,0,1);
    
    for (int i = 0; i < copy.size(); i++){
        copy[i] =  (((copy[i] - midPt + transformCurve)*rot.getInverse()) * scaleCurve + n.getPosition());
    }
    //copy.draw();
//
////    
    float pct = (float)currentFrame / (float)FDM.numFrames;
    
    
    if (lastFrameDraw != currentFrame){
        //cout << lastFrameDraw << " " << currentFrame << endl;
        
        for (int i = lastFrameDraw; i < currentFrame; i++){
            SPACE.update(copy, i);
            SPACE.update(copy, i);
        }
        
    }
    lastFrameDraw = currentFrame;
    
    if (currentFrame < 100 ){
        SPACE.reset();
    }
    
    SPACE.cam = &CM.baseCamera;
    
    // center the copy via the head position
    // draw with the head
    // put points moving around the copy
    // show them animate over time
    
    ofSetColor(ofColor::white);

	////////////////
    
	ofDisableDepthTest();
    CM.cameraEnd();
    
    SPACE.draw();
    
    
    CM.cameraStart();
    
    
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    if (!exporting){
        CM.drawCameraInternals(frame.img, frame.mask, backgroundPlate);
    }
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
    
    CM.cameraEnd();
    
    ofEnableAlphaBlending();
	
    
    if (exporting){
        
        int filenumber;
        std::ostringstream localOSS;
        string fileName;
        
        filenumber = 2034;
        
        localOSS << setw(4) << setfill('0') << currentFrame;
        
        fileName = localOSS.str();
        
        
        DEM.endDraw(true, fileName);
    } else {
        DEM.endDraw();
        
    }
    
    
    //DEM.endDraw(false, "");
    
    //ofDisableAlphaBlending();
    //targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);
    
    ofDisableAlphaBlending();
    ofDisableDepthTest();
    
    
    
    
    
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
    
    if (args.key == 'x'){
        sndPlayer.setPosition(0.1);
    }
    
    
    if (args.key == 's'){
        gui.saveToFile("adjustments.xml");
    }
    
    CM.keyPressed(args.key);
    
}
