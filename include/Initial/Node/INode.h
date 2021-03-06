//---------------------------------------------
//                Initial Engine
//                 Node  Module
//
//            By Thibault HENNEQUIN
//              January 18th 2008
//---------------------------------------------

#ifndef _INODE_HEADER_
#define _INODE_HEADER_

#include "Initial/IObject.h"
#include "Initial/Core/IList.h"
#include "Initial/Core/IString.h"
#include "Initial/Core/IVector3D.h"
#include "Initial/Math/IMatrix.h"
#include "Initial/Video/IRenderDriver.h"

//using namespace Initial::Math;

namespace Initial
{
	class IDevice;
	class IFrustum;

	class INode : public IObject
	{
		DECLARE_OBJECT(INode,IObject);
	protected:
		INode(IDevice *device=NULL);
		//virtual ~INode()=0;
	public:
		~INode();

		void InitNode();		

		//Return name of bitmap used in editor
		virtual Core::IString GetBitmapName() { return ""; }

		void SetDevice(IDevice *device);
		IDevice* GetDevice();

		Core::IString GetName() { return m_sName; }
		void SetName(Core::IString name) { m_sName=name; }
		Core::IString GetCategorie() { return Categorie; }

		bool IsSelected() { return m_bSelected; }
		void Select(bool selected=true) { m_bSelected=selected; }

		INode *AddNodeByClass(Core::IString nodeClassName);
		void AddNode(INode *node);
		void DeleteNode(INode *node);

		UINT GetNodeCount();
		INode *GetNode(UINT nodeIndex);

		void SetParent(INode *node);
		bool IsChild(INode *node);

		/**
			bool nosuper : just this class, not sub class
		**/
		Core::IList<INode*> GetNodeByClass(IObject::IObjectInfo *classinfo, bool recursive=true, bool noSubClass=false);
		Core::IList<INode*> GetNodeByName(Core::IString name, bool recursive=true);

		Core::IList<INode*> GetSelectedNode();
		void ResetSelection();

		virtual bool IsVisible(Video::IRenderDriver *driver)/*=0*/{ return true; };
		virtual void Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags=0);

		virtual void SetScale(Core::IVector3D scale);
		virtual void SetScale(float x, float y, float z);
		virtual Core::IVector3D GetScale();
		virtual void SetPosition(Core::IVector3D pos);
		virtual void SetPosition(float x, float y, float z);
		virtual Core::IVector3D GetPosition();
		virtual void SetRotation(Core::IVector3D rot);
		virtual void SetRotation(float x, float y, float z);
		virtual Core::IVector3D GetRotation();

		virtual void Move(Core::IVector3D mov, bool local=false);
		virtual void Move(float x, float y, float z, bool local=false);
		virtual void RotateX(float angle, bool local=false);
		virtual void RotateY(float angle, bool local=false);
		virtual void RotateZ(float angle, bool local=false);

		/*virtual void MoveFromAxis(Core::IVector3D mov, Core::IVector3D axis);
		virtual void MoveFromAxis(float x, float y, float z, Core::IVector3D axis);*/
		virtual void RotateFromAxis(float angle, Core::IVector3D axis);


		//Math::IMatrix
		virtual Math::IMatrix GetScaleMatrix();
		virtual Math::IMatrix GetRotationMatrix3x3();
		virtual Math::IMatrix GetRotationMatrix4x4();
		virtual Math::IMatrix GetPositionMatrix();

		virtual bool IsScalable();
		virtual bool IsRotatable();
		virtual bool IsMovable();

		/*virtual bool RayIntersectObject(Core::IVector3D start, Core::IVector3D dir, Core::IVector3D& res, bool twoside=false)=0;*/

		//virtual void Render(int flag=RF_NONE, ICamera *cam=NULL)=0;

		//virtual int InFrustumView(IFrustum *frustum)=0;

		//virtual IBBox GetBBox()=0;

		//Editing
		void StartDrag();
		Core::IVector3D GetDragPosition();

	protected:
		virtual void _InitNode() {}
	protected:
		IDevice *m_pDevice;

		Core::IString Categorie;
		Core::IString m_sName;
		bool m_bSelected;
		bool m_bShow;

		Core::IList<INode*> m_aChildren;
		INode *m_pParent;

		Core::IVector3D m_vPosition;
		Core::IVector3D m_vScale;
		Math::IMatrix m_mAngle; // Matrix 3x3
		//Core::IVector3D m_vAngle; // Use for property

		Core::IVector3D m_vStartDragPosition;
	};

}
#else
namespace Initial
{
	class INode;
};
#endif