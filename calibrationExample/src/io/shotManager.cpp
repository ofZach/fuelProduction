//
//  shotManager.cpp
//
//  Created by james on 9/16/14.
//
//

#include "shotManager.h"

void shotManager::setup(){
	//load all shots
	Shot* curshot;
	string footageWSlash = ofFilePath::addTrailingSlash(footageBasePath);

	//JACKIE PORTRAIT
	Shot sh001;
	curshot = &sh001;
	curshot->name = "SH001";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = true;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH001_Jackie_002_OBM" );
	shots[sh001.name] = sh001;

	//CRAIG PORTRAIT
	Shot sh002;
	curshot = &sh002;
	curshot->name = "SH002";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = true;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH002_Craig_003_OBM" );
	shots[sh002.name] = sh002;

	//MATT PORTRAIT
	Shot sh003;
	curshot = &sh003;
	curshot->name = "SH003";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = true;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH003_Matt_004_OBM" );
	shots[sh003.name] = sh003;

	//CRAIG SCIENCE FICTION
	Shot sh004;
	curshot = &sh004;
	curshot->name = "SH004";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = true;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH004a_Craig_003_OBM" );
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH004b_Craig_001_OBM" );
	shots[sh004.name] = sh004;

	//JACKIE SMOG
	Shot sh005;
	sh005.name = "SH005";
	sh005.footage2K = footageWSlash + sh005.name + "/Footage_fullsize_proxy";
	sh005.footageHD = footageWSlash + sh005.name + "/Footage_smallsize_proxy";
	sh005.footageSD = footageWSlash + sh005.name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	sh005.aCam = true;
	sh005.faceData.push_back( footageWSlash + sh005.name + "/SH005a_Jackie_001_OBM" );
	sh005.faceData.push_back( footageWSlash + sh005.name + "/SH005b_Jackie_001_OBM" );
	sh005.faceData.push_back( footageWSlash + sh005.name + "/SH005c_Jackie_001_OBM" );
	shots[sh005.name] = sh005;

	//JACKIE FUEL CELLS ARE IMPORTANT
	Shot sh006;
	curshot = &sh006;
	curshot->name = "SH006";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = false;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH006_Jackie_001_OBM" );
	shots[sh006.name] = sh006;
	
	//MATT INNOVATIVE TECHNOLOGY
	Shot sh007;
	sh007.name = "SH007";
	sh007.footage2K = footageWSlash + sh007.name + "/Footage_fullsize_proxy";
	sh007.footageHD = footageWSlash + sh007.name + "/Footage_smallsize_proxy";
	sh007.footageSD = footageWSlash + sh007.name + "/Footage_smallsize_proxy";			//TODO throw in the paths to the 360 proxies here
	sh007.aCam = true;
	sh007.faceData.push_back( footageWSlash + sh007.name + "/SH007a_Matt_002_OBM" );
	sh007.faceData.push_back( footageWSlash + sh007.name + "/SH007b_Matt_002_OBM" );
	shots[sh007.name] = sh007;

	//JACKIE
	Shot sh008;
	curshot = &sh008;
	curshot->name = "SH008";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = false;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH008_Jackie_001_OBM" );
	shots[sh008.name] = sh008;

	//JACKIE
	Shot sh009;
	curshot = &sh009;
	curshot->name = "SH009";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = true;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH009_Jackie_001_OBM" );
	shots[sh009.name] = sh009;
	
	//CRAIG "Mental models change"
	Shot sh010;
	sh010.name = "SH010";
	sh010.footage2K = footageWSlash + sh010.name + "/Footage_fullsize_proxy";
	sh010.footageHD = footageWSlash + sh010.name + "/Footage_smallsize_proxy";
	sh010.footageSD = footageWSlash + sh010.name + "/Footage_smallsize_proxy";	//TODO throw in the paths to the 360 proxies here
	sh010.aCam = false;
	sh010.faceData.push_back( footageWSlash + sh010.name + "/SH010a_Craig_001_OBM" );
	sh010.faceData.push_back( footageWSlash + sh010.name + "/SH010a_Craig_001_OBM" );
	shots[sh010.name] = sh010;

	//CRAIG "We did it"
	Shot sh011;
	curshot = &sh011;
	curshot->name = "SH011";
	curshot->footage2K = footageWSlash + curshot->name + "/Footage_fullsize_proxy";
	curshot->footageHD = footageWSlash + curshot->name + "/Footage_smallsize_proxy";
	curshot->footageSD = footageWSlash + curshot->name + "/Footage_360p_proxy";	//TODO throw in the paths to the 360 proxies here
	curshot->aCam = true;
	curshot->faceData.push_back( footageWSlash + curshot->name + "/SH011_Craig_001_OBM" );
	shots[sh011.name] = sh011;
	
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

