#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"
#include "frameDataManager.h"
#include "shotManager.h"
#include "ofxGui.h"
#include "cameraManager.h"
#include "LineManager.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include "ofxAlembic.h"
#include "rhondaLineRenderer.h"

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
	
    rhondaLineRenderer lineRenderer;
	LineManager line;
	cameraManager CM;
    
    cameraCalibrationManager CCM;

    int currentFrame;
    int lastFrame;

	ofLight light;
	ofxPanel gui;
	
	ofFbo targetFbo;

	void drawMesh(ofMesh& m, ofFloatColor color);

	ofParameter <ofVec3f> adjustments;
	ofParameter <bool> showWireframe;
	ofParameter <bool> showFilled;
    ofParameter <float> scaleFac;
    ofParameter <bool> playback;
    ofParameter <bool> playbackAudio;
	ofParameter <bool> drawFaceBox;
	ofParameter <bool> drawLineDebug;

	
	//line pos
    ofSoundPlayer sndPlayer;
    ofImage backgroundPlate;
    
    
    // ---------------
    ofMesh prevFrame;
    ofVec3f decompTranslation, decompScale;
    ofQuaternion decompRotation, decompSo;
    
    
	bool exporting;
	int exportFrame;
	void startExport();
	void writeFrame();
    ofxAlembic::Writer writer;
	
	
    
};

