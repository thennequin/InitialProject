//---------------------------------------------
//                Initial Engine
//             Bounding box  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IBBOX_HEADER_
#define _IBBOX_HEADER_

#include "Initial/Video/IRenderDriver.h"

namespace Initial
{
	class IBBox
	{
	public:
		
	public:
		IBBox(Core::IVector3D center=Core::IVector3D(), Core::IVector3D dim=Core::IVector3D());
		IBBox(float minx, float miny, float minz, float maxx, float maxy, float maxz);
		IBBox(IBBox& box);

		void SetBBox(Core::IVector3D center, Core::IVector3D dim);
		void SetBBox(float minx, float miny, float minz, float maxx, float maxy, float maxz);

		Core::IVector3D GetCenter();
		Core::IVector3D GetDimension();
		void SetCenter(Core::IVector3D center);
		void SetDimension(Core::IVector3D dim);

		void Render(Video::IRenderDriver *Device=NULL);
		static void Render(IBBox bbox, Video::IRenderDriver *Device=NULL);

		static IBBox MaxBBox(IBBox box1, IBBox box2);
		static IBBox MinBBox(IBBox box1, IBBox box2);

		void operator= (IBBox box);
	protected:
		Core::IVector3D m_vCenter;
		Core::IVector3D m_vDim;
	};
}

#endif