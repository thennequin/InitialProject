//---------------------------------------------
//                Initial Engine
//             Vertex source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/IBBox.h"

using namespace Initial;


IBBox::IBBox(IVertex center, IVertex dim)
{
	SetBBox(center,dim);
}

IBBox::IBBox( float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	SetBBox( minx, miny, minz, maxx, maxy, maxz);
}

IBBox::IBBox(IBBox& box)
{
	m_vCenter=box.m_vCenter;
	m_vDim=box.m_vDim;
}

void IBBox::SetBBox(IVertex center, IVertex dim)
{
	m_vCenter=center;
	m_vDim=dim;
}

void IBBox::SetBBox(float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	m_vCenter[0]=(minx+maxx)/2.0;
	m_vCenter[1]=(miny+maxy)/2.0;
	m_vCenter[2]=(minz+maxz)/2.0;

	m_vDim[0]=maxx-minx;
	m_vDim[1]=maxy-miny;
	m_vDim[2]=maxz-minz;
}


void IBBox::Render(IRenderDriver *Device)
{
	Render(*this,Device);
}

void IBBox::Render(IBBox bbox, IRenderDriver *Device)
{
	if (Device)
	{
		// X Lines
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0));
		// Y Lines
		Device->_DrawLine(	bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0));
		// Z Lines
		Device->_DrawLine(	bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
		Device->_DrawLine(	bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,-bbox.m_vDim[2]/2.0),
							bbox.m_vCenter+IVertex(-bbox.m_vDim[0]/2.0,-bbox.m_vDim[1]/2.0,bbox.m_vDim[2]/2.0));
	}
}

IBBox IBBox::MaxBBox(IBBox box1, IBBox box2)
{
	float minx,miny,minz,maxx,maxy,maxz;
	minx=box1.m_vCenter[0]-box1.m_vDim[0]/2.0;
	miny=box1.m_vCenter[1]-box1.m_vDim[1]/2.0;
	minz=box1.m_vCenter[2]-box1.m_vDim[2]/2.0;

	maxx=box1.m_vCenter[0]+box1.m_vDim[0]/2.0;
	maxy=box1.m_vCenter[1]+box1.m_vDim[1]/2.0;
	maxz=box1.m_vCenter[2]+box1.m_vDim[2]/2.0;

	if (minx>box2.m_vCenter[0]-box2.m_vDim[0]/2.0)
		minx=box2.m_vCenter[0]-box2.m_vDim[0]/2.0;

	if (miny>box2.m_vCenter[1]-box2.m_vDim[1]/2.0)
		miny=box2.m_vCenter[1]-box2.m_vDim[1]/2.0;

	if (minz>box2.m_vCenter[2]-box2.m_vDim[2]/2.0)
		minz=box2.m_vCenter[2]-box2.m_vDim[2]/2.0;

	if (maxx<box2.m_vCenter[0]+box2.m_vDim[0]/2.0)
		maxx=box2.m_vCenter[0]+box2.m_vDim[0]/2.0;

	if (maxy<box2.m_vCenter[1]+box2.m_vDim[1]/2.0)
		maxy=box2.m_vCenter[1]+box2.m_vDim[1]/2.0;

	if (maxz<box2.m_vCenter[2]+box2.m_vDim[2]/2.0)
		maxz=box2.m_vCenter[2]+box2.m_vDim[2]/2.0;

	return IBBox(minx,miny,minz,maxx,maxy,maxz);
}

IBBox IBBox::MinBBox(IBBox box1, IBBox box2)
{
		float minx,miny,minz,maxx,maxy,maxz;
	minx=box1.m_vCenter[0]-box1.m_vDim[0]/2.0;
	miny=box1.m_vCenter[1]-box1.m_vDim[1]/2.0;
	minz=box1.m_vCenter[2]-box1.m_vDim[2]/2.0;

	maxx=box1.m_vCenter[0]+box1.m_vDim[0]/2.0;
	maxy=box1.m_vCenter[1]+box1.m_vDim[1]/2.0;
	maxz=box1.m_vCenter[2]+box1.m_vDim[2]/2.0;

	if (minx<box2.m_vCenter[0]-box2.m_vDim[0]/2.0)
		minx=box2.m_vCenter[0]-box2.m_vDim[0]/2.0;

	if (miny<box2.m_vCenter[1]-box2.m_vDim[1]/2.0)
		miny=box2.m_vCenter[1]-box2.m_vDim[1]/2.0;

	if (minz<box2.m_vCenter[2]-box2.m_vDim[2]/2.0)
		minz=box2.m_vCenter[2]-box2.m_vDim[2]/2.0;

	if (maxx>box2.m_vCenter[0]+box2.m_vDim[0]/2.0)
		maxx=box2.m_vCenter[0]+box2.m_vDim[0]/2.0;

	if (maxy>box2.m_vCenter[1]+box2.m_vDim[1]/2.0)
		maxy=box2.m_vCenter[1]+box2.m_vDim[1]/2.0;

	if (maxz>box2.m_vCenter[2]+box2.m_vDim[2]/2.0)
		maxz=box2.m_vCenter[2]+box2.m_vDim[2]/2.0;

	return IBBox();
}

void IBBox::operator= (IBBox box)
{
	m_vCenter=box.m_vCenter;
	m_vDim=box.m_vDim;
}

IVertex IBBox::GetCenter()
{
	return m_vCenter;
}

IVertex IBBox::GetDimension()
{
	return m_vDim;
}
