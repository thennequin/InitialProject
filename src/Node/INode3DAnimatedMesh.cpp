//---------------------------------------------
//                Initial Engine
//    Node 3D Animated Object source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INode3DAnimatedMesh.h"
#include "Initial/IDevice.h"
#include "Initial/IRessourceManager.h"

using namespace Initial::Core;
using namespace Initial::Video;

//IMPLEMENT_OBJECT(INode3DAnimatedMesh,INode)

namespace Initial
{	
	INode3DAnimatedMesh::INode3DAnimatedMesh(IDevice *device)
		: INode(device)
	{
	}

	void INode3DAnimatedMesh::SetAnimatedMesh(Core::IString file)
	{
		//if (m_pDevice)
			m_pMesh = m_pDevice->GetRessourceManager()->LoadAnimatedMesh(file);
	}

	void INode3DAnimatedMesh::SetAnimatedMesh(ISmartPtr<Format::IMeshIEM> mesh)
	{
		m_pMesh=mesh;
	}

	ISmartPtr<Format::IMeshIEM> INode3DAnimatedMesh::GetAnimatedMesh()
	{
		return m_pMesh;
	}
}
