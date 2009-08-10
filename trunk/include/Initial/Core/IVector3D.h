//---------------------------------------------
//                Initial Engine
//                Vector3D Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IVECTOR3D_HEADER_
#define _IVECTOR3D_HEADER_

#include "Initial/Core/IVector3D.h"

namespace Initial
{
	namespace Core
	{
		class IVector3D
		{
		public:
			
			IVector3D(float x=0.0, float y=0.0, float z=0.0);
			IVector3D(const IVector3D& src);

			float GetX();
			float GetY();
			float GetZ();

			void SetX(float x);
			void SetY(float y);
			void SetZ(float z);
			void Set(float x, float y, float z);
			void Set(IVector3D& vert);

			void Normalize();
			float Length();

			static IVector3D Dot(IVector3D a, IVector3D b);

			/*float& operator[](int val);
			float& operator[](int val) const;*/

			void operator= (const IVector3D &vertex);

			const IVector3D operator+ (const IVector3D vertex) const;
			const IVector3D operator- (const IVector3D vertex) const;
			const IVector3D operator* (const IVector3D vertex) const;
			//const IVector3D operator+ (IVector3D vertex) const;

			/*IVector3D &operator*=(float f) { return *this = *this * f; }
			IVector3D &operator/=(float f) { return *this = *this / f; }*/
			//IVector3D &operator+=(const IVector3D &v) { return *this = *this + v; }
			//IVector3D &operator-=(const IVector3D &v) { return *this = *this - v; }

			const IVector3D operator* (const float scale) const;
			const IVector3D operator/ (const float scale) const;
			const IVector3D operator+ (const float scale) const;

			const IVector3D operator-() const;

			operator float*() const;

		public:
			union
			{
				struct {float x,y,z;};
				struct {float r,g,b;};
				float val[3];
			};
		protected:
			static float _temp;
		};

		float Dot(IVector3D pt1, IVector3D pt2);
		IVector3D Cross(IVector3D pt1, IVector3D pt2);
		float Determinant(IVector3D v1, IVector3D v2, IVector3D v3);
	}
}

const Initial::Core::IVector3D operator* (float val, const Initial::Core::IVector3D& vec);

#endif