//
//  frameDataManager.cpp
//  calibrationExample
//
//  Created by zach on 9/16/14.
//
//

#include "frameDataManager.h"
    
void frameDataManager::setup( string footageDir, string obmDir ){
	numFrames = 0;
    listDirs( footageDir,  obmDir);
}

void frameDataManager::listDirs( string footageDir, string obmDir){
	vector<string> dirs;
	listDirs(footageDir, obmDir);
}

void frameDataManager::listDirs( string footageDir, vector<string> obmDir){
    
	videoImages.listDir(footageDir);
	
	heads.clear();
	leftEyes.clear();
	rightEyes.clear();
	
	int handleInFrames = 100;
	
	for(int i = 0; i < obmDir.size(); i++){
		
		//ignore handles except for the end
		int startOffset = (i == 0) ? 0 : handleInFrames;
		int endOffset   = (i == obmDir.size()-1) ? 0 : handleInFrames;
		
		startOffset = 0;
		endOffset   = 0;
		
		ofDirectory headsDir;
		headsDir.allowExt("obm");
		headsDir.listDir(obmDir[i] + "/head");
		for(int j = startOffset; j < headsDir.size()-endOffset; j++){
			heads.push_back(headsDir.getPath(j));
		}
		
		ofDirectory leftEyesDir;
		leftEyesDir.allowExt("obm");
		leftEyesDir.listDir(obmDir[i] + "/leftEye");
		for(int j = startOffset; j < leftEyesDir.size()-endOffset; j++){
			leftEyes.push_back(leftEyesDir.getPath(j));
		}
		
		ofDirectory rightEyesDir;
		rightEyesDir.listDir(obmDir[i] + "/rightEye");
		for(int j = startOffset; j < rightEyesDir.size()-endOffset; j++){
			rightEyes.push_back(rightEyesDir.getPath(j));
		}
	}
	
    maskImages.listDir(footageDir);
    numFrames = videoImages.size();
	
    cout << "numeFrames " << numFrames << endl;
    cout << "num objects " << heads.size();
	
	calculateBaseEyeInfo();
	
}

void frameDataManager::calculateBaseEyeInfo(){
	//calculate base eye info
	if(rightEyes.size() > 0 && leftEyes.size() > 0){
		ofMesh baseLeftEye;
		ofMesh baseRightEye;
		
		ofxBinaryMesh::load(leftEyes[0], baseLeftEye);
		ofxBinaryMesh::load(rightEyes[0], baseRightEye);
		baseEyeCenter = baseLeftEye.getCentroid().getInterpolated(baseRightEye.getCentroid(), .5);
		float maxZ = baseLeftEye.getVertices()[0].z;
		eyeForwardIndex = 0;
		for(int i = 0; i < baseLeftEye.getNumVertices(); i++){
			if(baseLeftEye.getVertices()[i].z < maxZ){
				maxZ = baseLeftEye.getVertices()[i].z;
				eyeForwardIndex = i;
			}
		}
		baseEyeForward = ofVec3f(baseLeftEye.getCentroid() - baseLeftEye.getVertices()[eyeForwardIndex]).getNormalized();
	}	
}

void frameDataManager::getOrientation(const frameData& fd, ofNode& n ){
	ofVec3f translation = fd.leftEye.getCentroid().getInterpolated(fd.rightEye.getCentroid(), .5);
	ofVec3f right = (fd.rightEye.getCentroid() - fd.leftEye.getCentroid()).getNormalized();
	ofVec3f forward = right.getCrossed(ofVec3f(0,1,0)).getNormalized();
	ofVec3f up = forward.getCrossed(right).getNormalized();
	n.setPosition(ofVec3f(0,0,0));
	n.lookAt(forward,up);
	n.setPosition(translation);
}

void frameDataManager::loadFrame( int frameNum, frameData & fd){
    if (frameNum < videoImages.size()){
        fd.img.loadImage(videoImages[frameNum]);
    }

    if (frameNum < heads.size()){
        ofxBinaryMesh::load(heads[frameNum], fd.head);
    }

	if (frameNum < leftEyes.size()){
		ofxBinaryMesh::load(leftEyes[frameNum], fd.leftEye);
    }

	if (frameNum < rightEyes.size()){
		ofxBinaryMesh::load(rightEyes[frameNum], fd.rightEye);
    }
}
