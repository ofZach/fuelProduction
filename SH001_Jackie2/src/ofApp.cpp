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
	gui.add(exporting.set("exporting", false));
    
    gui.add(rotate.set("rotate", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(trans.set("trans", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(scale.set("scale", ofPoint(1,1,1), ofPoint(0.2,0.2,0.2), ofPoint(4.0,4.0,4.0)));
    
    
    
    ofParameter <ofVec3f > rotate;
    ofParameter <ofVec3f > trans;
    ofParameter <ofVec3f > scale;
    
    gui.loadFromFile("adjustments.xml");
    

    sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Footage (2)/SH001/SH001_1-2.aif");
    sndPlayer.setLoop(true);
    sndPlayer.setVolume(0);
    sndPlayer.play();
    

    bSaving = false;

    
#ifdef JAMES
	shotManager.footageBasePath = "/Volumes/CHOPPER/_ToyotaXpopTech_/GOLD_Footage/";
#else 
    shotManager.footageBasePath = "/Users/zachlieberman/Dropbox/+PopTech_Footage (2)";
#endif 
    
	shotManager.setup();

    shotManager.loadShot("SH001", FDM); //jackie portrait
//	shotManager.loadShot("SH002", FDM); //craig portrait
    //shotManager.loadShot("SH011", FDM); //matt portrait
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
    
    
    abc.open("JACKIE_001_2014-09-24-14-45-54-535.abc");
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
    
    

    
    abc.setTime(abc.getMaxTime());
    vector<ofPolyline> curvesMe;
    abc.get("curve", curvesMe);
    doneLine = curvesMe[0];
    doneLine = doneLine.getResampledByCount(800);
    
    for (int i = 0; i < doneLine.size(); i++){
        centroid += doneLine[i];
    }
    centroid /= (float)doneLine.size();
    
    
}


bool bExportingLastFrame = false;
int exportingFrameCount = 0;
ofxAlembic::Writer outputWriter;

void ofApp::update() {
    
    
    if (exporting){
        
        if (bExportingLastFrame == false){
            exportingFrameCount = 0;
            
            string path = "outputTest.abc";
            outputWriter.open(path, 24);
            
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
            outputWriter.close();
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


int lastFrameOfDraw = 0;
void ofApp::draw(){
    
    

	targetFbo.begin();
    ofViewport(ofRectangle(0,0,1920, 1080));
    
    
	ofClear(0,0,0,0);
    glClear(GL_DEPTH);
    
    
    CM.cameraStart();
    
    
    CM.drawCameraInternals(frame.img, frame.mask, backgroundPlate);
    ofPolyline curve;
    
    if (currentFrame < 100){
        smoother.clear();
        lastAddPoint = 0;
    } else {
        float t = ofMap(currentFrame, 100 + (6*24), 100 + (6*24)+5*24, 0, 1, true);
        t = powf(t, 1.9);
        
        //smoother.clear();
        if (t > 1) t = 1;
        if (t < 0) t = 0;
        cout << t << endl;
        
        int lastLast = lastAddPoint;
        lastAddPoint = doneLine.size() * t;
        
        if (lastLast <= lastAddPoint){
            for (int i = lastLast; i < lastAddPoint; i++){
                smoother.addVertex(doneLine.getVertices()[i]);
            }
        } else {
            smoother.clear();
            for (int i = 0; i < lastAddPoint; i++){
                smoother.addVertex(doneLine.getVertices()[i]);
            }
        }
        
        float pct = currentFrame / (float) FDM.numFrames;
        if (currentFrame != lastFrameOfDraw){
            int diff = currentFrame - lastFrameOfDraw;
            if (diff > 8) diff = 8;
            
            if (t < 0.99){
                for (int i = 0; i < diff; i++){
                    smoother = smoother.getSmoothed(3);
                }
            } else {
                smoother = smoother.getSmoothed(7);
                
                if (pct > 0.75){
                    for (int i = 0; i < 10; i++){
                        if (smoother.getVertices().size() > 1){
                            smoother.getVertices().erase(smoother.getVertices().begin());
                        }
                    }
                }
            }
        }
        
        
    }
    
    ofPolyline centeredSmooth =smoother;
    for (int i = 0; i < smoother.size(); i++){
        centeredSmooth.getVertices()[i] -= centroid;
    }
    
    ofSetColor(ofColor::pink);
    //smoother.draw();

    
    ofMatrix4x4 matInner;
    
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

    
    //ofGetCurrentMatrix(<#ofMatrixMode matrixMode_#>)
    
    ofTranslate(trans);
    ofRotate(rotate->x, 1,0,0);
    ofRotate(rotate->y, 0,1,0);
    ofRotate(rotate->z, 0,0,1);
    ofScale(scale->x, scale->y, scale->z);
    //ofParameter <ofVec3f > rotate;
    //ofParameter <ofVec3f > trans;
    //ofParameter <ofVec3f > scale;
    matInner= ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    
    
    
        ofPopStyle();
	ofPopMatrix();

    
    
    ofPolyline centeredSmoothedGood;
    for (int i = 0; i < centeredSmooth.size(); i++){
        centeredSmoothedGood.addVertex(centeredSmooth[i] * matInner + centroid);
    }
    centeredSmoothedGood.draw();
    
   
    
    if (exporting && lastFrameOfDraw!=currentFrame){
        vector < ofPolyline > lines;
        lines.push_back(centeredSmoothedGood);
        outputWriter.addCurves("/spline", lines);
    }
    
        
    lastFrameOfDraw = currentFrame;
    //ofCircle(curvesMe[0].getVertices()[curvesMe[0].getVertices().size()-1], 4);
    
    
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
    
    if (args.key == 'x'){
        sndPlayer.setPosition(0.4);
    }
    CM.keyPressed(args.key);
    
}
