#pragma once

#include "ofMain.h"

//draw arc

//sample arc around path

class LineManager {
  public:
    
	void setup();
	void update();
	void draw();
    
	void drawArc();

	
	void paramChanged(ofParameter<float>& param);
	
	//arc params
	ofParameter <float> startPointX;
	ofParameter <float> startPointY;
	ofParameter <float> startPointZ;
	
	ofParameter <float> extrudeAmount;
	ofParameter <float> arcRadius; //size of the arc
	ofParameter <float> arcAngle; //total angle
		
	void generateArcPoints();
	vector<ofVec3f> basePoints;
    
};

