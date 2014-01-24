//
//  ofxSimpleSpline.h
//
//  Created by lars berg on 10/23/13.
//
//

#pragma once
#include "ofMain.h"

class ofxSimpleSpline{
public:
	ofxSimpleSpline()
	{
		controlVertices = &cv;
		subdivisions = 3;
		bSetPolyline = true;
		bClosed = false;
	};
	~ofxSimpleSpline()
	{
		clear();
	};
	
	void setControlVertices( vector<ofVec3f>& _cv, bool updateVbo=true );
	
	vector<ofVec3f>& getControlVertices(){	return *controlVertices;}
	
	ofVec3f& getControlVertex(int i){ return (*controlVertices)[i];}
	
	void addControlVertex( ofVec3f v );
	
	void addControlVertices( vector<ofVec3f> _cv );
	
	static float interpolate( float p0, float p1, float p2, float p3, float t, float t2, float t3 );
	
	static ofVec3f interpolate( ofVec3f p0, ofVec3f p1, ofVec3f p2, ofVec3f p3, float w, float w2, float w3 );
	
	float tangentQuadraticBezier( float t, float p0, float p1, float p2 );
	
	static ofVec3f getPoint(float  k, vector<ofVec3f>& _cv, bool closed=false );
	
	ofVec3f getPoint(float k);
	
	ofVec3f getTangent(float u, float sampleOffset = .01);
	
	void update();
	
	void draw(GLenum drawType=GL_LINE_STRIP);
	
	void clear();
	
	void setSubdivisions(int subd);
	
	int getSubdiviions(){	return subdivisions;}
	
	void setupPolyline();
	
	void close( bool closed = true){
		bClosed = closed;
	}
	
//private:
	vector<ofVec3f> cv;
	vector<ofVec3f>* controlVertices;
	
	int subdivisions;
	
	vector<ofVec3f> v;
	vector<ofVec2f> texCoords;
	int vCount;
	ofVbo lineVbo;
	
	bool bSetPolyline, bClosed;
};