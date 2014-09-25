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


typedef struct {
	ofPolyline chase;
	int startFrame;
	int endFrame;
	ofVec3f startPos;
	ofVec3f endPos;
} LineChaser;

class MentalModeller {
  public:
	
	MentalModeller();
	
	void setup(ofMesh& firstMesh);
	void update(ofMesh& headMesh, int frame);
	void draw();
	void drawDebug();
	
	ofMesh pointDebug;

	void generateBaseParticles();
	
	ofVec3f adjust;
	float scale;

	ofParameter <int> seed;
	ofParameter <float> yPercent;
	ofParameter <float> extrusion;
	ofParameter <float> extraExtrusion;
	ofParameter <float> extraExtrusionSmooth;
	ofParameter <float> rotateY;
	ofParameter <float> rotateX;

	ofParameter <float> maxdistance;
	ofParameter <float> deleteChance;
	ofParameter <bool> popOn;
	ofParameter <bool> deleteImmediatly;

	ofParameter <int> chasersPerFrame;
	ofParameter <int> chaserDuration;
	ofParameter <float> outboundLaserChance;
	ofParameter <float> outboundLaserStartOffset;
	ofParameter <float> outboundLaserEndOffset;
	
	
	void paramChanged(float& param);
	void paramChangedInt(int& param);
	float curRotateY;
	
	vector<HeadParticle> headParticles;
	vector<LineChaser> chasers;
	vector<ofIndexType> targetIndices;
	ofMesh baseMesh;
	ofMesh connectionMesh;
	ofxNearestNeighbour3D neighbors;
	
	set< pair<int,int> > headParticleConnections;
	map<int,vector<int> > neighorParticles;
	

};

