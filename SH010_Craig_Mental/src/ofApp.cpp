#include "ofApp.h"
#include "ofxBinaryMesh.h"

using namespace Alembic::AbcGeom;

static string dataPath = "../../../sharedData/";


#ifndef NO_ALEMBIC
#include "ofxAlembic.h"
ofxAlembic::Reader abc;
#endif

void ofApp::setup() {
	
	
	ofSetVerticalSync(true);
	
	light.enable();
	light.setPosition(+500, +500, +500);

    drawFaceBox = false;
	exporting = false;
	
	//  shotManager.footageBasePath =  "/Users/zachlieberman/Desktop/GOLD_Footage";
	shotManager.footageBasePath =  "/Users/focus/Dropbox/+PopTech_Footage/";
	
	shotManager.setup();
	
//		shotManager.loadShot("SH001", FDM); //jackie portrait
	//	shotManager.loadShot("SH002", FDM); //craig portrait
	//	shotManager.loadShot("SH003", FDM); //matt portrait
	//	shotManager.loadShot("SH004", FDM); //craig scifi
	//	shotManager.loadShot("SH005", FDM); //Jackie SMOG
	//	shotManager.loadShot("SH006", FDM); //jackie "made fuel cells important to me"
	//	shotManager.loadShot("SH007", FDM); //MATT "Innovative technology"
	//	shotManager.loadShot("SH008", FDM); //JACKiE change the way communities
	//	shotManager.loadShot("SH009", FDM); //JACKIE 'states, nations, the world'
	shotManager.loadShot("SH010_v2", FDM); //CRAIG mental models;
	//	shotManager.loadShot("SH011", FDM); //CRAIG "we did it"
	
    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
//	FDM.getOrientation(frame, line.baseHeadNode);

    cout << FDM.numFrames << endl;

//    line.startFrame = 0;
//    line.endFrame = FDM.numFrames;
	
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(adjustments.set("adjustments", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(showWireframe.set("showWireframe", false));
    gui.add(showFilled.set("showFilled", false));
    gui.add(scaleFac.set("scaleFac", 1, 0.1, 2.0));
    gui.add(playback.set("playback", false));
    gui.add(playbackAudio.set("playbackAudio", false));
	gui.add(drawFaceBox.set("draw face box", false));
	gui.add(drawDebug.set("draw debug", false));
	
	gui.add(mentalModel.seed.set("seed", 0, 0, 500));
	gui.add(mentalModel.yPercent.set("y percent", .5, 0, 1.0));
	gui.add(mentalModel.deleteChance.set("point delete chance", .5, 0, 1.0));
	gui.add(mentalModel.startExtrusion.set("start extrusion", 100, 0, 500));
	gui.add(mentalModel.endExtrusion.set("end extrusion", 100, 0, 500));
	
	gui.add(mentalModel.extraExtrusion.set("xtra extrusion", 0, 0, 200));
	gui.add(mentalModel.extraExtrusionSmooth.set("xtra extrusion smooth", 50, 1.0, 100));
	gui.add(mentalModel.rotateY.set("rotate Y", 0, -90, 90));
	gui.add(mentalModel.rotateX.set("rotate X", 0, -90, 90));
	
	gui.add(mentalModel.maxdistance.set("max distance", 100, 0, 500));
	gui.add(mentalModel.chasersPerFrame.set("chasers per frame", 0, 0, 100));
	gui.add(mentalModel.chaserDuration.set("chase duration", 0, 0, 100));
	gui.add(mentalModel.popOn.set("pop on", false));
	gui.add(mentalModel.deleteImmediatly.set("delete immediatly", false));
	
	gui.add(mentalModel.laserChance.set("laser chance", .05, 0.0, .3));
	gui.add(mentalModel.laserStartOffset.set("laser start offset", 50, 0, 200));
	gui.add(mentalModel.laserEndOffset.set("laser end offset", 50, 0, 200));
	gui.add(mentalModel.laserLifeExtend.set("laser life extend", 0, 0, 30));

    gui.loadFromFile("adjustments.xml");
    
    //sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_1-2.aif");
    //sndPlayer.setLoop(true);
    //sndPlayer.setVolume(0);
    //sndPlayer.play();
    
	string testSequenceFolder = dataPath + "aCam/";
    
	CCM.loadCalibration(testSequenceFolder + "matrices/rgbCalib.yml",
                        testSequenceFolder + "matrices/depthCalib.yml",
                        testSequenceFolder + "matrices/rotationDepthToRGB.yml",
                        testSequenceFolder + "matrices/translationDepthToRGB.yml");
	
    int ppWidth  = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width);
    int ppHeight = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height);
    
    
    targetFbo.allocate(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width,
                       CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGB32F);

    CM.CCM = CCM;
	CM.setup();

	mentalModel.totalFrames = FDM.numFrames;
    mentalModel.setup(frame.head);

	lineRenderer.setup();
	lineRenderer.fakeDepthAdder = 0.018;
	
    backgroundPlate.loadImage(dataPath + "Background Plates/A-Cam_BackgroundPlate_360p.png");
    
}

void ofApp::update() {
    
    CM.update();

	if(playback){
		currentFrame = ofGetFrameNum() % FDM.getNumFrames();
	}
	else if(exporting){
		currentFrame = exportFrame;
	}
	else{
		currentFrame = ofClamp(mouseX, 0, FDM.numFrames-1);
	}
		
    if (lastFrame != currentFrame){
        FDM.loadFrame(currentFrame, frame);
//		FDM.getOrientation(frame, line.currentHeadNode);

    }
	mentalModel.adjust = adjustments;
	mentalModel.scale  = scaleFac;

	mentalModel.update(frame.head, currentFrame);
	
	
	lastFrame = currentFrame;

	if(exporting){
		writeFrame();
		exportFrame++;
		cout << "WRITING FRAME " << exportFrame << "/" << FDM.numFrames << endl;
		if(exportFrame == FDM.numFrames){
			cout << "DONE EXPORTING" << endl;
			writer.close();
			exporting = false;
		}
	}
}

void ofApp::startExport(){
	exporting = true;
	exportFrame = 0;
	
	string path = "CRAIG_010_" + ofGetTimestampString() + ".abc";
    writer.open(path, 24);
}

void ofApp::writeFrame(){

	//writer.setTime(exportFrame / 23.976);
	
	/*
	ofxAlembic::Curves curve;
	curve.curves.push_back(line.curCurve);
	writer.addCurves("/curve", curve);
	
	ofxAlembic::Points points;
	for(int i = 0; i < line.curHooks.size(); i++){

		string pathName = "/hooks_" + ofToString(i);
		ofxAlembic::XForm xform(line.curHooks[i].xform.getGlobalTransformMatrix());
		writer.addXform(pathName, xform);
		
		if(exportFrame == 0){
			ofBoxPrimitive b;
			b.setTransformMatrix(line.curHooks[i].xform.getGlobalTransformMatrix());
			ofxAlembic::PolyMesh p(b.getMesh());
			writer.addPolyMesh(pathName + "/shape", p);
		}
	}
	 */
}

void ofApp::draw(){
    
	targetFbo.begin();
    ofViewport(ofRectangle(0,0,1920, 1080));
	
	ofClear(100,100,100,255);
    glClear(GL_DEPTH);

	//BACKGROUND
    ofDisableDepthTest();
    backgroundPlate.draw(0,0,1920,1080);
    ofEnableDepthTest();
	
	//LINE DEBUG
//	if(true || drawDebug){
	
	CM.cameraStart();
	
	ofPushMatrix();
//	ofScale(-scaleFac,scaleFac,scaleFac);
//	ofTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
	if(drawDebug){
		mentalModel.drawDebug();
	}
	mentalModel.draw();
	
//	ofPopMatrix();
	
	CM.cameraEnd();
	
//	}
	
	// rhonda draw
	/*
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	ofEnableDepthTest();
	lineRenderer.draw(p, CM.baseCamera);
	 
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	ofEnableAlphaBlending();
	ofDisableDepthTest();
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_MIN_EXT);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_CONSTANT_COLOR);

	mentalModel.draw();
	lineRenderer.draw(p, CM.baseCamera);
	
	ofEnableDepthTest();
	ofEnableAlphaBlending();
	*/
	
	drawVideoScene();
	gui.draw();
    
}

void ofApp::drawVideoScene(){
	CM.cameraStart();
    
    CM.drawCameraInternals(frame.img, frame.mask, backgroundPlate);
	
    ofPushMatrix();
	ofScale(-scaleFac,scaleFac,scaleFac);
	ofTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
	
	drawMesh(frame.head, ofColor::darkGoldenRod);
	drawMesh(frame.rightEye, ofColor::red);
	drawMesh(frame.leftEye, ofColor::blue);
	
	ofPushStyle();
	
	ofNoFill();
	ofNode n;
	FDM.getOrientation(frame, n);
	n.draw();
	ofSetColor(255);
	ofMatrix4x4 mat = n.getGlobalTransformMatrix();
	ofMultMatrix(mat);
	if(drawFaceBox){
		ofBoxPrimitive(100, 100, 100).draw();
	}
	
	ofPopStyle();
	ofPopMatrix();
	
    ofSetColor(ofColor::white);
	
	////////////////
    
	ofDisableDepthTest();
    CM.cameraEnd();
    ofEnableAlphaBlending();
	targetFbo.end();
    targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);
	
}

void ofApp::drawMesh(ofMesh& m, ofFloatColor color){
	if(showFilled){
		ofSetColor(color);
		ofEnableDepthTest();
		ofEnableLighting();
		m.draw();
	}
	if(showWireframe){
		ofDisableLighting();
		ofSetColor(0);
		glDepthFunc(GL_LEQUAL);
		m.drawWireframe();
	}
	glDepthFunc(GL_LESS);
    ofDisableLighting();
}

void ofApp::exit(){
}

void ofApp::keyPressed(ofKeyEventArgs& args){
    
    if (args.key == 's'){
        gui.saveToFile("adjustments.xml");
    }
	
//	if(args.key == 'l'){
//		line.generateArc();
//	}
    
	if(args.key == '\\'){
		startExport();
	}
	
    CM.keyPressed(args.key);
    
}
