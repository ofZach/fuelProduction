//
//  shotManager.cpp
//
//  Created by james on 9/16/14.
//
//

#include "shotManager.h"

void shotManager::setup(){
	//load all shots
	/*
	 SH001
	 SH002
	 SH003
	 SH004
	 */
	string footageWSlash = ofFilePath::addTrailingSlash(footageBasePath);

	Shot sh005;
	sh005.name = "SH005";
	sh005.footage2K = footageWSlash + sh005.name + "/Footage_fullsize_proxy";
	sh005.footageHD = footageWSlash + sh005.name + "/Footage_smallsize_proxy";
	sh005.footageSD = footageWSlash + sh005.name + "/Footage_smallsize_proxy";	//ZACH throw in the paths to the 360 proxies here
	sh005.aCam = true;
	sh005.faceData.push_back( footageWSlash + sh005.name + "/SH005a_Jackie_001_OBM" );
	sh005.faceData.push_back( footageWSlash + sh005.name + "/SH005b_Jackie_001_OBM" );
	sh005.faceData.push_back( footageWSlash + sh005.name + "/SH005c_Jackie_001_OBM" );
	shots[sh005.name] = sh005;

	Shot sh007;
	sh007.name = "SH007";
	sh007.footage2K = footageWSlash + sh007.name + "/Footage_fullsize_proxy";
	sh007.footageHD = footageWSlash + sh007.name + "/Footage_smallsize_proxy";
	sh007.footageSD = footageWSlash + sh007.name + "/Footage_smallsize_proxy";	//ZACH throw in the paths to the 360 proxies here
	sh007.aCam = false;
	sh007.faceData.push_back( footageWSlash + sh007.name + "/SH007a_Matt_002_OBM" );
	sh007.faceData.push_back( footageWSlash + sh007.name + "/SH007b_Matt_002_OBM" );
	shots[sh007.name] = sh007;

	Shot sh010;
	sh010.name = "SH010";
	sh010.footage2K = footageWSlash + sh010.name + "/Footage_fullsize_proxy";
	sh010.footageHD = footageWSlash + sh010.name + "/Footage_smallsize_proxy";
	sh010.footageSD = footageWSlash + sh010.name + "/Footage_smallsize_proxy";	//ZACH throw in the paths to the 360 proxies here
	sh010.aCam = false;
	sh010.faceData.push_back( footageWSlash + sh010.name + "/SH010a_Craig_001_OBM" );
	sh010.faceData.push_back( footageWSlash + sh010.name + "/SH010a_Craig_001_OBM" );
	shots[sh010.name] = sh010;
	
	/*
	 SH006
	 SH007
	 SH008
	 SH009
	 SH010
	 SH011
	 */
	
}

void shotManager::loadShot(string name, frameDataManager& frameManager, bool lowRes){
	string footage;
	if(lowRes){
		footage = shots[name].footageSD;
	}
	else{
		footage = shots[name].footageHD;
	}
	
	cout << "name is " << name << " FOOATGE IS " << footage << endl;
	
	frameManager.listDirs(footage, shots[name].faceData);
	
}

