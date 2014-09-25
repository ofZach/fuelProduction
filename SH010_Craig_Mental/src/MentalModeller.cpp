#include "MentalModeller.h"

MentalModeller::MentalModeller(){
	
}

void MentalModeller::setup(ofMesh& firstMesh){
	
	yPercent.addListener(this, &MentalModeller::paramChanged);
	maxdistance.addListener(this, &MentalModeller::paramChanged);
	extrusion.addListener(this, &MentalModeller::paramChanged);
	extraExtrusion.addListener(this, &MentalModeller::paramChanged);
	extraExtrusionSmooth.addListener(this, &MentalModeller::paramChanged);
	deleteChance.addListener(this, &MentalModeller::paramChanged);
	seed.addListener(this, &MentalModeller::paramChangedInt);
	
	baseMesh = firstMesh;
	generateBaseParticles();
}

void MentalModeller::paramChanged(float& param){
	generateBaseParticles();
}

void MentalModeller::paramChangedInt(int& param){
	generateBaseParticles();
	
}

void MentalModeller::generateBaseParticles(){
	
	if(baseMesh.getVertices().size() == 0){
		cout << "NO VERTS!" << endl;
	}
	
	float minY, maxY;
	minY = maxY = baseMesh.getVertices()[0].y;
	for(int i = 0; i < baseMesh.getVertices().size(); i++){
		minY = MIN(minY, baseMesh.getVertices()[i].y);
		maxY = MAX(maxY, baseMesh.getVertices()[i].y);
	}
	headParticles.clear();
	
	srand(seed);

	vector<ofVec3f> points;
	for(int i = 0; i < baseMesh.getVertices().size(); i++){
		if( ofMap(baseMesh.getVertices()[i].y, minY, maxY, 0, 1.0) > yPercent && ofRandomuf() > deleteChance){
			HeadParticle p;
			p.meshIndex = i;
			p.originalPos = baseMesh.getVertices()[i];
			float thisExtrude = extrusion + ofNoise(i/extraExtrusionSmooth) * extraExtrusion;
			p.extrudePos = baseMesh.getVertices()[i] + baseMesh.getNormals()[i];
			points.push_back( p.extrudePos );
			headParticles.push_back(p);
		}
	}
	
	//cout << "head point size is " << points.size() << endl;
}

void MentalModeller::update(ofMesh& headMesh, int frame){
	pointDebug.clear();
	for(int i = 0; i < headParticles.size(); i++){
		float thisExtrude = extrusion + ofNoise(i/extraExtrusionSmooth) * extraExtrusion;
		ofVec3f lastPos = headParticles[i].curPos;
		ofVec3f newPos = headMesh.getVertex( headParticles[i].meshIndex ) + headMesh.getNormal( headParticles[i].meshIndex ) * thisExtrude;
		headParticles[i].curPos = lastPos.getInterpolated(newPos, 1.0);
		pointDebug.addVertex(headParticles[i].curPos);
	}

	neighorParticles.clear();
	headParticleConnections.clear();
	neighbors.buildIndex(pointDebug.getVertices());
	
	//connect closeset parts
	for(int i = 0; i < headParticles.size(); i++){
		vector<pair<size_t, float> > matches;
		neighbors.findPointsWithinRadius(headParticles[i].curPos, maxdistance, matches);
		for(int m = 0; m < matches.size(); m++){
			pair<int,int> p		  = make_pair(i, matches[m].first);
			pair<int,int> reverse = make_pair(matches[m].first, i);
			if(headParticleConnections.find(reverse) == headParticleConnections.end()){

				headParticleConnections.insert(p);
				neighorParticles[p.first].push_back(p.second);
				neighorParticles[p.second].push_back(p.first);
				
				//cout << "inserting " << p.first << " < > " << p.second << endl;
			}
		}
	}

	pointDebug.setMode(OF_PRIMITIVE_POINTS);
	
	connectionMesh.clear();
	set< pair<int,int> >::iterator it;
	for(it = headParticleConnections.begin(); it != headParticleConnections.end(); it++){
//		cout << "found particle at position " << it->first << " second " << it->second << endl;
		connectionMesh.addVertex( headParticles[ it->first ].curPos );
		connectionMesh.addVertex( headParticles[ it->second].curPos );
	}
	
	connectionMesh.setMode(OF_PRIMITIVE_LINES);
	
	//update chasers
	for(int i = chasers.size()-1; i >= 0; i--){
		LineChaser& chaser = chasers[i];
		if(chaser.startFrame > frame || chaser.endFrame < frame || chaser.startFrame == chaser.endFrame){
			chasers.erase(chasers.begin()+i);
		}
		else{
			float chasePercent = ofMap(frame, chaser.startFrame, chaser.endFrame, 0, 1.0, true);
			chaser.chase.addVertex(  chaser.startPos.getInterpolated(chaser.endPos, chasePercent) );
		}
	}
	
	//create new chasers
	for(int i = 0; i < chasersPerFrame; i++){
		LineChaser chaser;
		chaser.startFrame = frame;
		chaser.endFrame = frame + chaserDuration;
		
//		cout << "chasing from " << chaser.startFrame << " " << chaser.endFrame << endl;
		
		int startIndex = ofRandom(headParticles.size());
		if( neighorParticles[startIndex].size() != 0){
			//random end neighbor
			int endIndex = neighorParticles[startIndex][ ofRandom(neighorParticles[startIndex].size()) ];
			if(ofRandomuf() > .95){
				chaser.startPos = headParticles[startIndex].originalPos;
			}
			else{
				chaser.startPos = headParticles[startIndex].curPos;
			}
			chaser.endPos   = headParticles[endIndex].curPos;
			chasers.push_back(chaser);
		}
	}
}

void MentalModeller::draw(){
	ofPushStyle();
	ofSetLineWidth(5);
	ofSetColor(0);
	for(int i = 0; i < chasers.size(); i++){
		chasers[i].chase.draw();
	}
	ofPopStyle();
}

void MentalModeller::drawDebug(){
	pointDebug.draw();
	connectionMesh.draw();
}
