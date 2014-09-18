#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"
#include "frameDataManager.h"
#include "shotManager.h"
#include "ofxGui.h"
#include "cameraManager.h"
#include "LineManager.h"

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
    
	LineManager line;
	cameraManager CM;
    
    cameraCalibrationManager CCM;

    int currentFrame;
    int lastFrame;

	ofLight light;
	
	ofFbo targetFbo;

	void drawMesh(ofMesh& m, ofFloatColor color);

	ofParameter <ofVec3f> adjustments;
	ofParameter <bool> showWireframe;
	ofParameter <bool> showFilled;
    ofParameter <float> scaleFac;
    ofParameter <bool> playback;
    ofParameter <bool> playbackAudio;
	ofParameter <bool> drawFaceBox;

	//line pos
    ofSoundPlayer sndPlayer;
    ofImage backgroundPlate;
    
    
    // ---------------
//    ofMesh prevFrame;
//    ofVec3f decompTranslation, decompScale;
//    ofQuaternion decompRotation, decompSo;
    
    
    bool bSaving;
    int savingFrame;
    
    
    
};

