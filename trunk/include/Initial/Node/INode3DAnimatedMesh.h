//---------------------------------------------
//                Initial Engine
//             Node 3D Object Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODE3DANIMATEDMESH_HEADER_
#define _INODE3DANIMATEDMESH_HEADER_

#include "Initial/Node/INode.h"
//#include "Initial/Ressource/IAnimatedMesh.h"
#include "Initial/Format/IMeshIEM.h"
#include "Initial/Core/IString.h"

namespace Initial
{
	class INode3DAnimatedMesh : public INode
	{
		DECLARE_OBJECT(INode3DAnimatedMesh,INode)
	public:
		INode3DAnimatedMesh(IDevice *device=NULL);

		void SetAnimatedMesh(Core::IString file);
		void SetAnimatedMesh(ISmartPtr<Format::IMeshIEM> mesh);

		ISmartPtr<Format::IMeshIEM> GetAnimatedMesh();
	protected:
		ISmartPtr<Format::IMeshIEM> m_pMesh;
		CalModel* m_calModel;
	};
}

#endif