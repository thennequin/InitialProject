//---------------------------------------------
//                Initial Engine
//              Line source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/ILine.h"

using namespace Initial;
using namespace Initial::Core;

ILine::ILine(IVector3D pos1, IVector3D pos2)
{ 
	m_pVertex[0]=pos1; 
	m_pVertex[1]=pos2;
}

IVector3D ILine::GetPos1() 
{ 
	return m_pVertex[0]; 
}

IVector3D ILine::GetPos2() 
{ 
	return m_pVertex[1]; 
}

IVector3D ILine::GetPos(int i) 
{ 
	if (i>=0 && i<2) 
		return m_pVertex[i]; 
	else 
		return IVector3D(); 
}