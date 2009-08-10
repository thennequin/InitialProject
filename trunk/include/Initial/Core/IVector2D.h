//---------------------------------------------
//                Initial Engine
//                 Coord Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IVECTOR2D_HEADER_
#define _IVECTOR2D_HEADER_

namespace Initial
{
	namespace Core
	{
		class IVector2D
		{
		public:
			IVector2D(float U=0.0, float V=0.0)
			{
				val[0]=U;
				val[1]=V;
			}

			float GetU() { return val[0]; }
			float GetV() { return val[1]; }

			void SetU(float U) { val[0]=U; }
			void SetV(float V) { val[1]=V; }

			void operator= (const IVector2D &coord)
			{
				val[0]=coord.val[0];
				val[1]=coord.val[1];
			}

			operator float*() const
			{
				return (float*)val;
			}
		public:
			union
			{
				struct {float x,y;};
				struct {float u,v;};
				float val[2];
			};
		};
	}
}

#endif