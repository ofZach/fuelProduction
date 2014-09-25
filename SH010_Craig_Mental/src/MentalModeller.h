#pragma once

#include "ofMain.h"
#include "ofxNearestNeighbour.h"

typedef struct {
	ofIndexType meshIndex;
	ofVec3f curPos;
	ofVec3f curNorm;
	ofVec3f extrudePos;
	ofVec3f originalPos;
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
	ofParameter <float> extraExtrusion;
	ofParameter <float> extraExtrusionSmooth;
	ofParameter <float> maxdistance;
	ofParameter <float> deleteChance;
	ofParameter <int> seed;
	
	void paramChanged(float& param);
	void paramChangedInt(int& param);
	
	vector<HeadParticle> headParticles;
	vector<ofIndexType> targetIndices;
	ofMesh baseMesh;
	ofMesh connectionMesh;
	ofxNearestNeighbour3D neighbors;
	
	set< pair<int,int> > headParticleConnections;
	

};

