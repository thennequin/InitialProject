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
#include "Initial/Math/IMath.h"

namespace Initial
{
	namespace Core
	{
		float IVector3D::_temp;

		IVector3D::IVector3D(float X, float Y, float Z) 
		{ 
			x=X;
			y=Y;
			z=Z;
		}

		IVector3D::IVector3D(const IVector3D& src)
		{
			x=src.x;
			y=src.y;
			z=src.z;
		}

		float IVector3D::GetX() 
		{ 
			return x; 
		}

		float IVector3D::GetY() 
		{ 
			return y; 
		}

		float IVector3D::GetZ() 
		{ 
			return z; 
		}

		void IVector3D::SetX(float X) 
		{ 
			x=X; 
		}

		void IVector3D::SetY(float Y) 
		{ 
			y=Y;
		}

		void IVector3D::SetZ(float Z) 
		{ 
			z=Z;
		}

		void IVector3D::Set(float X, float Y, float Z)
		{
			x=X;
			y=Y;
			z=Z;
		}

		void IVector3D::Set(IVector3D& vert)
		{
			x=vert.x;
			y=vert.y;
			z=vert.z;
		}

		void IVector3D::Normalize()
		{
			float norm = Length();
			if (norm)
			{
				x/=norm;
				y/=norm;
				z/=norm;
			}
		}

		float IVector3D::Length()
		{
			return 1.0/Q_rsqrt( x*x+y*y+z*z );
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
			x=vertex.x;
			y=vertex.y;
			z=vertex.z;
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
			res.x=x+vertex.x;
			res.y=y+vertex.y;
			res.z=z+vertex.z;
			return res;
		}

		const IVector3D IVector3D::operator- (const IVector3D vertex) const
		{
			IVector3D res;
			res.x=x-vertex.x;
			res.y=y-vertex.y;
			res.z=z-vertex.z;
			return res;
		}

		const IVector3D IVector3D::operator* (const IVector3D vertex) const
		{
			IVector3D res;
			res.x=x*vertex.x;
			res.y=y*vertex.y;
			res.z=z*vertex.z;
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
			res.x=x*scale;
			res.y=y*scale;
			res.z=z*scale;
			return res;
		}

		const IVector3D IVector3D::operator/ (const float scale) const
		{
			ASSERT(scale);
			IVector3D res;
			res.x=x/scale;
			res.y=y/scale;
			res.z=z/scale;
			return res;
		}

		const IVector3D IVector3D::operator+ (const float scale) const
		{
			IVector3D res;
			res.x=x+scale;
			res.y=y+scale;
			res.z=z+scale;
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