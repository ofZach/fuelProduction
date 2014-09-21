#include "ofApp.h"
#include "ofxBinaryMesh.h"
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include "ofxAlembic.h"

using namespace Alembic::AbcGeom;

static string dataPath = "../../../sharedData/";


//void listDirs();



#ifndef NO_ALEMBIC
#include "ofxAlembic.h"
ofxAlembic::Reader abc;
#endif

void ofApp::setup() {
	
    
    DEM.setup();
    
       
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(adjustments.set("adjustments", ofPoint(0, 0, 0), -ofPoint(200,200,200), ofPoint(200,200,200)));
    gui.add(showWireframe.set("showWireframe", false));
    gui.add(showFilled.set("showFilled", false));
    gui.add(scaleFac.set("scaleFac", 1, 0.1, 2.0));
    gui.add(playback.set("playback", false));
    gui.add(playbackAudio.set("playbackAudio", false));
    gui.add(exporting.set("exporting", false));
    gui.loadFromFile("adjustments.xml");
    
    
    sndPlayer.loadSound("/Users/zachlieberman/Dropbox/+PopTech_Toyota_Footage/SH002_Craig_test/SH002_1-2.aif");
    sndPlayer.setLoop(true);
    sndPlayer.setVolume(0);
    sndPlayer.play();
    

    bSaving = false;
   
    shotManager.footageBasePath =  "/Users/zachlieberman/Desktop/GOLD_Footage";
    shotManager.setup();
    shotManager.loadShot("SH002", FDM); //jackie portrait
	

    
    FDM.loadFrame(0, frame);            // load frame 0
    FDM.loadFrame(0, firstFrame);
    
    
	ofSetVerticalSync(true);
	
    
	light.enable();
	light.setPosition(+500, +500, +500);
    
	string testSequenceFolder = dataPath + "aCam/";
    
	CCM.loadCalibration(
                        testSequenceFolder + "matrices/rgbCalib.yml",
                        testSequenceFolder + "matrices/depthCalib.yml",
                        testSequenceFolder + "matrices/rotationDepthToRGB.yml",
                        testSequenceFolder + "matrices/translationDepthToRGB.yml");
	
    int ppWidth = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width);
    int ppHeight = ofNextPow2(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height);
    
    
    targetFbo.allocate(CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().width,
                       CCM.rgbCalibration.getDistortedIntrinsics().getImageSize().height,GL_RGBA32F);

   
    
    CM.CCM = CCM;
	CM.setup();
    
    

//    // save head and tranform
//    
//    //CCM.extrinsics
//    
////    ofxAlembic::Writer writer;
////    string path = "cam.abc";
////    writer.open(path, 24);
////
////    writer.addXform("/box", CCM.extrinsics);
////    //if (j == 0){
////        ofBoxPrimitive box;
////        box.set(40);
////        writer.addPolyMesh("/box/boxShape", box.getMesh());
////    //}
////
////        // draw the box of orientation using new alexmbic style
////        
////        
////    //}
//    
//    
//    
//    writer.close();
    
    
    abc.open("SH04_Spline_01c.abc");
    abc.dumpNames();
    
    
    backgroundPlate.loadImage("../../../sharedData/Background Plates/A-Cam_BackgroundPlate_360p.png");

    
//    ofxAlembic::Writer writer;
//    
//    
//    string path = ofGetTimestampString() + ".abc";
//    writer.open(path, 24);
//    /Users/zachlieberman/Desktop/of_v0.8.3_osx_release/apps/fuelProduction/sharedData/Background Plates/A-Cam_BackgroundPlate_360p.png
//    for (int j = 0; j < FDM.numFrames; j++){
//        
//        FDM.loadFrame(j, frame);            // load frame 0
//        ofMatrix4x4 mm;
//        mm.glScale(-scaleFac,scaleFac,scaleFac);
//        mm.glTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
//
//        ofMesh temp = frame.head;
//        cout << temp.getNumVertices() << endl;
//        ofxAlembic::transform(temp, mm);
//        writer.addPolyMesh("/head", temp);
//
//        ofNode n;
//        FDM.getOrientation(frame, n);
//
//        ofMatrix4x4 mat = n.getGlobalTransformMatrix();
//        ofMatrix4x4 newMat =  mat * mm;
//        newMat.glScale(-1, 1,1);
//        writer.addXform("/box", newMat);
//        if (j == 0){
//            ofBoxPrimitive box;
//            box.set(40);
//            writer.addPolyMesh("/box/boxShape", box.getMesh());
//        }
//        
//        // draw the box of orientation using new alexmbic style
//        
//        
//    }
//    
//    writer.close();
//
 
    
    
    
    
}


bool bExportingLastFrame = false;
int exportingFrameCount = 0;
ofxAlembic::Writer outputWriter;


void ofApp::update() {
    
    
    if (exporting){
        
        if (bExportingLastFrame == false){
            exportingFrameCount = 0;
            
            string path = "outputTest.abc";
            outputWriter.open(path);
            
            ////    string path = "cam.abc";
            ////    writer.open(path, 24);
            
        } else {
            
            exportingFrameCount++;
        }
        
        if (currentFrame > FDM.numFrames){
            exporting = false;
        }
        
        
        currentFrame = exportingFrameCount;
        if (lastFrame != currentFrame){
            FDM.loadFrame(currentFrame, frame);
            
        }
    }

    
    if (!exporting){
        
        if (bExportingLastFrame){
            outputWriter.close();
        }
            
        
        
        if (!playback){
            currentFrame = mouseX;
        } else {
            
            float time = sndPlayer.getPositionMS() / 1000.0;
            if (playbackAudio){
                sndPlayer.setVolume(1);
            } else {
                sndPlayer.setVolume(0);
            }
            
            //float totalTime = FDM.numFrames / 24.0;
            //float t = ofGetElapsedTimef();
            //while (t > totalTime) t -= totalTime;
            currentFrame = (int)(time * 24.0);
            
        }
        if (lastFrame != currentFrame){
            FDM.loadFrame(currentFrame, frame);
            
        }
    }
    lastFrame = currentFrame;
    
    
    
    CM.update();
    
    bExportingLastFrame = exporting;
    
    
    //----------------
    
}


vector < vector < ofPoint > > getTangentLines(double x1, double y1, double r1, double x2, double y2, double r2) {
    
    
    vector < vector < ofPoint > > pts;
    
    double d_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    
    //cout << d_sq << " " << (r1-r2)*(r1-r2) << endl;
    
    
    //if (d_sq <= (r1-r2)*(r1-r2)) return pts;
    
    double d = sqrt(d_sq);
    double vx = (x2 - x1) / d;
    double vy = (y2 - y1) / d;
    
    pts.resize(4);
    for (int i = 0; i < 4; i++){
        pts[i].resize(2);
    }
    
    int i = 0;
    
    // Let A, B be the centers, and C, D be points at which the tangent
    // touches first and second circle, and n be the normal vector to it.
    //
    // We have the system:
    //   n * n = 1          (n is a unit vector)
    //   C = A + r1 * n
    //   D = B +/- r2 * n
    //   n * CD = 0         (common orthogonality)
    //
    // n * CD = n * (AB +/- r2*n - r1*n) = AB*n - (r1 -/+ r2) = 0,  <=>
    // AB * n = (r1 -/+ r2), <=>
    // v * n = (r1 -/+ r2) / d,  where v = AB/|AB| = AB/d
    // This is a linear equation in unknown vector n.
    
    for (int sign1 = +1; sign1 >= -1; sign1 -= 2) {
        double c = (r1 - sign1 * r2) / d;
        
        // Now we're just intersecting a line with a circle: v*n=c, n*n=1
        
        if (c*c > 1.0) continue;
        double h = sqrt(max(0.0, 1.0 - c*c));
        
        for (int sign2 = +1; sign2 >= -1; sign2 -= 2) {
            double nx = vx * c - sign2 * h * vy;
            double ny = vy * c + sign2 * h * vx;
            int a = i++;
            
            //cout << a << " " << i << endl;
            //double[] a = res[i++];
            pts[a][0].x = x1 + r1 * nx;
            pts[a][0].y = y1 + r1 * ny;
            pts[a][1].x = x2 + sign1 * r2 * nx;
            pts[a][1].y = y2 + sign1 * r2 * ny;
        }
    }
    
    return pts;
    
}


int clockwiseFromPt( ofPoint a, ofPoint b, ofPoint c){
    
    ofPoint dA = a - c;
    ofPoint dB = b - c;
    
    
    
    float angle = atan2 (dA.y, dA.x);
    float angle2 = atan2 (dB.y, dB.x);
    
    float angleD = angle2 - angle;
    while (angleD > PI) angleD -= TWO_PI;
    while (angleD < -PI) angleD += TWO_PI;
    //cout << angleD << endl;
    
    return angleD < 0 ? -1 : 1;
    
    
}

int sideOfLine( ofPoint A, ofPoint B, ofPoint m){
    return ( (B.x-A.x)*(m.y-A.y) - (B.y-A.y)*(m.x-A.x) ) > 0 ? 1 : -1;
}



class circleInSpace{
public:
    ofPoint pt;
    float size;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ofPolyline lineFromCircle( vector < circleInSpace > circlesToTrack ){

    ofPolyline line;

    vector < ofPoint > circles;
    vector < float > sizes;

    for (int i = 0; i < circlesToTrack.size(); i++){
        
        circles.push_back(circlesToTrack[i].pt);
        sizes.push_back(circlesToTrack[i].size);
    }

    int clockWise = -1;

    ofPoint lastLineA;
    ofPoint lastLineB;

    for (int i = 0; i < circles.size()-2; i++){
    

    vector < vector<ofPoint> > pts = getTangentLines(circles[i].x,circles[i].y,sizes[i],
                                                     circles[i+1].x,circles[i+1].y,sizes[i+1]);
    
    if (pts.size() == 0) continue;
    
    int rightOrLeft = sideOfLine(circles[i], circles[i+2], circles[i+1]);
    
    
    ofPoint lineCircle = circles[i+2] - circles[i];
    
    lineCircle.normalize();
    
    vector < vector<ofPoint> > goodPts;
    
    for (int j = 0; j < pts.size(); j++){
        
        
        ofPoint a = circles[i+1] - lineCircle*1000.0;
        ofPoint b = circles[i+1] + lineCircle*1000.0;
        ofPoint c = pts[j][1];
        if ( sideOfLine(a,b,c) == rightOrLeft){
            
            // let's compare this clockwise, vs the prev clockwise
            
            ofPoint lastLineNormalized = lastLineB - lastLineA;
            lastLineNormalized.normalize();
            
            ofPoint thisLineNormalized = pts[j][1] - pts[j][0];
            thisLineNormalized.normalize();
            
            if (true){
                
                
                int  clock = clockwiseFromPt( lastLineB, lastLineB + lastLineNormalized, circles[i]) ;
                int clock2 = clockwiseFromPt( pts[j][0], pts[j][0] + thisLineNormalized, circles[i]);
                
                
                if (clock2 == clock){
                    
                    if (pts[j][0].distance(ofPoint(0,0,0)) > 1 &&pts[j][1].distance(ofPoint(0,0,0)) > 1){
                        
                        goodPts.push_back(pts[j]);
                    }
                }
            }
        }
    }
    
    
        
        if (goodPts.size() > 0){
            int whichOne = (int)ofRandom(100) % goodPts.size();
            
            if (goodPts[whichOne][0].distance(ofPoint(0,0,0)) > 1 &&
                goodPts[whichOne][1].distance(ofPoint(0,0,0)) > 1){
                
                
                
                line.addVertex(goodPts[whichOne][0]);
                line.addVertex(goodPts[whichOne][1]);
                lastLineA = goodPts[whichOne][0];
                lastLineB = goodPts[whichOne][1];
            }
            
        } else {
            if (pts.size() > 0){
                int whichOne = (int)ofRandom(100) % pts.size();
                
                if (pts[whichOne][0].distance(ofPoint(0,0,0)) > 1 &&
                    pts[whichOne][1].distance(ofPoint(0,0,0)) > 1){
                    
                    line.addVertex(pts[whichOne][0]);
                    line.addVertex(pts[whichOne][1]);
                    lastLineA = pts[whichOne][0];
                    lastLineB = pts[whichOne][1];
                }
                
            }
        }
    }
    
    return line;
}

//-----------------------------------------------------------------------------



void ofApp::draw(){
    
    
#ifndef NO_ALEMBIC
    float t = currentFrame / 24.0;
    if (t > abc.getMaxTime()){
        t = abc.getMaxTime();
    }
    abc.setTime(t);
#endif
    
	//DEM.startDraw();
    
    targetFbo.begin();
    
    ofViewport(ofRectangle(0,0,1920, 1080));
	ofClear(0,0,0,0);
    glClear(GL_DEPTH);
    
    

    
    
    CM.cameraStart();
    
    
    
    
    if (!exporting){
        CM.drawCameraInternals(frame.img, frame.mask, backgroundPlate);
    }
   
    
    
    
    ofPolyline curve;
    
#ifndef NO_ALEMBIC
    vector<ofPolyline> curvesMe;
    abc.get("SplineSpline", curvesMe);
    ofPushMatrix();
    //ofTranslate(ofPoint(0,0,-1095.244));
    for (int i = 0; i< curvesMe.size(); i++){
    //    curvesMe[i].draw();
    }
    curve = curvesMe[0];
    ofPopMatrix();
#endif
    
    
    curve = curve.getSmoothed(11);
    
    ofSetColor(ofColor::aquamarine);
    //curve.draw();
    
    //-----------------------------------------------------------------
    
    //cout << mouseY << endl;
    ofSeedRandom(526);
    
    
    float tf = currentFrame * (1.0 / 24.0);
    
    vector < circleInSpace > circles;
    int nCircles = ofRandom(20,40);
    for (int i = 0; i < nCircles; i++){
        circleInSpace cc;
        
        float pct = ofMap(i,0, nCircles, 0-PI/4,  PI + PI/4);
        ofPoint base( -50 + 300 * cos(pct), -50 + 300 * sin(pct));
        
        cc.pt.set( base.x + ofRandom(-100,100), base.y + ofRandom(-100,100));
        cc.size =  15 + sin(tf + i/2.0) * 10;
        if (ofRandom(0,1) > 0.85) cc.size = ofRandom(20,50);
        circles.push_back(cc);
    }
    ofPolyline line = lineFromCircle(circles);
    
    ofPushMatrix();
        ofScale(-scaleFac,scaleFac,scaleFac);
        ofTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));

        drawMesh(frame.head, ofColor::darkGoldenRod);
//        drawMesh(frame.rightEye, ofColor::red);
//        drawMesh(frame.leftEye, ofColor::blue);

        ofPushStyle();

        //ofScale(10,10,10);
        ofNoFill();
        ofNode n;
        FDM.getOrientation(frame, n);
        n.draw();
        ofSetColor(255);
            ofMatrix4x4 mat = n.getGlobalTransformMatrix();
            ofMultMatrix(mat);
            //ofBoxPrimitive(100, 100, 100).draw();
    
            ofSetLineWidth(4);
                    line.draw();
            ofFill();
            for (int i = 0; i < circles.size(); i++){
               ofCircle(circles[i].pt, circles[i].size);
            }
    
    
   
                //transform
                //outputWriter.addCurves(, );
    
    
            ofPopMatrix();
    
        ofPopStyle();
	ofPopMatrix();

    
    
    
    if (exporting){
    
    ofNode n2;
    FDM.getOrientation(frame, n2);
    ofMatrix4x4 matrix;
    matrix.glScale(-scaleFac,scaleFac,scaleFac);
    matrix.glTranslate(ofVec3f(-adjustments->x,adjustments->y,adjustments->z));
    matrix = n2.getGlobalTransformMatrix() * matrix;
    
    ofPolyline newLine;
    //ofBeginShape();
    for (int i = 0; i < line.getVertices().size(); i++){
        ofPoint a = line.getVertices()[i] * matrix ;
        newLine.addVertex( a.x, a.y, a.z );
    }
    
    vector<ofPolyline> curves;
    curves.push_back(newLine);
    //ofEndShape();
    outputWriter.addCurves("/line", curves);
    
    for (int i = 0; i < circles.size(); i++){
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        for (int j = 0; j < 20; j++){
            
            float angle = (j / 20.0) * TWO_PI;
            float angle2 = ((j+1) / 20.9) * TWO_PI;
            mesh.addVertex(circles[i].pt * matrix);
            mesh.addVertex( (circles[i].pt + ofPoint(cos(angle), sin(angle)) * circles[i].size) * matrix);
            mesh.addVertex( (circles[i].pt + ofPoint(cos(angle2), sin(angle2)) * circles[i].size) * matrix);
        }
        //ofxAlembic::transform(mesh, matrix);
        outputWriter.addPolyMesh("/circle" + ofToString(i), mesh);

    }
        
    }
    
    
    
    ofSetColor(ofColor::white);

	ofDisableDepthTest();
    CM.cameraEnd();
    
    ofEnableAlphaBlending();
    targetFbo.end();
    
    targetFbo.getTextureReference().drawSubsection(0, 0, 1920/2, 1080/2, 0, targetFbo.getHeight() - 1080, 1920, 1080);

    
//    if (exporting){
//        
//        int filenumber;
//        std::ostringstream localOSS;
//        string fileName;
//        
//        filenumber = 2034;
//        
//        localOSS << setw(4) << setfill('0') << currentFrame;
//        
//        fileName = localOSS.str();
//        
//        
//        DEM.endDraw(true, fileName);
//    } else {
//        DEM.endDraw();
//
//    }

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
	//adjustGui->saveSettings("adjustments->xml");
}

void ofApp::keyPressed(ofKeyEventArgs& args){
    
    if (args.key == 's'){
        gui.saveToFile("adjustments.xml");
    }
    
    CM.keyPressed(args.key);
    
}
