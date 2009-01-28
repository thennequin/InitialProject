//---------------------------------------------
//                Initial Engine
//              Line source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/ILine.h"

using namespace Initial;

ILine::ILine(IVertex pos1, IVertex pos2)
{ 
	m_pVertex[0]=pos1; 
	m_pVertex[1]=pos2;
}

IVertex ILine::GetPos1() 
{ 
	return m_pVertex[0]; 
}

IVertex ILine::GetPos2() 
{ 
	return m_pVertex[1]; 
}

IVertex ILine::GetPos(int i) 
{ 
	if (i>=0 && i<2) 
		return m_pVertex[i]; 
	else 
		return IVertex(); 
}