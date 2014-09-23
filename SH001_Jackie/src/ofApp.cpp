#include "ofApp.h"
#include "ofxBinaryMesh.h"

using namespace Alembic::AbcGeom;

static string dataPath = "../../../sharedData/";


#ifndef NO_ALEMBIC
#include "ofxAlembic.h"
ofxAlembic::Reader abc;
#endif

void ofApp::setup() {
	
    drawFaceBox = false;
	
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(adjustments.set("adjustments", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(showWireframe.set("showWireframe", false));
    gui.add(showFilled.set("showFilled", false));
    gui.add(scaleFac.set("scaleFac", 1, 0.1, 2.0));
    gui.add(playback.set("playback", false));
    gui.add(playbackAudio.set("playbackAudio", false));
	gui.add(drawFaceBox.set("draw face box", false));

	gui.add(line.startPointX.set("start x",   0, -200,  200));
	gui.add(line.startPointY.set("start y",   0, -200,  200));
	gui.add(line.startPointZ.set("start z",-900, -700, -2000));

	gui.add(line.extrudeAmount.set("extrude", 200, 0, 1000));
	gui.add(line.arcRadius.set("arc radius", 200, 0, 400));
	gui.add(line.arcAngle.set("arc angle", 360, 180, 720));
	gui.add(line.twistDampen.set("twist damp", 50, 1, 200));
	gui.add(line.yParamGradient.set("param gradient", 200, -200, 200));
	
	gui.add(line.rotationAmount.set("rotation amount", 360, 0, 360*25));
	gui.add(line.resampleAmount.set("resample count", 100, 10, 1000));
	
	gui.add(line.chaseDampen.set("chase damp", .01, 0, .3));
	gui.add(line.maxNewAngle.set("max new", 45, 0, 90));
	
	gui.add(line.aRadius.set("a radius", 50, 0, 100));
	gui.add(line.bRadius.set("b radius", 50, 0, 100));


	gui.add(line.computeAttachmentPoints.set("generate hooks", false));
	gui.add(line.numAttachPoints.set("attach points", 5, 0, 50));


    gui.loadFromFile("adjustments.xml");
    
    //sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_1-2.aif");
    //sndPlayer.setLoop(true);
    //sndPlayer.setVolume(0);
    //sndPlayer.play();
    
	
    exporting = false;
	
//  shotManager.footageBasePath =  "/Users/zachlieberman/Desktop/GOLD_Footage";
	shotManager.footageBasePath =  "/Users/focus/Dropbox/+PopTech_Footage/";

	shotManager.setup();

	shotManager.loadShot("SH001", FDM); //jackie portrait
//	shotManager.loadShot("SH002", FDM); //craig portrait
//	shotManager.loadShot("SH003", FDM); //matt portrait
//	shotManager.loadShot("SH004", FDM); //craig scifi
//	shotManager.loadShot("SH005", FDM); //Jackie SMOG
//	shotManager.loadShot("SH006", FDM); //jackie "made fuel cells important to me"
//	shotManager.loadShot("SH007", FDM); //MATT "Innovative technology"
//	shotManager.loadShot("SH008", FDM); //JACKiE change the way communities
//	shotManager.loadShot("SH009", FDM); //JACKIE 'states, nations, the world'
//	shotManager.loadShot("SH010", FDM); //CRAIG mental models;
//	shotManager.loadShot("SH011", FDM); //CRAIG "we did it"

    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
    
    cout << FDM.numFrames << endl;
    
	ofSetVerticalSync(true);
	
	light.enable();
	light.setPosition(+500, +500, +500);
    
	string testSequenceFolder = dataPath + "aCam/";
    
	CCM.loadCalibration(testSequenceFolder + "matrices/rgbCalib.yml",
                        testSequenceFolder + "matrices/depthCalib.yml",
                        testSequenceFolder + "matrices/rotationDepthToRGB.yml",
                        testSequenceFolder + "matrices/translationDepthToRGB.yml");
	
    int ppWidth  = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width);
    int ppHeight = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height);
    
    
    targetFbo.allocate(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width,
                       CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGBA32F);

    CM.CCM = CCM;
	CM.setup();
    
    line.setup();
	line.generateArc(FDM.numFrames);

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
    }
	
	line.update(currentFrame);
    
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
	
	string path = "JACKIE_001_" + ofGetTimestampString() + ".abc";
    writer.open(path, 24);
}

void ofApp::writeFrame(){

	//writer.setTime(exportFrame / 23.976);
	
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
	   
}

void ofApp::draw(){
    
	targetFbo.begin();
    ofViewport(ofRectangle(0,0,1920, 1080));
	
	ofClear(100,100,100,0);
    glClear(GL_DEPTH);

	//BACKGROUND
    ofDisableDepthTest();
    //backgroundPlate.draw(0,0,1920,1080);
    ofEnableDepthTest();
	
	//LINE DEBUG
//	CM.cameraStart();
//    line.draw();
//	CM.cameraEnd();

	// rhonda draw
	ofPolyline p;
	for(int i = line.curCurve.getVertices().size()-1; i >= 0; i-- ){
		p.addVertex(line.curCurve.getVertices()[i]);
	}
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	ofEnableDepthTest();
	lineRenderer.draw(p, CM.baseCamera);
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	ofEnableAlphaBlending();
	ofDisableDepthTest();
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_MIN_EXT);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_CONSTANT_COLOR);
	
	lineRenderer.draw(p, CM.baseCamera);
	
	ofEnableDepthTest();
	ofEnableAlphaBlending();
	
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
    gui.draw();
    
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
	
	if(args.key == 'l'){
		line.generateArc(FDM.numFrames);
	}
    
	if(args.key == '\\'){
		startExport();
	}
    CM.keyPressed(args.key);
    
}
