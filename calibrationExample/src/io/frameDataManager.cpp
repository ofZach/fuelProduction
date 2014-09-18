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

    videoImages.listDir(footageDir);
    heads.listDir(obmDir + "/head");
    leftEyes.listDir(obmDir + "/leftEye");
    rightEyes.listDir(obmDir + "/rightEye");
    maskImages.listDir(footageDir);
    numFrames = videoImages.size();
    cout << "numeFrames " << numFrames << endl;
    
	//calculate base eye info
	if(rightEyes.size() > 0 && leftEyes.size() > 0){
		ofMesh baseLeftEye;
		ofMesh baseRightEye;

		ofxBinaryMesh::load(leftEyes.getPath(0), baseLeftEye);
		ofxBinaryMesh::load(rightEyes.getPath(0), baseRightEye);
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
        ofxBinaryMesh::load(heads.getPath(frameNum), fd.head);
    }

	if (frameNum < leftEyes.size()){
		ofxBinaryMesh::load(leftEyes.getPath(frameNum), fd.leftEye);
    }

	if (frameNum < rightEyes.size()){
		ofxBinaryMesh::load(rightEyes.getPath(frameNum), fd.rightEye);
    }
}
