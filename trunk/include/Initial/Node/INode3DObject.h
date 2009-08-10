//---------------------------------------------
//                Initial Engine
//             Node 3D Object Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODE3DOBJECT_HEADER_
#define _INODE3DOBJECT_HEADER_

#include "Initial/Node/INode.h"

namespace Initial
{
	class INode3DObject : public INode
	{
		DECLARE_OBJECT(INode3DObject,INode)
	public:
		INode3DObject(IDevice *device=NULL);

		//void SetObject(IMesh *obj);
	protected:
		//ISmartPtr<IMesh>	m_pObject;
	};
}

#endif