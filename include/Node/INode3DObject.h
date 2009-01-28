//---------------------------------------------
//                Initial Engine
//             Node 3D Object Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODE3DOBJECT_HEADER_
#define _INODE3DOBJECT_HEADER_

#include "3D/INode.h"

namespace Initial
{
	class INode3DObject : public INode
	{
	public:
		INode3DObject(IRenderDriver *Device);

		virtual void Render(IRenderDriver *device)=0;

		void SetObject(I3DObject *obj);
	protected:

		I3DObject*	m_pObject3D;
		/*IMatrix m_mOrigPosition;
		IMatrix m_mPosition;
		float m_fScale;

		IMaterial *m_pMaterial;
		IColor m_cColor;*/
	};
}

#endif