//---------------------------------------------
//                Initial Engine
//             Vertex source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/IVertex.h"

using namespace Initial;

//cross product u et v
//(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);


IVertex::IVertex(float x, float y, float z) 
{ 
	pos[0]=x;
	pos[1]=y;
	pos[2]=z;
}

IVertex::IVertex(const IVertex& src)
{
	pos[0]=src.pos[0];
	pos[1]=src.pos[1];
	pos[2]=src.pos[2];
}

float IVertex::GetX() 
{ 
	return pos[0]; 
}

float IVertex::GetY() 
{ 
	return pos[1]; 
}

float IVertex::GetZ() 
{ 
	return pos[2]; 
}

void IVertex::SetX(float x) 
{ 
	pos[0]=x; 
}

void IVertex::SetY(float y) 
{ 
	pos[1]=y; 
}

void IVertex::SetZ(float z) 
{ 
	pos[2]=z; 
}

void IVertex::Set(float x, float y, float z)
{
	pos[0]=x;
	pos[1]=y;
	pos[2]=z;
}

void IVertex::Set(IVertex& vert)
{
	pos[0]=vert[0];
	pos[1]=vert[1];
	pos[2]=vert[2];
}

void IVertex::Normalize()
{
	//float norm = 1.0 / sqrt( pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2] );
	float norm = 1.0/(pos[0]+pos[1]+pos[2]);
	pos[0]*=norm;
	pos[1]*=norm;
	pos[2]*=norm;
}

float& IVertex::operator[](int val)
{
	float temp=0.0;
	if (val>=0 && val<3)
		return pos[val];
	else return temp;
}

float& IVertex::operator[](int val) const
{
	float temp=0.0;
	if (val>=0 && val<3)
		temp=pos[val];
	return temp;
}

void IVertex::operator= (IVertex vertex)
{
	pos[0]=vertex.pos[0];
	pos[1]=vertex.pos[1];
	pos[2]=vertex.pos[2];
}

//const IVertex IVertex::operator+ (IVertex vertex)
//{
//	IVertex res;
//	res.pos[0]=pos[0]+vertex.pos[0];
//	res.pos[1]=pos[1]+vertex.pos[1];
//	res.pos[2]=pos[2]+vertex.pos[2];
//	return res;
//}

const IVertex IVertex::operator+ (const IVertex vertex) const
{
	IVertex res;
	res.pos[0]=pos[0]+vertex.pos[0];
	res.pos[1]=pos[1]+vertex.pos[1];
	res.pos[2]=pos[2]+vertex.pos[2];
	return res;
}

const IVertex IVertex::operator- (const IVertex vertex) const
{
	IVertex res;
	res.pos[0]=pos[0]-vertex.pos[0];
	res.pos[1]=pos[1]-vertex.pos[1];
	res.pos[2]=pos[2]-vertex.pos[2];
	return res;
}

const IVertex IVertex::operator* (const IVertex vertex) const
{
	IVertex res;
	res.pos[0]=pos[0]*vertex.pos[0];
	res.pos[1]=pos[1]*vertex.pos[1];
	res.pos[2]=pos[2]*vertex.pos[2];
	return res;
}

const IVertex IVertex::operator+ (INormal vertex) const
{
	IVertex res;
	res.pos[0]=pos[0]+vertex.GetX();
	res.pos[1]=pos[1]+vertex.GetY();
	res.pos[2]=pos[2]+vertex.GetZ();
	return res;
}

const IVertex IVertex::operator* (const float scale) const
{
	IVertex res;
	res.pos[0]=pos[0]*scale;
	res.pos[1]=pos[1]*scale;
	res.pos[2]=pos[2]*scale;
	return res;
}

IVertex::operator float*() const
{
	return (float*)pos;
}