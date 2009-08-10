//---------------------------------------------
//                Initial Engine
//             Node Light Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INODELIGHT_HEADER_
#define _INODELIGHT_HEADER_

#include "Initial/Node/INode.h"
#include "Initial/Video/ITexture.h"

namespace Initial
{
	class INodeLight : public INode
	{
		DECLARE_OBJECT(INodeLight,INode)
	public:
		enum LightType
		{
			LT_OMNI,
			LT_SPOT,
			LT_DIRECTIONAL,
		};
	protected:
		INodeLight(IDevice *Device=NULL);
	public:

		virtual void Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags);

		Video::ITexture* GetProjectedTexture();
		void SetProjectedTexture(Video::ITexture* tex);

		bool IsSpot();
		void SetSpot(bool spot);

		Core::IColor GetColor();
		void SetColor(Core::IColor color);

		float GetRadius();
		void SetRadius(float radius);

		float GetFov();
		void SetFov(float fov);

		float GetExponent();
		void SetExponent(float exp);

		Math::IMatrix GetProjectionMatrix(int i=0);
	protected:	
		bool m_bSpot;
		Core::IColor m_cColor;
		float m_fRadius;

		//Spot
		float m_fFov;
		float m_fExponent; // For border
		ISmartPtr<Video::ITexture> m_pProjected;
	};
}

#endif