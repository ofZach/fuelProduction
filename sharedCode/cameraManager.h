

#include "ofMain.h"
#include "cameraCalibrationManager.h"



class cameraManager {
    
public: 
    
    void keyPressed (int key);
    
    void setup();
    void update();
    void cameraStart();
    void cameraEnd();

    void drawCameraInternals(ofImage &person, ofImage &mask, ofImage &backplate);
    
    bool useSideCamera;
	ofCamera* currentCamera;
    ofCamera sideCam;
	ofCamera topCamera;
	ofCamera baseCamera;
    vector < ofCamera * > cameraPtrs;
    int cameraCounter;
    ofEasyCam easyCam;
    
    cameraCalibrationManager CCM;
    ofShader shader;
    
    
    
    
    
};


