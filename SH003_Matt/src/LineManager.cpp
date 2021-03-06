#include "LineManager.h"
#include "ofxNearestNeighbour.h"

void LineManager::setup(){

	startPointX.addListener(this, &LineManager::paramChanged);;
	startPointY.addListener(this, &LineManager::paramChanged);
	startPointZ.addListener(this, &LineManager::paramChanged);
	
	extrudeAmount.addListener(this, &LineManager::paramChanged);
	arcRadius.addListener(this, &LineManager::paramChanged);
	arcAngle.addListener(this, &LineManager::paramChanged);

	rotationAmount.addListener(this, &LineManager::paramChanged);
	resampleAmount.addListener(this, &LineManager::paramChanged);
	
	chaseDampen.addListener(this, &LineManager::paramChanged);
	maxNewAngle.addListener(this, &LineManager::paramChanged);
	
	aRadius.addListener(this, &LineManager::paramChanged);
	bRadius.addListener(this, &LineManager::paramChanged);
	
	numAttachPoints.addListener(this, &LineManager::paramChanged);;

	
}

void LineManager::update(int cf){
	
	curFrame = cf;
	
	if(meshes.size() > 0){
		int curIndex = ofClamp(curFrame,0,meshes.size()-1);
		curMesh = meshes[curIndex];
		curHooks = hooksPerFrame[curIndex];
	}
}

void LineManager::draw(){
	curMesh.draw();
	for(int i = 0; i < curHooks.size(); i++){
		ofDrawSphere(curHooks[i].pos, 10);
	}
}

void LineManager::drawArc(){
	ofMesh m;
	m.addVertices( basePoints );
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	m.draw();
	
	ptf.debugDraw(10.0);
}

void LineManager::paramChanged(float& param){
	generateLine(numFrames);
}

void LineManager::generateLine(int numF){
	
	numFrames = numF;
	
	b.setParent(a);
	b.setPosition(0, aRadius, 0);
	c.setParent(b);
	c.setPosition(0, bRadius, 0);
	
	basePoints.clear();
	linePoints.clear();
	ptf.clear();
	meshes.clear();
	hooks.clear();
	hooksPerFrame.clear();
	
	//the basis of this is a simple arc with a Parallel Transport Frame
	//PTF allows us to have a consistent normal and tangent from the curve as well as a position
	ofVec3f startPoint(startPointX,startPointY,startPointZ);
	for(int i = 0; i < numFrames; i++){
		
		float percentDone = 1.0 * i / numFrames;
		float angle = percentDone * arcAngle;
		
		ofVec3f pos = startPoint + ofVec3f(0,-1,0).getRotated(angle, ofVec3f(0,0,1) ) * arcRadius;
		pos += ofVec3f(0,0, extrudeAmount * percentDone);

		ptf.addPoint(pos);

		basePoints.push_back( pos );
	}
	
	for(int i = 0; i < numAttachPoints; i++){
		AttachPoint hook;
		hook.startFrame = ofMap(i, 0, numAttachPoints, 0, numFrames);
		hook.percentOnCurve = 1.0 * i / numAttachPoints;
		hook.firstHook = true; //set the initial position, then stick to the line
		hooks.push_back(hook);
	}
	
	//after building the arc we run the nodes along it and collect their path
	//along with resampling and storing the mesh for each frame
	for(int i = 0; i < numFrames; i++){
		
		float percentAlongCurve = 1.0 * i / numFrames;
		
		//apply the transform to the base
		int index = percentAlongCurve * (numFrames-2);
		int nextIndex = index+1;
		a.setTransformMatrix(ptf.frameAt(index));
		
		ofNode next;
		next.setTransformMatrix(ptf.frameAt(nextIndex));
		ofVec3f lookDir = next.getPosition() - a.getPosition();
		
		//rotate it around a consistent angle
		a.rotate(rotationAmount*percentAlongCurve, lookDir);
		
		//make B chase C
//		ofNode aT,bT,cT;
//		aT.setTransformMatrix(a.getGlobalTransformMatrix());
//		bT.setTransformMatrix(b.getGlobalTransformMatrix());
//		cT.setTransformMatrix(c.getGlobalTransformMatrix());
		
		ofQuaternion angleBtoC, angleAdjust;
		ofVec3f curLook = b.getSideDir();
		ofVec3f targetLook = c.getPosition()-b.getPosition();
		
		float angleBetween = curLook.angle(targetLook);
		b.rotate(angleBetween*chaseDampen, 0, 0, 1);
		
		//angleBtoC.makeRotate(curLook, targetLook);
		//angleAdjust.slerp(chaseDampen, b.getOrientationQuat()*angleBtoC, b.getOrientationQuat());
		//b.setOrientation(b.getOrientationQuat() * angleAdjust);
		if(angleBetween < 5){
			c.rotateAround(ofQuaternion(ofRandom(-maxNewAngle,maxNewAngle), ofVec3f(0,0,1)), b.getPosition());
		}
		
		//capture the parented position of c into n and add it to the path
		ofNode n;
		n.setTransformMatrix(c.getGlobalTransformMatrix());
		
		//add it to the points, resample and reset the current line to the resampled line
		//this is how the line uncurls over time
		linePoints.push_back(n.getPosition());
		ofPolyline p;
		p.addVertices(linePoints);
		p = p.getResampledByCount(resampleAmount);
		linePoints = p.getVertices();
		
		//store the mesh
		ofMesh m;
		m.setMode(OF_PRIMITIVE_LINE_STRIP);
		m.addVertices(linePoints);
		meshes.push_back(m);
		
		
		//create attachment points for the little tools
		//calculate KDTree of this line
		ofxNearestNeighbour3D nn;
		nn.buildIndex(linePoints);
		
		vector<AttachPoint> hooksThisFrame;
		//calculate hooks with the current line
		for(int h = 0; h < hooks.size(); h++){
			
			if(hooks[h].startFrame < i){
				//first time we have seen this point lock it to the line
				if(hooks[h].firstHook){
					hooks[h].pos = linePoints.back();
					hooks[h].firstHook = false;
				}
				else{
					//STICKY POINTS woohoo!!!
					//otherise move it to the closest point on the resampled line
					vector<size_t> closestIndeces;
					vector<float> distances;
					nn.findNClosestPoints(hooks[h].pos, 2, closestIndeces, distances);

					ofVec3f x0 = hooks[h].pos;
					ofVec3f x1 = linePoints[closestIndeces[0]];
					ofVec3f x2 = linePoints[closestIndeces[1]];
					//formulate for the distnce from a point to a line segment
//					d   =   (|(x_2-x_1)x(x_1-x_0)|)/(|x_2-x_1|)
					float d = (x2-x1).getCrossed(x1-hooks[h].pos).length() / (x2-x1).length();
					//solve the pythag formula to get the distance along the line from x1 towards x2
					float xn = sqrt((float)(x0 - x1).lengthSquared() - d*d);
					//that leg of the triangle goes from x1 to x2
					hooks[h].pos = x1 + (x2-x1).normalize() * xn;
					
				}
				
				hooksThisFrame.push_back(hooks[h]);
			}
		}
		
		hooksPerFrame.push_back(hooksThisFrame);
	}
}

