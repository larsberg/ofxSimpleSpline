//
//  ofxSimpleSpline.h
//
//  Created by lars berg on 10/23/13.
//
//

#include "ofMain.h"

class ofxSimpleSpline{
public:
	ofxSimpleSpline(){
		controlVertices = &cv;
		subdivisions = 3;
	};
	~ofxSimpleSpline()
	{
		clear();
	};
	
	void setControlVertices( vector<ofVec3f>& _cv );
	vector<ofVec3f>& getControlVertices(){	return *controlVertices;}
	
	void addControlVertex( ofVec3f v );
	void addControlVertices( vector<ofVec3f> _cv );
	
	static float interpolate( float p0, float p1, float p2, float p3, float t, float t2, float t3 );
	float tangentQuadraticBezier( float t, float p0, float p1, float p2 );
	static ofVec3f getPoint(float  k, vector<ofVec3f>& _cv );
	ofVec3f getPoint(float k);
	
	
	void update();
	void draw();
	
	void clear();
	
	void setSubdivisions(int subd);
	int getSubdiviions(){	return subdivisions;}
	
	ofPolyline* getPolyLine(){	return &polyline;}
	
private:
	vector<ofVec3f> cv;
	vector<ofVec3f>* controlVertices;
	
	int subdivisions;
	ofPolyline polyline;
};