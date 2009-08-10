//---------------------------------------------
//                Initial Engine
//          3D Animated Object Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _I3DANIMATEDOBJECT_HEADER_
#define _I3DANIMATEDOBJECT_HEADER_

#include "Initial/Video/IRenderDriver.h"
#include "Initial/Core/IArray.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/IRessource.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/3D/IBBox.h"

class CalCoreModel;
class CalModel;

namespace Initial
{
	class IAnimatedMesh : public IRessource
	{
		friend class Video::IRenderDriver;
	public:
		virtual void Reload();

		IAnimatedMesh(Video::IRenderDriver *Device, Core::IString AnimatedMesh);
		~IAnimatedMesh();

		int LoadAnimation(Core::IString animation);
		int GetAnimation(Core::IString name);

		void SetDevice(Video::IRenderDriver *Device) { m_pDevice=Device; }

		virtual void Render(Video::IRenderDriver *Device=NULL, unsigned int Flags=0);

		void SetFlags(unsigned int flag,bool enable);
		bool GetFlag(unsigned int flag);

		void SetMaterial(Video::IMaterial *mat, bool ToChildren=true);
		void SetColor(Core::IColor color, bool ToChildren=true);

		Video::IMaterial *GetMaterial();
		Core::IColor GetColor();

		void Translate(float x, float y, float z);
		void RotateX(float x);
		void RotateY(float y);
		void RotateZ(float z);
		void InitPosition();

		void TranslateOrig(float x, float y, float z);
		void RotateOrigX(float x);
		void RotateOrigY(float y);
		void RotateOrigZ(float z);
		void InitOrigPosition();

		void SetScale(float scale);

	protected:
		void _Init(Video::IRenderDriver *Device);

	protected:
		unsigned long m_iFlags;
		Video::IRenderDriver *m_pDevice;
		IBBox m_BBox;
		
		Math::IMatrix m_mOrigPosition;
		Math::IMatrix m_mPosition;
		float m_fScale;

		Video::IMaterial *m_pMaterial;
		Core::IColor m_cColor;

		bool m_bUpdated;

		unsigned long StartTime;

		CalCoreModel* m_calCoreModel;
		CalModel* m_calModel;
	};
}

#endif