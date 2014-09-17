#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"
#include "frameDataManager.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{
public:
    
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(ofKeyEventArgs& args);
    void listDirs();
    
    frameDataManager FDM;
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
    ofxPanel gui;

	bool useSideCamera;

	ofCamera* currentCamera;
    ofCamera sideCam;
	ofCamera topCamera;
	ofCamera baseCamera;
    vector < ofCamera * > cameraPtrs;
    int cameraCounter;
//    

	ofEasyCam easyCam;


    cameraCalibrationManager CCM;
    
    
    // ---------------
    ofMesh prevFrame;
    ofVec3f decompTranslation, decompScale;
    ofQuaternion decompRotation, decompSo;
    
    
    bool bSaving;
    int savingFrame;
    
    
    
};
