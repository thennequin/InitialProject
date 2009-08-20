//---------------------------------------------
//                Initial Engine
//              Node source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INode.h"
#include "Initial/Math/IMath.h"
#include "Initial/IDevice.h"

#include "Initial/Node/INodeContainer.h"
#include "Initial/Node/INodeCamera.h"
#include "Initial/Node/INodeLight.h"
#include "Initial/Node/INodeSkybox.h"
#include "Initial/Node/INode3DObject.h"
#include "Initial/Node/INode3DAnimatedMesh.h"
#include "Initial/Node/INodeText.h"

using namespace Initial::Core;
using namespace Initial::Math;
using namespace Initial::Video;

namespace Initial
{	
	IMPLEMENT_ABSTRACT_OBJECT(INode,IObject)

	IMPLEMENT_OBJECT(INodeContainer,INode)
	IMPLEMENT_OBJECT(INodeCamera,INode)
	IMPLEMENT_OBJECT(INodeLight,INode)
	IMPLEMENT_OBJECT(INodeSkybox,INode)
	IMPLEMENT_OBJECT(INode3DObject,INode)
	IMPLEMENT_OBJECT(INode3DAnimatedMesh,INode)
	IMPLEMENT_OBJECT(INodeText,INode)


	INode::INode(IDevice *device)
	{
		m_pDevice=device;
		m_vPosition.Set(0.0,0.0,0.0);
		m_vScale.Set(1.0,1.0,1.0);
		m_mAngle.MakeIdentity(3);
		m_pParent=NULL;
		m_bSelected=false;
		m_bShow=true;

		AddProperty("Name",IPT_STRING,&m_sName);
		AddProperty("Show",IPT_BOOL,&m_bShow);
		//if (IsMovable())
			AddProperty("Position",IPT_VEC3,&m_vPosition);
		//if (IsScalable())
			AddProperty("Scale",IPT_VEC3,&m_vScale);	
		//if (IsRotatable())
	}

	INode::~INode()
	{
		FOREACH(INode*,ite,m_aChildren)
		{
			if (ite.GetData())
			{
				delete ite.GetData();
			}
		}
	}

	void INode::InitNode()
	{
		m_sName=GetClass()->GetName();
		_InitNode();
	}

	void INode::SetDevice(IDevice *device)
	{
		m_pDevice=device;
	}

	IDevice* INode::GetDevice()
	{
		return m_pDevice;
	}

	INode* INode::AddNodeByClass(Core::IString nodeClassName)
	{
		for (UINT i=0;i<IObject::ObjectsClass.Count();i++)
		{
			//printf("%s\n",IObject::ObjectsClass[i]->GetName());
			if (IObject::ObjectsClass[i]
				&& IObject::ObjectsClass[i]->GetName()==nodeClassName
				&& IObject::ObjectsClass[i]->IsKindOf(INode::GetStaticClass()))
			{
				INode *node = (INode*)IObject::ObjectsClass[i]->CreateInstance();
				if (node)
				{
					node->SetDevice(m_pDevice);
					node->InitNode();
					node->SetParent(this);
				}
				return node;
			}
		}
		return NULL;
	}

	void INode::AddNode(INode *node)
	{
		if (node)
		{
			m_aChildren.PushBack(node);
			node->m_pParent=this;
		}
	}

	void INode::DeleteNode(INode *node)
	{
		if (node)
			m_aChildren.DeleteItem(node);
	}

	UINT INode::GetNodeCount()
	{
		return m_aChildren.Count();
	}

	INode* INode::GetNode(UINT nodeIndex)
	{
		if (nodeIndex>=0 && nodeIndex<m_aChildren.Count())
		{
			return m_aChildren[nodeIndex];
		}
		return NULL;
	}

	void INode::SetParent(INode *node)
	{
		if (!IsChild(node) /*&& node!=m_pParent*/)
		{
			if (m_pParent)
			{
				m_pParent->DeleteNode(this);
			}

			if (node)
			{
				node->AddNode(this);
			}
		}		
	}

	bool INode::IsChild(INode *node)
	{
		FOREACH(INode*,ite,m_aChildren)
		{
			if (ite.GetData())
			{
				if (ite.GetData()==node)
					return true;
				bool res = ite.GetData()->IsChild(node);
				if (res)
					return true;
			}
		}
		return false;
	}

	IList<INode*> INode::GetNodeByClass(IObject::IObjectInfo *classinfo, bool recursive, bool noSubClass)
	{
		IList<INode*> result;
		FOREACH(INode*,ite,m_aChildren)
		{
			if (ite.GetData())
			{
				INode *node = ite.GetData();
				if (noSubClass)
				{
					if (node->GetStaticClass()==classinfo)
						result.PushBack(node);
				}else{
					if (node->IsKindOf(classinfo))
						result.PushBack(node);
				}

				if (recursive)
				{
					IList<INode*> resultRec = node->GetNodeByClass(classinfo,true);
					for (UINT j=0;j<resultRec.Count();j++)
						result.PushBack(resultRec[j]);
				}
			}
		}
		return result;
	}

	IList<INode*> INode::GetNodeByName(IString name, bool recursive)
	{
		IList<INode*> result;
		FOREACH(INode*,ite,m_aChildren)
		{
			if (ite.GetData())
			{
				INode *node = ite.GetData();
				if (name==node->m_sName)
					result.PushBack(node);

				if (recursive)
				{
					IList<INode*> resultRec = node->GetNodeByName(name,true);
					for (UINT j=0;j<resultRec.Count();j++)
						result.PushBack(resultRec[j]);
				}
			}
		}
		return result;
	}

	void INode::Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags)
	{
		if (!m_bShow)
			return;
		FOREACH(INode*,ite,m_aChildren)
		{
			if (ite.GetData())
			{
				ite.GetData()->Render(driver,frustum,flags);
			}
		}
	}

	bool INode::IsScalable()
	{
		return true;
	}

	bool INode::IsRotatable()
	{
		return true;
	}

	bool INode::IsMovable()
	{
		return true;
	}

	void INode::SetScale(IVector3D scale)
	{
		m_vScale = scale;
	}

	void INode::SetScale(float x, float y, float z)
	{
		SetScale(IVector3D(x,y,z));
	}

	IVector3D INode::GetScale()
	{
		return m_vScale;
	}

	void INode::SetPosition(IVector3D pos)
	{
		m_vPosition = pos;
	}

	void INode::SetPosition(float x, float y, float z)
	{
		SetPosition(IVector3D(x,y,z));
	}

	IVector3D INode::GetPosition()
	{
		return m_vPosition;
	}

	void INode::SetRotation(IVector3D rot)
	{
	}

	void INode::SetRotation(float x, float y, float z)
	{
		SetRotation(IVector3D(x,y,z));
	}

	//TODO
	IVector3D INode::GetRotation()
	{
		return IVector3D();
	}

	void INode::Move(Core::IVector3D mov, bool local)
	{
		if (local)
		{
			m_vPosition=m_vPosition+m_mAngle*mov;
		}else
			m_vPosition=m_vPosition+mov;
	}

	void INode::Move(float x, float y, float z, bool local)
	{
		Move(IVector3D(x,y,z),local);
	}

	void INode::RotateX(float angle, bool local)
	{
		if (local)
			m_mAngle=AngleXToMatrix3x3(angle)*m_mAngle;
		else
			m_mAngle=m_mAngle*AngleXToMatrix3x3(angle);
	}

	void INode::RotateY(float angle, bool local)
	{
		if (local)
			m_mAngle=AngleYToMatrix3x3(angle)*m_mAngle;
		else
			m_mAngle=m_mAngle*AngleYToMatrix3x3(angle);
	}

	void INode::RotateZ(float angle, bool local)
	{
		if (local)
			m_mAngle=AngleZToMatrix3x3(angle)*m_mAngle;
		else
			m_mAngle=m_mAngle*AngleZToMatrix3x3(angle);
	}

	//TODO
	void INode::RotateFromAxis(float angle, Core::IVector3D axis)
	{

	}

	Math::IMatrix INode::GetScaleMatrix()
	{
		return ScaleToMatrix(m_vScale);
	}

	Math::IMatrix INode::GetRotationMatrix3x3()
	{
		return m_mAngle;
	}

	Math::IMatrix INode::GetRotationMatrix4x4()
	{
		IMatrix mat(4,4);
		mat.MakeIdentity();
		for (int i=0;i<3;i++)
		{
			for (int j=0;j<3;j++)
			{
				mat(i,j)=m_mAngle(i,j);
			}
		}
		return mat;
	}

	Math::IMatrix INode::GetPositionMatrix()
	{
		return TranslationToMatrix(m_vPosition);
	}
}