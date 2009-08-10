

#ifndef _ILIGHT_HEADER_
#define _ILIGHT_HEADER_

#include "Initial/Core/IVector3D.h"
#include "Initial/Core/IColor.h"
#include "Initial/Core/IArray.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Video/IFrameBuffer.h"


namespace Initial
{
	class ILight
	{
	public:
		ILight(Core::IColor color, Core::IVector3D position, float radius, Video::IRenderDriver *driver);
		~ILight();
		void SetPosition(Core::IVector3D pos);
		Core::IVector3D GetPosition();

		void DrawShadow();
		Video::ITexture *GetShadowTexture();

	//protected:
		Core::IVector3D m_vPosition;
		Core::IColor m_cColor;
		float m_fRadius;
		bool m_bGodRay;

		Video::IRenderDriver *m_pRenderDriver;
		Video::IFrameBuffer *m_pFrameBuffer;
			Video::ITexture *m_pShadowTexture;

		static Core::IArray<ILight*> m_aLights;
	};
};

#endif