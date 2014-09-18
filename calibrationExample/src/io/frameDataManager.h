

#pragma once

#include "ofMain.h"
#include "ofxBinaryMesh.h"

typedef struct {
    
    ofImage     img;        // person
    ofMesh      head;       // mesh
	ofMesh		leftEye;	// leftEye
	ofMesh		rightEye;	// rightEye
    ofImage     mask;       // mask image
    
} frameData;



class frameDataManager {
    
    
public:
    
    void setup( string footageDir, string obmDir );
//    void setup( string footageDir, vector<string> obmDir );
    void listDirs( string footageDir, string obmDir);
    void listDirs( string footageDir, vector<string> obmDir);

    
    string rootDirectory;
    int numFrames;
    
//  ofDirectory heads;
//	ofDirectory leftEyes;
//	ofDirectory rightEyes;
	
	vector<string> heads;
	vector<string> leftEyes;
	vector<string> rightEyes;
	
    ofDirectory videoImages;
    ofDirectory maskImages;

    int getNumFrames();
    void loadFrame( int frameNum, frameData & fd);
    void getOrientation(const frameData& fd, ofNode& n );

	//after loading, compute some things to get a stable mesh
	void calculateBaseEyeInfo();
	ofVec3f baseEyeCenter;
	ofVec3f baseEyeForward;
	ofIndexType eyeForwardIndex;
};



//void ofApp::listDirs() {
//    
//    string testSequenceFolder = dataPath + "jackie_002/";
//    
//    bgImages.listDir("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/Footage_360p_Proxy");
//    objs.listDir("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_Craig_003_trim_OBM/head");
//    maskImages.listDir("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_mask_360p");
//    
//    currentFrame = 1;
//    lastFrame = 1;
//    
//    loadFrame(currentFrame);
//}
