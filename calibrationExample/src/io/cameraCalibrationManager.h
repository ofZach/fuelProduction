


#include "ofMain.h"
#include "ofxCv.h"

class cameraCalibrationManager{
    
    public:

    bool loadCalibration(string rgbIntrinsicsPath,
                                                   string depthIntrinsicsPath,
                                                   string rotationPath,
                                                   string translationPath);
    
    ofxCv::Calibration depthCalibration, rgbCalibration;
	cv::Mat rotationDepthToRGB, translationDepthToRGB;

    ofVec2f depthPrincipalPoint;
	ofVec2f depthFOV;
	ofRectangle depthImageSize;
	
	ofVec2f colorPrincipalPoint;
	ofVec2f colorFOV;
	ofRectangle colorImageSize;
    
	float depthToRGBRotation[9];
	ofVec3f depthToRGBTranslation;
	ofMatrix4x4 extrinsics;
	ofMatrix4x4 getAdjustedMatrix();
	
	ofVec3f distortionK;
	ofVec2f distortionP;
};
