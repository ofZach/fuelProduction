#include "MentalModeller.h"


//rotate head mesh
//outbound head particles shift along normal

MentalModeller::MentalModeller(){
	
}

void MentalModeller::setup(ofMesh& firstMesh){
	curRotateY = 0;
	
	seed.addListener(this, &MentalModeller::paramChangedInt);
	yPercent.addListener(this, &MentalModeller::paramChanged);
	startExtrusion.addListener(this, &MentalModeller::paramChanged);
	endExtrusion.addListener(this, &MentalModeller::paramChanged);
	extraExtrusion.addListener(this, &MentalModeller::paramChanged);
	extraExtrusionSmooth.addListener(this, &MentalModeller::paramChanged);
	deleteChance.addListener(this, &MentalModeller::paramChanged);
	
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
	
	//srand(seed);

	vector<ofVec3f> points;
	for(int i = 0; i < baseMesh.getVertices().size(); i++){
		if( ofMap(baseMesh.getVertices()[i].y, minY, maxY, 0, 1.0) > yPercent && ofRandomuf() > deleteChance){
			HeadParticle p;
			p.curNorm = baseMesh.getNormals()[i];
			p.meshIndex = i;
			p.originalPos = baseMesh.getVertices()[i];
			float thisExtrude = startExtrusion + ofNoise(i/extraExtrusionSmooth) * extraExtrusion;
			p.extrudePos = baseMesh.getVertices()[i] + baseMesh.getNormals()[i];
			points.push_back( p.extrudePos );
			headParticles.push_back(p);
		}
	}
	
	//cout << "head point size is " << points.size() << endl;
}

void MentalModeller::update(ofMesh& headMesh, int frame){
	
	///////////////
	///1 ..... CREATE HALO CLOUD POINTS
	///////////////
	
	curRotateY += rotateY;
	ofMatrix4x4 posRotater;
	posRotater.makeIdentityMatrix();

	posRotater.translate(-headMesh.getCentroid());
	posRotater.rotate(rotateY, 0, 1, 0);
	posRotater.rotate(rotateX, 1, 0, 0);
	posRotater.translate(headMesh.getCentroid());
	posRotater.scale(-scale,scale,scale);
	posRotater.translate(adjust);
	
	ofQuaternion normRotX,normRotY, normRot;
	normRotY.makeRotate(rotateY, 0, 1, 0);
	normRotX.makeRotate(rotateX, 1, 0, 0);
	normRot = normRotY * normRotX;

	float percentdone = 1.0 * frame/totalFrames;
	pointDebug.clear();
	for(int i = 0; i < headParticles.size(); i++){
		
		HeadParticle& p = headParticles[i];
		float thisExtrude = ofMap(powf(percentdone,3.0), 0, 1.0, startExtrusion, endExtrusion) + ofNoise(i/extraExtrusionSmooth) * extraExtrusion;
	 	ofVec3f lastPos = p.curPos;
		p.originalPos = headMesh.getVertex( p.meshIndex ) ;
		p.curNorm = headMesh.getNormal( p.meshIndex );
		ofVec3f newPos = posRotater.preMult( p.originalPos + p.curNorm * thisExtrude );
		p.curPos = newPos;
		
		p.originalPos = posRotater.preMult(p.originalPos);
		//p.curNorm = normRot * p.curNorm;
		
		pointDebug.addVertex(p.curPos);
	}
	pointDebug.setMode(OF_PRIMITIVE_POINTS);

	
	///////////////
	///2 ..... CREATE CONNECTIONS BETWEEN THEM
	///////////////
	
	neighorParticles.clear();
	headParticleConnections.clear();
	neighbors.buildIndex(pointDebug.getVertices());
	
	float maxDistance = ofMap(powf(percentdone,1.5), 0, 1.0, startMaxDistance, endMaxDistance);
	//connect closeset parts
	for(int i = 0; i < headParticles.size(); i++){
		vector<pair<size_t, float> > matches;
		neighbors.findPointsWithinRadius(headParticles[i].curPos, maxDistance, matches);
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

	connectionMesh.clear();
	set< pair<int,int> >::iterator it;
	for(it = headParticleConnections.begin(); it != headParticleConnections.end(); it++){
		connectionMesh.addVertex( headParticles[ it->first ].curPos );
		connectionMesh.addVertex( headParticles[ it->second].curPos );
	}
	connectionMesh.setMode(OF_PRIMITIVE_LINES);
	
	
	///////////////
	///3 ..... UPDATE CHASING PARTICLES
	///////////////
	
	//update chasers
	for(int i = chasers.size()-1; i >= 0; i--){
		LineChaser& chaser = chasers[i];
		if(chaser.startFrame > frame || chaser.endFrame < frame || chaser.startFrame == chaser.endFrame){
			if(deleteImmediatly || chaser.chase.getVertices().size() == 0){
				chasers.erase(chasers.begin()+i);
			}
			else{
				chaser.chase.getVertices().erase(chaser.chase.getVertices().begin());
			}
		}
		else{
			float chasePercent = ofMap(frame, chaser.startFrame, chaser.endFrame, 0, 1.0, true);
			chaser.chase.addVertex(  chaser.startPos.getInterpolated(chaser.endPos, chasePercent) );
		}
	}
	
	///////////////
	///4 ..... SPAWN NEW CHASER PARTICLES
	///////////////
	
	//create new chasers
	float chasersPerFrame = ofMap(powf(percentdone,1.5), 0, 1.0, startChasersPerFrame, endChasersPerFrame);
	if(chasersPerFrame < 1){
		int framesPerChaser = 1.0/chasersPerFrame;
		if(frame % framesPerChaser == 0){
			chasersPerFrame = 1;
		}
	}
	
	for(int i = 0; i < chasersPerFrame; i++){

		LineChaser chaser;
		chaser.startFrame = frame;
		chaser.endFrame = frame + chaserDuration;
		
//		cout << "chasing from " << chaser.startFrame << " " << chaser.endFrame << endl;
		
		int startIndex = ofRandom(headParticles.size());
		if( neighorParticles[startIndex].size() != 0){
			//random end neighbor
			int endIndex = neighorParticles[startIndex][ ofRandom(neighorParticles[startIndex].size()) ];
			if(ofRandomuf() < powf(laserChance, 2.0)){
				ofVec3f laserdDir = (headParticles[endIndex].curPos - headParticles[startIndex].originalPos).normalized();
				chaser.startPos = headParticles[startIndex].originalPos + laserdDir * laserStartOffset;
				chaser.endPos   = headParticles[endIndex].curPos + laserdDir * laserEndOffset;
				chaser.endFrame += laserLifeExtend;
			}
			else{
				chaser.startPos = headParticles[startIndex].curPos;
				chaser.endPos   = headParticles[endIndex].curPos;
			}

			if(popOn){
				for(int frame = chaser.startFrame; frame < chaser.endFrame; frame++){
					float chasePercent = ofMap(frame, chaser.startFrame, chaser.endFrame, 0, 1.0, true);
					chaser.chase.addVertex( chaser.startPos.getInterpolated(chaser.endPos, chasePercent) );
				}
				int numFrames = chaser.endFrame - chaser.startFrame;
				chaser.startFrame -= numFrames;
				chaser.endFrame   -= numFrames;
			}
			
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
