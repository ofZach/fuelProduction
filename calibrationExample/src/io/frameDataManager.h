



#include "ofMain.h"
#include "ofxBinaryMesh.h"



typedef struct {
    
    ofImage     img;        // person
    ofMesh      mesh;       // mesh
    ofImage     mask;       // mask image
    
} frameData;



class frameDataManager {
    
    
public:
    
    void setup( string rootDir );
    void listDirs();
    
    string rootDirectory;
    int numFrames;
    
    int getNumFrames();
    
    void getFrameData( int frameNum, frameData & fd);
    
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
