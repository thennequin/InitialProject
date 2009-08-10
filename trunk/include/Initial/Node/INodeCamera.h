//---------------------------------------------
//                Initial Engine
//              Node Camera Module
//
//            By Thibault HENNEQUIN
//              January 18th 2008
//---------------------------------------------

#ifndef _INODECAMERA_HEADER_
#define _INODECAMERA_HEADER_

#include "Initial/Node/INode.h"

namespace Initial
{
	class INodeCamera : public INode
	{
		DECLARE_OBJECT(INodeCamera,INode)
	public:
		INodeCamera(IDevice *device=NULL);

		void SetFov(float fov);
		float GetFov();

		virtual Math::IMatrix GetUpdateViewMatrix();

		void SetRatio(float ratio);
		void SetzFar(float z);
		void SetzNear(float z);
	protected:
		float m_fFov;
		float m_fRatio;
		float m_fzFar;
		float m_fzNear;

		bool m_bAutoZ; // Optimize Z space between zNear and zFar
	};
}

#endif