#pragma once

#include "ofMain.h"
//sample arc around path
#include "ofxPtf.h"

typedef struct {
	ofIndexType meshIndex;
	ofVec3f curPos;
} HeadParticle;

class MentalModeller {
  public:
	
	MentalModeller();
	
	void setup(ofMesh& firstMesh);
	void update(ofMesh& headMesh);
	void draw();
	void drawDebug();
	
	ofMesh pointDebug;

	void generateBaseParticles();
	
	ofParameter <float> yPercent;
	ofParameter <float> extrusion;
	
	void paramChanged(float& param);
	void paramChangedInt(int& param);

	vector<HeadParticle> headParticles;
	vector<ofIndexType> targetIndices;
	ofMesh baseMesh;
};

