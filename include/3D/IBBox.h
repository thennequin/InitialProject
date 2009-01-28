//---------------------------------------------
//                Initial Engine
//             Bounding box  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IBBOX_HEADER_
#define _IBBOX_HEADER_

#include "Video/IRenderDriver.h"

namespace Initial
{
	class IBBox
	{
	public:
		
	public:
		IBBox(IVertex center=IVertex(), IVertex dim=IVertex());
		IBBox(float minx, float miny, float minz, float maxx, float maxy, float maxz);
		IBBox(IBBox& box);

		void SetBBox(IVertex center, IVertex dim);
		void SetBBox(float minx, float miny, float minz, float maxx, float maxy, float maxz);

		IVertex GetCenter();
		IVertex GetDimension();

		void Render(IRenderDriver *Device=NULL);
		static void Render(IBBox bbox, IRenderDriver *Device=NULL);

		static IBBox MaxBBox(IBBox box1, IBBox box2);
		static IBBox MinBBox(IBBox box1, IBBox box2);

		void operator= (IBBox box);
	protected:
		IVertex m_vCenter;
		IVertex m_vDim;
	};
}

#endif