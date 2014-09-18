#include "LineManager.h"

void LineManager::setup(){
//	zDepth = 0;
//	xDistance = 0;
//	centerX = 0;;
//	topY = 0;

	startPointX.addListener(this, &LineManager::paramChanged);
}

void LineManager::update(){
	
}

void LineManager::draw(){

	
}

void LineManager::drawArc(){
	ofMesh m;
	m.addVertices( basePoints );
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	m.draw();
	
}

void LineManager::paramChanged(float& param){

}

void LineManager::generateArcPoints(){
	
	basePoints.clear();
	
	float angleStep = 5;
	float curAngle = 0;
	
	ofVec3f startPoint(startPointX,startPointY,startPointZ);
	for(float angle = 0; angle < arcAngle; angle += angleStep){
		ofVec3f pos = startPoint + ofVec3f(0,-1,0).getRotated(angle, ofVec3f(0,0,1) ) * arcRadius;
		float percentDone = angle / arcAngle;
		pos += ofVec3f(0,0,extrudeAmount * percentDone);
		basePoints.push_back( pos );
	}
	
	
}

