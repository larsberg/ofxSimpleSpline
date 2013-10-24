#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//make some control vertices
	cv.resize(10);
	float radius = 150;
	for (int i=0; i<cv.size(); i++) {
		cv[i].set( sin( TWO_PI * float(i)/float(cv.size())) * radius, 0, cos(TWO_PI * float(i)/float(cv.size())) * radius );
	}
	
	//setup our curve
	curve.setSubdivisions( 10 );
	curve.setControlVertices( cv );
}


//--------------------------------------------------------------
void ofApp::update()
{
	
	//move arounf our control vertices
	for (int i=0; i<cv.size(); i++) {
		cv[i].y = sin( ofGetElapsedTimef() + i ) * 30.;
	}
	
	//update the vertices
	curve.update();
	
	//find a point on curve 0-1
	pointOnCurve = curve.getPoint( sin(ofGetElapsedTimef())*.5 + .5 );
}

//--------------------------------------------------------------
void ofApp::draw()
{
	camera.begin();
	
	//draw the control vertices
	ofSetColor(0,255,255);
	for (int i=0; i<cv.size(); i++) {
		ofDrawSphere(cv[i], 3);
	}
	
	//draw a point on curve
	ofSetColor(255, 255, 0);
	ofDrawSphere( pointOnCurve, 5);
	
	
	//draw the curve
	ofSetColor(255);
	curve.draw();
	
	camera.end();
}