//
//  ofxSimpleSpline.cpp
//
//  Created by lars berg on 10/23/13.
//
//

#include "ofxSimpleSpline.h"

/**
 * Catmull-Rom 1D interpolation
 */
float ofxSimpleSpline::interpolate( float p0, float p1, float p2, float p3, float w, float w2, float w3 )
{
	float v0 = ( p2 - p0 ) * 0.5, v1 = ( p3 - p1 ) * 0.5;
	return ( 2 * ( p1 - p2 ) + v0 + v1 ) * w3 + ( - 3 * ( p1 - p2 ) - 2 * v0 - v1 ) * w2 + v0 * w + p1;
};

/**
 * Catmull-Rom 3D interpolation
 */
ofVec3f ofxSimpleSpline::interpolate( ofVec3f p0, ofVec3f p1, ofVec3f p2, ofVec3f p3, float w, float w2, float w3 )
{
	ofVec3f v0 = ( p2 - p0 ) * 0.5, v1 = ( p3 - p1 ) * 0.5;
	return ( 2 * ( p1 - p2 ) + v0 + v1 ) * w3 + ( - 3 * ( p1 - p2 ) - 2 * v0 - v1 ) * w2 + v0 * w + p1;
};

/**
 * get a position along the curve
 * @param  k sample position. 0-1
 * @return   ofVec3f
 */
ofVec3f ofxSimpleSpline::getPoint(float k)
{
	return getPoint( k, *controlVertices );
}

/**
 * get vector tangent to a point on the curve
 * @param  u sample position. 0-1
 * @return   ofVec3f
 */
ofVec3f ofxSimpleSpline::getTangent(float u, float sampleOffset)
{
	float tSample = u+sampleOffset;
	if (tSample <= 1.)
	{
		return getPoint(u) - getPoint(tSample);
	}else{
		return getPoint(u-sampleOffset) - getPoint(u);
	}
}

/**
 * get position on curve by interoplating some control controlVertices
 * @param  k   sample position
 * @param  _cv our control vertices
 * @return     ofVec3f
 */
ofVec3f ofxSimpleSpline::getPoint(float  k, vector<ofVec3f>& _cv, bool closed )
{
	if(!closed)
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
		
		return interpolate( pa, pb, pc, pd, w, w2, w3 );
	}
	else
	{
		if(_cv.size() == 0)	return ofVec3f();
		
		int maxI = _cv.size()-1;
		
		float point = float( _cv.size() - 1 ) * k,
		intPoint = floor( point ),
		w = point - intPoint,
		w2 = w * w,
		w3 = w * w2;
		
		ofVec3f pa = _cv[ intPoint == 0 ? maxI : intPoint-1 ],
		pb = _cv[ intPoint ],
		pc = _cv[ int(intPoint+1) % _cv.size() ],
		pd = _cv[ int(intPoint+2) % _cv.size() ];
		
		return interpolate( pa, pb, pc, pd, w, w2, w3 );
	}
}

/**
 * apppend a control vertex to our control vertices
 */
void ofxSimpleSpline::addControlVertex( ofVec3f v )
{
	bSetPolyline = true;
	controlVertices = &cv;
	cv.push_back( v );
}

/**
 * append a vector of control vertices to our control vertices
 * @param _cv vector of control vertices
 */
void ofxSimpleSpline::addControlVertices( vector<ofVec3f> _cv )
{
	bSetPolyline = true;
	controlVertices = &cv;
	cv.insert(cv.end(), _cv.begin(), _cv.end());
}

/**
 * point our control vertices to an outside vector of control vertices. useful for animating
 * @param _cv reference to a vector of control vertices
 */
void ofxSimpleSpline::setControlVertices( vector<ofVec3f>& _cv, bool updateVbo )
{
	controlVertices = &_cv;
	
	if(controlVertices->size())
	{
		vector <ofVec3f> v( subdivisions * controlVertices->size() );
		
		float step = 1./(v.size()-1);
		for (int i=0; i<v.size(); i++) {
			v[i] = getPoint( step * float(i), *controlVertices );
		}
		
		bSetPolyline = true;
		
		if(updateVbo)	update();
	}
}

/**
 * set the number of subdivisions per segment. I haven't checked to see of this is working perfectly
 */
void ofxSimpleSpline::setSubdivisions(int subd)
{
	bSetPolyline = true;
	subdivisions = max(subd, 1);
}

/**
 * update the vbo's vertex positions & texCoords
 */
void ofxSimpleSpline::update()
{
	if(controlVertices->size())
	{
		
		if(bSetPolyline)	setupPolyline();
		
		float step = 1./(v.size()-1);
		for (int i=0; i<v.size(); i++)
		{
			v[i] = getPoint( float(i) * step, *controlVertices );
			texCoords[i].set(float(i) * step, float(i) * step);
		}
		
		lineVbo.updateVertexData( &v[0], v.size());
		lineVbo.updateTexCoordData( &texCoords[0], texCoords.size() );
	}
}

/**
 * draws our vbo
 */
void ofxSimpleSpline::draw( GLenum drawType)
{
	lineVbo.draw( drawType, 0, vCount );
}

/**
 * clears it
 */
void ofxSimpleSpline::clear()
{
	lineVbo.clear();
	cv.clear();
	v.clear();
	texCoords.clear();
}


void ofxSimpleSpline::setupPolyline()
{
	bSetPolyline = false;
	
	vCount = subdivisions * controlVertices->size();
	v.resize( vCount );
	texCoords.resize( vCount );

	lineVbo.clear();
	lineVbo.setVertexData( &v[0], v.size(), GL_STATIC_DRAW );
	lineVbo.setTexCoordData( &texCoords[0], texCoords.size(), GL_STATIC_DRAW);
}





