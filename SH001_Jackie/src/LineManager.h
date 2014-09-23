#pragma once

#include "ofMain.h"

//draw arc

//sample arc around path
#include "ofxPtf.h"
typedef struct{
	//ofVec3f pos;
	ofNode xform;
	float percentOnCurve;
	int startFrame;
	bool firstHook;
} AttachPoint;

class LineManager {
  public:
    
	void setup();
	void update(int curFrame);
	void draw();
	void drawArc();

	void paramChanged(float& param);
	//node chase params
	void generateArc(int numFrames);

	//arc params
	ofParameter <float> startPointX;
	ofParameter <float> startPointY;
	ofParameter <float> startPointZ;
	
	ofParameter <float> extrudeAmount;
	ofParameter <float> arcRadius; //size of the arc
	ofParameter <float> arcAngle; //total angle
	ofParameter <float> rotationAmount; //total angle
	ofParameter <float> resampleAmount; //uncurling

	ofParameter <float> chaseDampen;
	ofParameter <float> maxNewAngle;

	ofParameter <float> aRadius;
	ofParameter <float> bRadius;
	
	ofParameter <bool> computeAttachmentPoints;
	ofParameter <float> numAttachPoints;
	
	vector<ofVec3f> basePoints;
	
	ofxPtf ptf;
	int numFrames;
	int curFrame;
	vector<AttachPoint> hooks; //distributed as we go;
	ofMesh curMesh;
	ofPolyline curCurve;
	vector<AttachPoint> curHooks;
	bool reattachHooks; //when the params change
	ofNode a;
	ofNode b;
    ofNode c;
};

