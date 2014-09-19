#include "LineManager.h"

void LineManager::setup(){

	startPointX.addListener(this, &LineManager::paramChanged);;
	startPointY.addListener(this, &LineManager::paramChanged);
	startPointZ.addListener(this, &LineManager::paramChanged);
	
	extrudeAmount.addListener(this, &LineManager::paramChanged);
	arcRadius.addListener(this, &LineManager::paramChanged);
	arcAngle.addListener(this, &LineManager::paramChanged);

	rotationAmount.addListener(this, &LineManager::paramChanged);
	
	percentAlongCurve = 0.0;
	
	currentRotation = 0.0;
	
	b.setParent(a);
	b.setPosition(0, 100, 0);
}

void LineManager::update(){
	
	//percentAlongCurve = MIN(percentAlongCurve+.01, 1.0);
	
	percentAlongCurve =  ofClamp(1.0 * ofGetMouseX() / ofGetWidth(), 0, 1.0);

	int index = percentAlongCurve * (ptf.framesSize()-1);
	a.setTransformMatrix(ptf.frameAt(index));
	
	a.rotate(rotationAmount*percentAlongCurve, a.getUpDir());

	
	//a.tilt(rotationAmount*percentAlongCurve);
	
	//thisNode.setTransformMatrix();

//	ofVec3f pos = basePoints[index];
//	a.setPosition(pos);
	
//	ofVec3f nextPos = basePoints[index+1];
//	ofVec3f lookDir = nextPos - pos;
	
//	currentRotation += rotationsSpeed;
//	ofQuaternion curRotation;
}

void LineManager::draw(){
	
	b.draw();
	
	ofMesh m;
	m.addVertices( linePoints );
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	m.draw();
	
}

void LineManager::drawArc(){
	
	ofMesh m;
	m.addVertices( basePoints );
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	m.draw();
	
	ptf.debugDraw(10.0);
	
}

void LineManager::paramChanged(float& param){
	generateLine(numFrames);
}

void LineManager::generateLine(int numF){
	
	numFrames = numF;
	
	basePoints.clear();
	linePoints.clear();
	ptf.clear();
	
	float angleStep = ;
	float curAngle = 0;
	
	ofVec3f startPoint(startPointX,startPointY,startPointZ);
	for(int i = 0; i < numFrames; i++){
		
		float percentDone = 1.0* i / numFrames;
		float angle = angle * arcRadius;
		
		ofVec3f pos = startPoint + ofVec3f(0,-1,0).getRotated(angle, ofVec3f(0,0,1) ) * arcRadius;
		pos += ofVec3f(0,0,extrudeAmount * percentDone);

		ptf.addPoint(pos);

		basePoints.push_back( pos );
	}
	
	for(int i = 0; i < numFrames; i++){
		percentAlongCurve =  ofClamp(1.0 * i / numFrames, 0, 1.0);
		
		int index = percentAlongCurve * (ptf.framesSize()-1);
		a.setTransformMatrix(ptf.frameAt(index));
		
		a.rotate(rotationAmount*percentAlongCurve, a.getUpDir());
		ofNode n;
		n.setTransformMatrix(b.getGlobalTransformMatrix());
		
		linePoints.push_back(n.getPosition());
		
		//cout << "percent " << percentAlongCurve << " a " << a.getPosition() << " b " << n.getPosition() << endl;
	}
	
}

