#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"
#include "frameDataManager.h"



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
    

    int currentFrame;
    int lastFrame;
    

	ofLight light;
	
	ofFbo targetFbo;

	void drawMesh(ofMesh& m, ofFloatColor color);

	ofxUISuperCanvas* adjustGui;
	ofVec3f adjustments;
	int offsetShiftMillis;
	bool showWireframe;
	bool showFilled;
	bool showObjSequence;
	bool showBlendShape;
	float videoAlpha;

	bool useSideCamera;

	ofCamera* currentCamera;
    ofCamera sideCam;
	ofCamera topCamera;
	ofCamera baseCamera;

	ofEasyCam easyCam;


    cameraCalibrationManager CCM;
    
    
    // ---------------
    ofMesh prevFrame;
    ofVec3f decompTranslation, decompScale;
    ofQuaternion decompRotation, decompSo;
    
    
    bool bSaving;
    int savingFrame;
    
    
    
};

