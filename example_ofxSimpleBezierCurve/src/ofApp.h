#pragma once

#include "ofMain.h"
#include "ofxSimpleBezierCurve.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	
	vector<ofVec3f> cv;
	
	ofEasyCam camera;
	
	ofxSimpleBezierCurve bezierCurve;
	
	ofVec3f pointOnCurve;
	
};
