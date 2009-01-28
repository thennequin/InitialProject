//---------------------------------------------
//                Initial Engine
//                 Line  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ILINE_HEADER_
#define _ILINE_HEADER_

#include "IVertex.h"

namespace Initial
{

	class ILine // triangle & quad
	{
	public:
		ILine(IVertex pos1=IVertex(), IVertex pos2=IVertex());

		IVertex GetPos1();
		IVertex GetPos2();
		IVertex GetPos(int i);
		
	private:
		IVertex	m_pVertex[2];		
	};

}

#endif