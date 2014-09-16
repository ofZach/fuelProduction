#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"


class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(ofKeyEventArgs& args);

 
    void listDirs();
    
    ofDirectory bgImages;
    ofDirectory objs;
    ofDirectory maskImages;
    
    int currentFrame;
    int lastFrame;
    
    ofImage currBg;
    ofMesh currMesh;
    //ofImage currFsImg;
    
    void loadFrame(int frame);
    

	ofEasyCam cam;
	ofCamera baseCamera;
	ofLight light;
	
	bool useEasyCam;
	ofVideoPlayer backdrop;
	ofFbo targetFbo;

	ofImage testOverlay;
	vector<ofMesh> meshes;
	int curMesh;
	void drawMesh(ofMesh& m, ofFloatColor color);

	ofxUISuperCanvas* adjustGui;
	ofVec3f adjustments;
	int offsetShiftMillis;
	bool showWireframe;
	bool showFilled;
	bool showObjSequence;
	bool showBlendShape;
	float videoAlpha;


    ofCamera cam3d;
    
    cameraCalibrationManager CCM;
    
    
    // ---------------
    ofMesh prevFrame;
    ofVec3f decompTranslation, decompScale;
    ofQuaternion decompRotation, decompSo;
    
    
    bool bSaving;
    int savingFrame;
    
    
    ofShader shader;
    
    
    ofFbo output;
    
    
    
    
};

