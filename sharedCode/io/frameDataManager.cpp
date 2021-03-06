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

void frameDataManager::listDirs( string footageDir, vector<string> obmDir, string mattes){
    
	
	heads.clear();
	leftEyes.clear();
	rightEyes.clear();
	
	int handleInFrames = 100;
	
	for(int i = 0; i < obmDir.size(); i++){
		
		//ignore handles except for the end
		int startOffset = (i == 0) ? 0 : handleInFrames;
		int endOffset   = (i == obmDir.size()-1) ? 0 : handleInFrames;

		//uncomment to RETAIN HANDLES
//		startOffset = 0;
//		endOffset   = 0;
//
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
	
	ofDirectory videoDirectory(footageDir);
	videoDirectory.allowExt("png");
	videoDirectory.allowExt("jpg");
	videoDirectory.listDir();
	
	vector<int> shotSwitches;
	string curShot = "a";
	bool checkForCut = videoDirectory.getName(0).find(curShot) != string::npos;
	for(int i = 0; i < videoDirectory.size(); i++){
		
		videoImages.push_back(videoDirectory.getPath(i));
		
		//find the internal handle midpoints when the shots switch
		string shotFileName = videoDirectory.getName(i);

		if(checkForCut && shotFileName.find(curShot) == string::npos){
			shotSwitches.push_back(i);
			if(curShot == "a"){
				curShot = "b";
			}
			else if(curShot == "b"){
				curShot = "c";
			}
		}
	}
	
	//delete the internal handles
	int handleframesRemoved = 0;
	for(int i = shotSwitches.size()-1; i >= 0; i--){
		int internalHandleStart = shotSwitches[i] - handleInFrames;
		int internalHandleEnd = shotSwitches[i] + handleInFrames;
		cout << "HANDLE [" << internalHandleStart << ", " << internalHandleEnd << "] out of " << videoImages.size() << " frames" << endl;
		for(int j = internalHandleEnd - 1; j >= internalHandleStart; j-- ){
			handleframesRemoved++;
			videoImages.erase(videoImages.begin() + j);
		}
	}
	
	cout << "REMOVED " << handleframesRemoved << " Handle Frames" << endl;
	numFrames = videoImages.size();
	
	if(mattes != "" && ofDirectory(mattes).exists()){
		maskImages.listDir(mattes);
		cout << "num masks " << maskImages.numFiles() << endl;
	}

    cout << "numeFrames " << numFrames << endl;
    cout << "num objects " << heads.size() << endl;;
	
	calculateBaseEyeInfo();
    
    maskStandIn.loadImage("../../../sharedData/maskStandIn/maskStandIn.png");
    
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
    
    if(frameNum < maskImages.size()){
		fd.mask.loadImage(maskImages.getPath(frameNum));
	}
    else{
		fd.mask.clone(maskStandIn);
	}
    
}
