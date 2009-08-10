//---------------------------------------------
//                Initial Engine
//                 Line  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ILINE_HEADER_
#define _ILINE_HEADER_

#include "Initial/Core/IVector3D.h"

namespace Initial
{

	class ILine // triangle & quad
	{
	public:
		ILine(Core::IVector3D pos1=Core::IVector3D(), Core::IVector3D pos2=Core::IVector3D());

		Core::IVector3D GetPos1();
		Core::IVector3D GetPos2();
		Core::IVector3D GetPos(int i);
		
	private:
		Core::IVector3D	m_pVertex[2];		
	};

}

#endif