//---------------------------------------------
//                Initial Engine
//                 Coord Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICOORD_HEADER_
#define _ICOORD_HEADER_

namespace Initial
{
	class ICoord
	{
	public:
		ICoord(float U=0.0, float V=0.0)
		{
			uv[0]=U;
			uv[1]=V;
		}

		float GetU() { return uv[0]; }
		float GetV() { return uv[1]; }

		void SetU(float U) { uv[0]=U; }
		void SetV(float V) { uv[1]=V; }

		void operator= (ICoord coord)
		{
			uv[0]=coord.uv[0];
			uv[1]=coord.uv[1];
		}

		operator float*() const
		{
			return (float*)uv;
		}
	private:
		float uv[2];
	};
}

#endif