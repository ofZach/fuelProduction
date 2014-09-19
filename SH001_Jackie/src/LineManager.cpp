#include "LineManager.h"

void LineManager::setup(){

	startPointX.addListener(this, &LineManager::paramChanged);;
	startPointY.addListener(this, &LineManager::paramChanged);
	startPointZ.addListener(this, &LineManager::paramChanged);
	
	extrudeAmount.addListener(this, &LineManager::paramChanged);
	arcRadius.addListener(this, &LineManager::paramChanged);
	arcAngle.addListener(this, &LineManager::paramChanged);

	percentAlongCurve = 0.0;
	
	currentRotation = 0.0;
	
	b.setParent(a);
}

void LineManager::update(){
	
	//percentAlongCurve = MIN(percentAlongCurve+.01, 1.0);
	percentAlongCurve =  1.0 * ofGetMouseX() / ofGetWidth();
	
	int index = percentAlongCurve * (ptf.framesSize()-1);
	a.setTransformMatrix(ptf.frameAt(index));
	
	//thisNode.setTransformMatrix();

//	ofVec3f pos = basePoints[index];
//	a.setPosition(pos);
	
//	ofVec3f nextPos = basePoints[index+1];
//	ofVec3f lookDir = nextPos - pos;

	
//	currentRotation += rotationsSpeed;
//	ofQuaternion curRotation;
}

void LineManager::draw(){
	
	a.draw();
	
}

void LineManager::drawArc(){
	ofMesh m;
	m.addVertices( basePoints );
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	m.draw();
	
	ptf.debugDraw(10.0);
	
}

void LineManager::paramChanged(float& param){
	generateArcPoints();
}

void LineManager::generateArcPoints(){
	
	basePoints.clear();
	ptf.clear();
	
	float angleStep = 5;
	float curAngle = 0;
	
	ofVec3f startPoint(startPointX,startPointY,startPointZ);
	for(float angle = 0; angle < arcAngle; angle += angleStep){
		ofVec3f pos = startPoint + ofVec3f(0,-1,0).getRotated(angle, ofVec3f(0,0,1) ) * arcRadius;
		float percentDone = angle / arcAngle;
		pos += ofVec3f(0,0,extrudeAmount * percentDone);

		ptf.addPoint(pos);

		basePoints.push_back( pos );
	}
	
	
}

