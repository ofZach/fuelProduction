#pragma once

#include "ofMain.h"

//draw arc

//sample arc around path
#include "ofxPtf.h"

class LineManager {
  public:
    
	void setup();
	void update();
	void draw();
	void drawArc();

	void paramChanged(float& param);
	
	//arc params
	ofParameter <float> startPointX;
	ofParameter <float> startPointY;
	ofParameter <float> startPointZ;
	
	ofParameter <float> extrudeAmount;
	ofParameter <float> arcRadius; //size of the arc
	ofParameter <float> arcAngle; //total angle

	ofParameter <float> rotationAmount; //total angle

	void generateLine(int numFrames);
	
	vector<ofVec3f> basePoints;
	vector<ofVec3f> linePoints;
	
	float percentAlongCurve;
	float currentRotation;
	
	ofxPtf ptf;
	int numFrames;
	
	//one per frame;
	vector<ofMesh> meshes;
	ofMesh curMesh;
	vector<ofVec3f> hooks; //distributed as we go;
	
	ofNode a;
	ofNode b;
    ofNode c;
};

