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
	
	percentAlongCurve =  ofClamp(1.0 * ofGetMouseX() / ofGetWidth(), 0, 1.0);

	if(meshes.size() > 0){
		int meshIndex = percentAlongCurve * meshes.size();
		curMesh = meshes[meshIndex];
	}

	int index = percentAlongCurve * (ptf.framesSize()-1);
	a.setTransformMatrix(ptf.frameAt(index));
	a.rotate(rotationAmount*percentAlongCurve, a.getUpDir());
}

void LineManager::draw(){
	
	b.draw();

	curMesh.draw();
//	ofMesh m;
//	m.addVertices( linePoints );
//	m.setMode(OF_PRIMITIVE_LINE_STRIP);
//	m.draw();
	
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
	meshes.clear();
	
//	float angleStep = arcAngle / numF;
//	float curAngle = 0;
	
	ofVec3f startPoint(startPointX,startPointY,startPointZ);
	for(int i = 0; i < numFrames; i++){
		
		float percentDone = 1.0 * i / numFrames;
		float angle = percentDone * arcAngle;
		
		ofVec3f pos = startPoint + ofVec3f(0,-1,0).getRotated(angle, ofVec3f(0,0,1) ) * arcRadius;
		pos += ofVec3f(0,0, extrudeAmount * percentDone);

		ptf.addPoint(pos);

		basePoints.push_back( pos );
	}
	
	for(int i = 0; i < numFrames; i++){
		
		percentAlongCurve = 1.0 * i / numFrames;
		
		int index = percentAlongCurve * (numFrames-1);
		a.setTransformMatrix(ptf.frameAt(index));
		
		a.rotate(rotationAmount*percentAlongCurve, a.getUpDir());
		ofNode n;
		n.setTransformMatrix(b.getGlobalTransformMatrix());
		
		
		linePoints.push_back(n.getPosition());
		
		ofMesh m;
		m.setMode(OF_PRIMITIVE_LINE_STRIP);
		m.addVertices(linePoints);
		meshes.push_back(m);
		
	}
	
}

