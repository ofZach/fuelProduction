//
//  frameDataManager.cpp
//  calibrationExample
//
//  Created by zach on 9/16/14.
//
//

#include "frameDataManager.h"




    
void frameDataManager::setup( string rootDir ){
    rootDirectory = rootDir;
    listDirs();
    numFrames = 0;
}

void frameDataManager::listDirs(){

    videoImages.listDir(rootDirectory + "Footage_360p_Proxy");
    heads.listDir(rootDirectory + "SH002_Craig_003_trim_OBM/head");
    leftEyes.listDir(rootDirectory + "SH002_Craig_003_trim_OBM/leftEye");
	rightEyes.listDir(rootDirectory + "SH002_Craig_003_trim_OBM/rightEye");

    maskImages.listDir(rootDirectory + "SH002_mask_360p");

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
