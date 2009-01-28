//---------------------------------------------
//                Initial Engine
//               3D Object Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _I3DOBJECT_HEADER_
#define _I3DOBJECT_HEADER_

#include "Video/IRenderDriver.h"
#include "Core/IArray.h"
#include "Core/IMatrix.h"
#include "3D/ITriangle.h"
#include "3D/IBBox.h"

using namespace Initial::Math;

namespace Initial
{
	class I3DObject
	{
		friend class IRenderDriver;
	public:
		I3DObject(IRenderDriver *Device=NULL, I3DObject* parent=NULL);
		I3DObject(IRenderDriver *Device, IArray<ITriangle> polygons, I3DObject* parent=NULL);
		~I3DObject();

		void SetDevice(IRenderDriver *Device) { m_pDevice=Device; }

		virtual void Render(IRenderDriver *Device=NULL, unsigned int Flags=0);

		void SetFlags(unsigned int flag,bool enable);
		bool GetFlag(unsigned int flag);

		void AddPolygon(ITriangle pol);

		void SetMaterial(IMaterial *mat, bool ToChildren=true);
		void SetColor(IColor color, bool ToChildren=true);

		IMaterial *GetMaterial();
		IColor GetColor();

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

		void AddChild(I3DObject *child);
		I3DObject* GetParent();
		void SetParent(I3DObject* parent);

		void CalculateBBox(bool WithChild=true);
		IBBox GetBBox();

	protected:
		void _Init(IRenderDriver *Device, I3DObject* parent);

	protected:
		unsigned long m_iFlags;
		IRenderDriver *m_pDevice;
		IArray<ITriangle> m_aPolygons;
		IBBox m_BBox;
		unsigned int m_iDisplayListSolid;
		unsigned int m_iDisplayListWireframe;
		unsigned int m_iDisplayListNormal;
		
		IMatrix m_mOrigPosition;
		IMatrix m_mPosition;
		float m_fScale;

		IMaterial *m_pMaterial;
		IColor m_cColor;

		IArray<I3DObject*> m_aChildren;
		I3DObject* m_pParent;

		bool m_bUpdated;

		static IArray<I3DObject*> m_aObjects;
	};
}

#endif