//
//  cameraCalibrationManager.cpp
//  faceShiftOffline
//
//  Created by zach on 9/12/14.
//
//

#include "cameraCalibrationManager.h"


bool cameraCalibrationManager::loadCalibration(string rgbIntrinsicsPath,
							string depthIntrinsicsPath,
							string rotationPath,
							string translationPath)
{
    
	if(!ofFile::doesFileExist(rgbIntrinsicsPath) ||
	   !ofFile::doesFileExist(depthIntrinsicsPath) ||
	   !ofFile::doesFileExist(rotationPath) ||
	   !ofFile::doesFileExist(translationPath))
	{
		ofLogError() << "ofxRGBDRenderer::setup -- Missing one or more matrix files! " << rgbIntrinsicsPath << " " <<
        depthIntrinsicsPath << " " <<
        rotationPath << " " <<
        translationPath;
		return false;
	}
	
	depthCalibration.load(depthIntrinsicsPath);
	rgbCalibration.load(rgbIntrinsicsPath);
    
	ofxCv::loadMat(rotationDepthToRGB, rotationPath);
	ofxCv::loadMat(translationDepthToRGB, translationPath);
    
	//	Point2d fov = depthCalibration.getUndistortedIntrinsics().getFov();
	//	fx = tanf(ofDegToRad(fov.x) / 2) * 2;
	//	fy = tanf(ofDegToRad(fov.y) / 2) * 2;
	//	fx = depthCalibration.getUndistortedIntrinsics().getCameraMatrix().at<double>(0,0);
	//	fy = depthCalibration.getUndistortedIntrinsics().getCameraMatrix().at<double>(1,1);
	//	principalPoint = depthCalibration.getUndistortedIntrinsics().getPrincipalPoint();
	//	imageSize = depthCalibration.getUndistortedIntrinsics().getImageSize();
    
	//intrinsics
	depthFOV.x = depthCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(0,0);
	depthFOV.y = depthCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(1,1);
	depthPrincipalPoint = ofxCv::toOf(depthCalibration.getDistortedIntrinsics().getPrincipalPoint());
	depthImageSize = ofRectangle(0, 0,
								 depthCalibration.getDistortedIntrinsics().getImageSize().width,
								 depthCalibration.getDistortedIntrinsics().getImageSize().height);
    
	colorFOV.x = rgbCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(0,0);
	colorFOV.y = rgbCalibration.getDistortedIntrinsics().getCameraMatrix().at<double>(1,1);
	colorPrincipalPoint = ofxCv::toOf( rgbCalibration.getDistortedIntrinsics().getPrincipalPoint() );
	colorImageSize = ofRectangle(0,0,
								 rgbCalibration.getDistortedIntrinsics().getImageSize().width,
								 rgbCalibration.getDistortedIntrinsics().getImageSize().height);
	
	//extrinsics
	depthToRGBTranslation = ofVec3f(translationDepthToRGB.at<double>(0,0),
									translationDepthToRGB.at<double>(1,0),
									translationDepthToRGB.at<double>(2,0));
    //	cout << "translation is " << depthToRGBTranslation << endl;
    //	depthToRGBTranslation /= 100;
    
	cv::Mat rx3;
    
	if(rotationDepthToRGB.rows == 3 && rotationDepthToRGB.cols == 3) {
		cout << "LOADING 3x3 ROTATION!" << endl;
		rotationDepthToRGB.copyTo(rx3);
		float rotation3fv[9] = {
			float(rx3.at<double>(0,0)),float(rx3.at<double>(1,0)),float(rx3.at<double>(2,0)),
			float(rx3.at<double>(0,1)),float(rx3.at<double>(1,1)),float(rx3.at<double>(2,1)),
			float(rx3.at<double>(0,2)),float(rx3.at<double>(1,2)),float(rx3.at<double>(2,2))
		};
		memcpy(depthToRGBRotation, rotation3fv, sizeof(float)*3*3);
	}
	else {
		//openFrameworkds needs a better Matrix3x3 class...
		cv::Rodrigues(rotationDepthToRGB, rx3);
		float rotation3fv[9] = {
			float(rx3.at<double>(0,0)),float(rx3.at<double>(1,0)),float(rx3.at<double>(2,0)),
			float(rx3.at<double>(0,1)),float(rx3.at<double>(1,1)),float(rx3.at<double>(2,1)),
			float(rx3.at<double>(0,2)),float(rx3.at<double>(1,2)),float(rx3.at<double>(2,2))
		};
		memcpy(depthToRGBRotation, rotation3fv, sizeof(float)*3*3);
	}
	
	ofMatrix4x4 rot;
    
	float mat4x4[16] = {
		depthToRGBRotation[0],depthToRGBRotation[1],depthToRGBRotation[2],0,
		depthToRGBRotation[3],depthToRGBRotation[4],depthToRGBRotation[5],0,
		depthToRGBRotation[6],depthToRGBRotation[7],depthToRGBRotation[8],0,
		0,0,0,1
	};
    
	extrinsics = ofMatrix4x4(mat4x4).getInverse();
	extrinsics.translate(depthToRGBTranslation);
	
    
	//windows seems to load these differently sometimes
	cv::Mat dis = rgbCalibration.getDistCoeffs();
	if(dis.cols == 1){
		distortionK = ofVec3f(dis.at<double>(0,0),
							  dis.at<double>(1,0),
							  dis.rows == 5 ? dis.at<double>(4,0) : 0);
		distortionP = ofVec2f(dis.at<double>(2,0),dis.at<double>(3,0));
	}
	else if(dis.rows == 1){
		distortionK = ofVec3f(dis.at<double>(0,0),
							  dis.at<double>(0,1),
							  dis.cols == 5 ? dis.at<double>(0,4) : 0);
		distortionP = ofVec2f(dis.at<double>(0,2),dis.at<double>(0,3));
	}
    
	return true;
}