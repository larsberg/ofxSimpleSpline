//
//  ofxSimpleBezierCurve.cpp
//
//  Created by lars berg on 10/23/13.
//
//

#include "ofxSimpleBezierCurve.h"

/**
 * Catmull-Rom 1D interpolation
 * @param  p0 
 * @param  p1 
 * @param  p2 
 * @param  p3 
 * @param  w  
 * @param  w2
 * @param  w3
 * @return    interpolated float
 */
float ofxSimpleBezierCurve::interpolate( float p0, float p1, float p2, float p3, float w, float w2, float w3 )
{
	float v0 = ( p2 - p0 ) * 0.5, v1 = ( p3 - p1 ) * 0.5;
	
	return ( 2 * ( p1 - p2 ) + v0 + v1 ) * w3 + ( - 3 * ( p1 - p2 ) - 2 * v0 - v1 ) * w2 + v0 * w + p1;
};

/**
 * get a position along the curve 
 * @param  k sample position. 0-1
 * @return   ofVec3f
 */
ofVec3f ofxSimpleBezierCurve::getPoint(float k)
{
	return getPoint( k, *controlVertices );
}

/**
 * get position on curve by interoplating some control controlVertices
 * @param  k   sample position
 * @param  _cv our control vertices
 * @return     ofVec3f
 */
ofVec3f ofxSimpleBezierCurve::getPoint(float  k, vector<ofVec3f>& _cv )
{
	if(_cv.size() == 0)	return ofVec3f();
	
	int maxI = _cv.size()-1;
	
	float point = float( _cv.size() - 1 ) * k,
	intPoint = floor( point ),
	w = point - intPoint,
	w2 = w * w,
	w3 = w * w2;
	
	ofVec3f pa = _cv[ ofClamp( intPoint-1, 0, maxI) ],
	pb = _cv[ intPoint ],
	pc = _cv[ ofClamp( intPoint+1, 0, maxI) ],
	pd = _cv[ ofClamp( intPoint+2, 0, maxI) ];
	
	return ofVec3f(interpolate( pa.x, pb.x, pc.x, pd.x, w, w2, w3 ), interpolate( pa.y, pb.y, pc.y, pd.y, w, w2, w3 ), interpolate( pa.z, pb.z, pc.z, pd.z, w, w2, w3 ));
}

/**
 * apppend a control vertex to our control vertices
 */
void ofxSimpleBezierCurve::addControlVertex( ofVec3f v )
{
	controlVertices = &cv;
	cv.push_back( v );
}

/**
 * append a vector of control vertices to our control vertices
 * @param _cv vector of control vertices
 */
void ofxSimpleBezierCurve::addControlVertices( vector<ofVec3f> _cv )
{
	controlVertices = &cv;
	cv.insert(cv.end(), _cv.begin(), _cv.end());
}

/**
 * point our control vertices to an outside vector of control vertices. useful for animating
 * @param _cv reference to a vector of control vertices
 */
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

/**
 * set the number of subdivisions per segment. I haven't checked to see of this is working perfectly
 */
void ofxSimpleBezierCurve::setSubdivisions(int subd)
{
	subdivisions = max(subd, 1);
}

/**
 * update the polyline's vertex positions 
 */
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

/**
 * draws our polyline
 */
void ofxSimpleBezierCurve::draw()
{
	polyline.draw();
}

/**
 * clears it
 */
void ofxSimpleBezierCurve::clear()
{
	polyline.clear();
	cv.clear();
}



