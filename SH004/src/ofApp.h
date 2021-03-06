#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"
#include "frameDataManager.h"
#include "shotManager.h"
#include "ofxGui.h"
#include "cameraManager.h"
#include "outerspace.h"
#include "depthExporterManager.h"

class ofApp : public ofBaseApp{
public:
    
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(ofKeyEventArgs& args);
    void listDirs();
    
    frameDataManager FDM;
    shotManager shotManager;
    frameData frame;
    frameData firstFrame;

    int currentFrame;
    int lastFrame;


	ofLight light;
	
	ofFbo targetFbo;

	void drawMesh(ofMesh& m, ofFloatColor color);

	//ofxUISuperCanvas* adjustGui;
	ofParameter <ofVec3f> adjustments;
	ofParameter <bool> showWireframe;
	ofParameter <bool> showFilled;
    ofParameter <float> scaleFac;
    ofParameter <bool> playback;
    ofParameter <bool> playbackAudio;
	ofParameter <bool> drawFaceBox;
	ofParameter <float> playbackSpeed;
	
    
    ofParameter <float> depthOffsetForLines;
    ofParameter <ofVec3f> transformCurve;
    ofParameter <ofVec3f> scaleCurve;
    ofParameter <ofVec3f> rotateCurve;
    ofParameter <bool> exporting;
    
    ofxPanel gui;

    ofSoundPlayer sndPlayer;
    
   
    ofImage backgroundPlate;
    
	cameraManager CM;
    

    
    cameraCalibrationManager CCM;
    
    
    // ---------------
    ofMesh prevFrame;
    ofVec3f decompTranslation, decompScale;
    ofQuaternion decompRotation, decompSo;
    
    
    bool bSaving;
    int savingFrame;
    
    depthExporterManager DEM;
    
    outerspace SPACE;
    
    
};

