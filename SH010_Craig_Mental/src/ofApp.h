#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "cameraCalibrationManager.h"
#include "frameDataManager.h"
#include "shotManager.h"
#include "ofxGui.h"
#include "cameraManager.h"
#include "MentalModeller.h"

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
    
	void drawVideoScene();
	void drawMesh(ofMesh& m, ofFloatColor color);

    frameDataManager FDM;
    shotManager shotManager;
    frameData frame;
    frameData firstFrame;
	
    rhondaLineRenderer lineRenderer;
	MentalModeller mentalModel;
	cameraManager CM;
    
    cameraCalibrationManager CCM;

    int currentFrame;
    int lastFrame;

	ofLight light;
	ofxPanel gui;
	
	ofFbo targetFbo;

	ofParameter <ofVec3f> adjustments;
    ofParameter <float> scaleFac;
	ofParameter <bool> showWireframe;
	ofParameter <bool> showFilled;
    ofParameter <bool> playback;
    ofParameter <bool> playbackAudio;
	ofParameter <bool> drawFaceBox;
	ofParameter <bool> drawDebug;

	//line pos
    ofSoundPlayer sndPlayer;
    ofImage backgroundPlate;
    
    // ---------------
//    ofMesh prevFrame;
//    ofVec3f decompTranslation, decompScale;
//    ofQuaternion decompRotation, decompSo;
    
	//export stuff
	bool exporting;
	int exportFrame;
	void startExport();
	void writeFrame();
    ofxAlembic::Writer writer;
	
	
    
};

