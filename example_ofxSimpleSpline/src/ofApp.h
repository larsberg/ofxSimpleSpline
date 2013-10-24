#pragma once

#include "ofMain.h"
#include "ofxSimpleSpline.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	
	vector<ofVec3f> cv;
	
	ofEasyCam camera;
	
	ofxSimpleSpline curve;
	
	ofVec3f pointOnCurve;
	
};
