//
//  ofxSimpleBezierCurve.cpp
//
//  Created by laserstorm on 10/23/13.
//
//

#include "ofxSimpleBezierCurve.h"

float ofxSimpleBezierCurve::interpolate( float p0, float p1, float p2, float p3, float t, float t2, float t3 )
{
	float v0 = ( p2 - p0 ) * 0.5,
	v1 = ( p3 - p1 ) * 0.5;
	
	return ( 2 * ( p1 - p2 ) + v0 + v1 ) * t3 + ( - 3 * ( p1 - p2 ) - 2 * v0 - v1 ) * t2 + v0 * t + p1;
};

ofVec3f ofxSimpleBezierCurve::getPoint(float k)
{
	return getPoint( k, *controlVertices );
}

ofVec3f ofxSimpleBezierCurve::getPoint(float  k, vector<ofVec3f>& _cv )
{
	if(_cv.size() == 0)
	{
		cout << "wtf" << endl;
		return ofVec3f();
	}
	
	int maxI = _cv.size()-1;
	
	float point = float( _cv.size() - 1 ) * k,
	intPoint = floor( point ),
	weight = point - intPoint,
	w2 = weight * weight,
	w3 = weight * w2;
	
	ofVec3f pa = _cv[ ofClamp( intPoint-1, 0, maxI) ],
	pb = _cv[ intPoint ],
	pc = _cv[ ofClamp( intPoint+1, 0, maxI) ],
	pd = _cv[ ofClamp( intPoint+2, 0, maxI) ];
	
	return ofVec3f(interpolate( pa.x, pb.x, pc.x, pd.x, weight, w2, w3 ), interpolate( pa.y, pb.y, pc.y, pd.y, weight, w2, w3 ), interpolate( pa.z, pb.z, pc.z, pd.z, weight, w2, w3 ));
}

void ofxSimpleBezierCurve::addControlVertex( ofVec3f v )
{
	controlVertices = &cv;
	cv.push_back( v );
}

void ofxSimpleBezierCurve::addControlVertices( vector<ofVec3f> _cv )
{
	controlVertices = &cv;
	cv.insert(cv.end(), _cv.begin(), _cv.end());
}

void ofxSimpleBezierCurve::setControlVertices( vector<ofVec3f>& _cv )
{
	controlVertices = &_cv;
	
	if(controlVertices->size())
	{
		vector <ofVec3f> v( subdivisions * controlVertices->size() );
		
		float step = 1./(v.size()-1);
		for (int i=0; i<v.size(); i++) {
			v[i] = getPoint( step * float(i), *controlVertices );
		}
		
		polyline.clear();
		polyline.addVertices( v );
	}
}


void ofxSimpleBezierCurve::setSubdivisions(int subd)
{
	subdivisions = max(subd, 1);
}

void ofxSimpleBezierCurve::update()
{
	if(controlVertices->size())
	{
		
		vector <ofVec3f>& v = polyline.getVertices();
		float step = 1./(v.size()-1);
		for (int i=0; i<v.size(); i++)
		{
			v[i] = getPoint( float(i) * step, *controlVertices );
		}
	}
}

void ofxSimpleBezierCurve::draw()
{
	polyline.draw();
}

void ofxSimpleBezierCurve::clear()
{
	polyline.clear();
	cv.clear();
}



