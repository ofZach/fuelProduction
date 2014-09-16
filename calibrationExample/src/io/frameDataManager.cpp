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
    objs.listDir(rootDirectory + "SH002_Craig_003_trim_OBM/head");
    maskImages.listDir(rootDirectory + "SH002_mask_360p");

}


void frameDataManager::loadFrame( int frameNum, frameData & fd){
    if (frameNum < videoImages.size()){
        fd.img.loadImage(videoImages[frameNum]);
    }
    if (frameNum < objs.size()){
        ofxBinaryMesh::load(objs.getPath(frameNum), fd.mesh);
    }
}
