//---------------------------------------------
//                Initial Engine
//             Vertex source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Core/IAssert.h"
#include <math.h>
#include "Initial/Core/IVector3D.h"
#include "Initial/Math/IMatrix.h"

namespace Initial
{
	namespace Core
	{
		float IVector3D::_temp;

		IVector3D::IVector3D(float x, float y, float z) 
		{ 
			val[0]=x;
			val[1]=y;
			val[2]=z;
		}

		IVector3D::IVector3D(const IVector3D& src)
		{
			val[0]=src.val[0];
			val[1]=src.val[1];
			val[2]=src.val[2];
		}

		float IVector3D::GetX() 
		{ 
			return val[0]; 
		}

		float IVector3D::GetY() 
		{ 
			return val[1]; 
		}

		float IVector3D::GetZ() 
		{ 
			return val[2]; 
		}

		void IVector3D::SetX(float x) 
		{ 
			val[0]=x; 
		}

		void IVector3D::SetY(float y) 
		{ 
			val[1]=y; 
		}

		void IVector3D::SetZ(float z) 
		{ 
			val[2]=z; 
		}

		void IVector3D::Set(float x, float y, float z)
		{
			val[0]=x;
			val[1]=y;
			val[2]=z;
		}

		void IVector3D::Set(IVector3D& vert)
		{
			val[0]=vert[0];
			val[1]=vert[1];
			val[2]=vert[2];
		}

		void IVector3D::Normalize()
		{
			float norm = Length();
			val[0]/=norm;
			val[1]/=norm;
			val[2]/=norm;
		}

		float IVector3D::Length()
		{
			return sqrt( val[0]*val[0] + val[1]*val[1] + val[2]*val[2] );
		}

		IVector3D IVector3D::Dot(IVector3D a, IVector3D b)
		{
			IVector3D vert;
			vert.SetX((a.GetY()*b.GetZ()) - (a.GetZ()*b.GetY()));
			vert.SetY((a.GetZ()*b.GetX()) - (a.GetX()*b.GetZ()));
			vert.SetZ((a.GetX()*b.GetY()) - (a.GetY()*b.GetX()));
			return vert;
		}

		/*float& IVector3D::operator[](int val)
		{
			if (val>=0 && val<3)
				return val[val];
			else return _temp;
		}

		float& IVector3D::operator[](int val) const
		{
			float temp;
			if (val>=0 && val<3)
				temp=val[val];
			return _temp;
		}*/

		void IVector3D::operator= (const IVector3D &vertex)
		{
			val[0]=vertex.val[0];
			val[1]=vertex.val[1];
			val[2]=vertex.val[2];
		}

		//const IVector3D IVector3D::operator+ (IVector3D vertex)
		//{
		//	IVector3D res;
		//	res.val[0]=val[0]+vertex.val[0];
		//	res.val[1]=val[1]+vertex.val[1];
		//	res.val[2]=val[2]+vertex.val[2];
		//	return res;
		//}

		const IVector3D IVector3D::operator+ (const IVector3D vertex) const
		{
			IVector3D res;
			res.val[0]=val[0]+vertex.val[0];
			res.val[1]=val[1]+vertex.val[1];
			res.val[2]=val[2]+vertex.val[2];
			return res;
		}

		const IVector3D IVector3D::operator- (const IVector3D vertex) const
		{
			IVector3D res;
			res.val[0]=val[0]-vertex.val[0];
			res.val[1]=val[1]-vertex.val[1];
			res.val[2]=val[2]-vertex.val[2];
			return res;
		}

		const IVector3D IVector3D::operator* (const IVector3D vertex) const
		{
			IVector3D res;
			res.val[0]=val[0]*vertex.val[0];
			res.val[1]=val[1]*vertex.val[1];
			res.val[2]=val[2]*vertex.val[2];
			return res;
		}

		/*const IVector3D IVector3D::operator+ (IVector3D vertex) const
		{
			IVector3D res;
			res.val[0]=val[0]+vertex.GetX();
			res.val[1]=val[1]+vertex.GetY();
			res.val[2]=val[2]+vertex.GetZ();
			return res;
		}*/

		const IVector3D IVector3D::operator* (const float scale) const
		{
			IVector3D res;
			res.val[0]=val[0]*scale;
			res.val[1]=val[1]*scale;
			res.val[2]=val[2]*scale;
			return res;
		}

		const IVector3D IVector3D::operator/ (const float scale) const
		{
			ASSERT(scale);
			IVector3D res;
			res.val[0]=val[0]/scale;
			res.val[1]=val[1]/scale;
			res.val[2]=val[2]/scale;
			return res;
		}

		const IVector3D IVector3D::operator+ (const float scale) const
		{
			IVector3D res;
			res.val[0]=val[0]+scale;
			res.val[1]=val[1]+scale;
			res.val[2]=val[2]+scale;
			return res;
		}

		const IVector3D IVector3D::operator-() const
		{
			IVector3D res = IVector3D(0,0,0)-*this;
			return res;
		}

		IVector3D::operator float*() const
		{
			return (float*)val;
		}

		float Dot(IVector3D pt1, IVector3D pt2)
		{
			return pt1.GetX()*pt2.GetX() + pt1.GetY()*pt2.GetY() + pt1.GetZ()*pt2.GetZ();
		}

		IVector3D Cross(IVector3D pt1, IVector3D pt2)
		{
			IVector3D res;
			res.SetX((pt1.GetY()*pt2.GetZ()) - (pt1.GetZ()*pt2.GetY()));
			res.SetY((pt1.GetZ()*pt2.GetX()) - (pt1.GetX()*pt2.GetZ()));
			res.SetZ((pt1.GetX()*pt2.GetY()) - (pt1.GetY()*pt2.GetX()));
			return res;
		}

		float Determinant(IVector3D v1, IVector3D v2, IVector3D v3)
		{
			Math::IMatrix mat(3,3);
			mat[0][0]=v1.GetX();
			mat[0][1]=v1.GetY();
			mat[0][2]=v1.GetZ();
			mat[1][0]=v2.GetX();
			mat[1][1]=v2.GetY();
			mat[1][2]=v2.GetZ();
			mat[2][0]=v3.GetX();
			mat[2][1]=v3.GetY();
			mat[2][2]=v3.GetZ();
			return mat.Det();
		}
	}
}

const Initial::Core::IVector3D operator* (float val, const Initial::Core::IVector3D& vec)
{
	return vec*val;
}