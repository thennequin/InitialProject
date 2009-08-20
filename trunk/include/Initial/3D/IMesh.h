//---------------------------------------------
//                Initial Engine
//               3D Object Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _I3DOBJECT_HEADER_
#define _I3DOBJECT_HEADER_

#include "Initial/Video/IRenderDriver.h"
#include "Initial/Core/IArray.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/IObject.h"
#include "Initial/3D/ITriangle.h"
#include "Initial/3D/IBBox.h"

#include <vector>

using namespace Initial::Math;

namespace Initial
{
	class IMeshLoader
	{

	};
	class IMesh : public IObject
	{
		friend class Video::IRenderDriver;
		friend class IMeshLoader;
	public:
		/*#pragma pack(push, 1)
		struct MeshIndex
		{
			unsigned long VerticeIndex[3];
			unsigned long NormalIndex[3];
			unsigned long TextureIndex[3];
		};
		#pragma pack(pop)
		struct MeshData
		{
			//unsigned long iMaterialId;
			unsigned long iVerticeCount;
			unsigned long iNormalsCount;
			unsigned long iTexCoordCount;
			unsigned long iIndexCount;

			float (*pVertices)[3];
			float (*pNormals)[3];
			float (*pTexCoords)[2];

			MeshIndex* pIndex;
		};*/

		struct MeshIndex
		{
			unsigned long VerticeIndex[3];
			unsigned long NormalIndex[3];
			unsigned long TextureIndex[3];
		};
		struct SubMesh
		{
			std::vector<Core::IVector3D> m_Vertices;
			std::vector<Core::IVector3D> m_Normal;
			std::vector<Core::IVector3D> m_Tangent;
			std::vector<Core::IVector2D> m_Coord;
			std::vector<MeshIndex> m_Index;			
		};
		//
	public:
		IMesh(Video::IRenderDriver *Device=NULL, IMesh* parent=NULL);
		IMesh(Video::IRenderDriver *Device, Core::IArray<ITriangle> polygons, IMesh* parent=NULL);
		~IMesh();

		void SetDevice(Video::IRenderDriver *Device) { m_pDevice=Device; }

		virtual void Render(Video::IRenderDriver *Device=NULL, unsigned int Flags=0);

		void SetFlags(unsigned int flag,bool enable);
		bool GetFlag(unsigned int flag);

		void AddPolygon(ITriangle pol);

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

		void AddChild(IMesh *child);
		int GetChildCount();
		IMesh* GetChild(int i);

		IMesh* GetParent();
		void SetParent(IMesh* parent);

		void CalculateBBox(bool WithChild=true);
		IBBox GetBBox();

		void CalcNormals();

	protected:
		void _Init(Video::IRenderDriver *Device, IMesh* parent);

	protected:
		unsigned long m_iFlags;
		Video::IRenderDriver *m_pDevice;
		Core::IArray<ITriangle> m_aPolygons;
		IBBox m_BBox;
		unsigned int m_iDisplayListSolid;
		unsigned int m_iDisplayListWireframe;
		unsigned int m_iDisplayListNormal;
		
		Math::IMatrix m_mOrigPosition;
		Math::IMatrix m_mPosition;
		float m_fScale;

		Video::IMaterial *m_pMaterial;
		Core::IColor m_cColor;

		Core::IArray<IMesh*> m_aChildren;
		IMesh* m_pParent;

		bool m_bUpdated;

		static Core::IArray<IMesh*> m_aObjects;

		std::vector<SubMesh> m_SubMeshs;
		std::vector<ISmartPtr<Video::IMaterial>> m_Materials;
	};
}

#endif