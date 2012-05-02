#ifndef CPOLYGON_H
#define CPOLYGON_H

#include <vector>
#include "Vector.h"
#include "Rect.h"

class cPolygon
{
public:
	cPolygon(){};
	cPolygon(Vector origin);
	cPolygon(Vector origin, Vector point);
	// :TODO: Copy ctr
	~cPolygon();

	void addPoint(Vector point);
	void addPoint(Vector point, int listPos);
	void addPoint(float x, float y);
	void addPoint(float x, float y, int listPos);

	void drawDebug();
	void drawPoints();

	void clear();

	void triangulate();

	void rotate(float rotation);
	void resetRotation();

	Vector getPos();
	float getRotation();
	Rect getBoundingBox();

	void setPos(Vector pos);

	bool requireTriangulation()			{return mReqTriangulation;}
	std::vector<Vector> getTriangles()	{return mTriangles;}

	std::vector<Vector> mVertices;

	Rect  mDrawRect;
private:
	Vector	mOrigin;
	Rect	mBoundingBox;
	float	mRotation;
	std::vector<Vector> mTriangles;
	bool	mReqTriangulation;
};

#endif