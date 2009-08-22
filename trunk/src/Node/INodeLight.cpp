//---------------------------------------------
//                Initial Engine
//           Node light source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Node/INodeLight.h"
#include "Initial/Math/IMath.h"
#include "Initial/3D/IFrustum.h"
#include "Initial/IDevice.h"

using namespace Initial::Core;
using namespace Initial::Math;
using namespace Initial::Video;

int const RADIUS_QUALITY = 32;

namespace Initial
{	
	INodeLight::INodeLight(IDevice *device)
		: INode(device)
	{
		m_bSpot=false;
		m_fRadius=1.0;

		m_fFov=90.0;
		m_fExponent=128;

		AddProperty("Radius",IPT_FLOAT,&m_fRadius);
		AddProperty("Color",IPT_COLOR,&m_cColor);
	}

	void INodeLight::_InitNode()
	{
		if (m_pDevice && m_pDevice->GetRessourceManager())
		{
			m_pEmissiveVertex = m_pDevice->GetRessourceManager()->LoadMaterial("materials/engine/emissive-vertex.ima");
		}
	}

	Video::ITexture* INodeLight::GetProjectedTexture()
	{
		return m_pProjected;
	}

	
	void INodeLight::SetProjectedTexture(Video::ITexture* tex)
	{
		m_pProjected = tex;
	}

	bool INodeLight::IsSpot()
	{
		return m_bSpot;
	}

	void INodeLight::SetSpot(bool spot)
	{
		m_bSpot = spot;
	}

	Core::IColor INodeLight::GetColor()
	{
		return m_cColor;
	}

	void INodeLight::SetColor(Core::IColor color)
	{
		m_cColor = color;
	}

	float INodeLight::GetRadius()
	{
		return m_fRadius;
	}

	void INodeLight::SetRadius(float radius)
	{
		m_fRadius = radius;
	}

	float INodeLight::GetFov()
	{
		return m_fFov;
	}

	void INodeLight::SetFov(float fov)
	{
		m_fFov = fov;
	}

	float INodeLight::GetExponent()
	{
		return m_fExponent;
	}

	void INodeLight::SetExponent(float exp)
	{
		m_fExponent = exp;
	}

	void INodeLight::Render(Video::IRenderDriver *driver, IFrustum *frustum, int flags)
	{
		if (!m_bShow || !IsSelected())
			return;

		float cubeSize=0.1;
		//Draw Cube
		driver->UseMaterial(m_pEmissiveVertex);
		driver->_SetColor(IColor(1.0,1.0,1.0,1.0));
		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,cubeSize,cubeSize),GetPosition()+IVector3D(cubeSize,cubeSize,cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,-cubeSize,cubeSize),GetPosition()+IVector3D(cubeSize,-cubeSize,cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,cubeSize,-cubeSize),GetPosition()+IVector3D(cubeSize,cubeSize,-cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,-cubeSize,-cubeSize),GetPosition()+IVector3D(cubeSize,-cubeSize,-cubeSize));

		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,cubeSize,cubeSize),GetPosition()+IVector3D(-cubeSize,-cubeSize,cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(cubeSize,cubeSize,cubeSize),GetPosition()+IVector3D(cubeSize,-cubeSize,cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,cubeSize,-cubeSize),GetPosition()+IVector3D(-cubeSize,-cubeSize,-cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(cubeSize,cubeSize,-cubeSize),GetPosition()+IVector3D(cubeSize,-cubeSize,-cubeSize));

		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,cubeSize,cubeSize),GetPosition()+IVector3D(-cubeSize,cubeSize,-cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(cubeSize,-cubeSize,cubeSize),GetPosition()+IVector3D(cubeSize,-cubeSize,-cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(-cubeSize,-cubeSize,cubeSize),GetPosition()+IVector3D(-cubeSize,-cubeSize,-cubeSize));
		driver->_DrawLine(GetPosition()+IVector3D(cubeSize,cubeSize,cubeSize),GetPosition()+IVector3D(cubeSize,cubeSize,-cubeSize));

		IVector3D pos = GetPosition();
		//Draw Radius
		for (float i=0;i<=RADIUS_QUALITY;i++)
		{
			driver->_DrawLine(IVector3D(pos.GetX()+m_fRadius*cos(i/RADIUS_QUALITY*2*I_PIE),pos.GetY()+m_fRadius*sin(i/RADIUS_QUALITY*2*I_PIE),pos.GetZ()),
				IVector3D(pos.GetX()+m_fRadius*cos((i+1)/RADIUS_QUALITY*2*I_PIE),pos.GetY()+m_fRadius*sin((i+1)/RADIUS_QUALITY*2*I_PIE),pos.GetZ()));
			driver->_DrawLine(IVector3D(pos.GetX()+m_fRadius*cos(i/RADIUS_QUALITY*2*I_PIE),pos.GetY(),pos.GetZ()+m_fRadius*sin(i/RADIUS_QUALITY*2*I_PIE)),
				IVector3D(pos.GetX()+m_fRadius*cos((i+1)/RADIUS_QUALITY*2*I_PIE),pos.GetY(),pos.GetZ()+m_fRadius*sin((i+1)/RADIUS_QUALITY*2*I_PIE)));
			driver->_DrawLine(IVector3D(pos.GetX(),pos.GetY()+m_fRadius*sin(i/RADIUS_QUALITY*2*I_PIE),pos.GetZ()+m_fRadius*cos(i/RADIUS_QUALITY*2*I_PIE)),
				IVector3D(pos.GetX(),pos.GetY()+m_fRadius*sin((i+1)/RADIUS_QUALITY*2*I_PIE),pos.GetZ()+m_fRadius*cos((i+1)/RADIUS_QUALITY*2*I_PIE)));
		}

		//RenderFrustum
		/*IFrustum frustum2;
		for (int i=0;i<6;i++)
		{
			frustum2.SetFrustumFromMatrix(GetProjectionMatrix(i));
			frustum2.CalcVertice();
			frustum2.DrawFrustum(driver);
		}*/
		BaseClass::Render(driver,frustum,flags);
	}

	IMatrix INodeLight::GetProjectionMatrix(int i)
	{
		IMatrix invRot;
		//Spot
		/*invRot = GetRotationMatrix4x4();
		invRot.Inv(invRot);*/
		//Omni
		if (i==0)
		{
			invRot.MakeIdentity();
			invRot=AngleYToMatrix4x4(0);
		}else if (i==1)
		{
			invRot=AngleYToMatrix4x4(I_PIE);
		}else if (i==2)
		{
			invRot=AngleYToMatrix4x4(I_PIE/2.0);
		}else if (i==3)
		{
			invRot=AngleYToMatrix4x4(-I_PIE/2.0);
		}else if (i==4)
		{
			invRot=AngleXToMatrix4x4(I_PIE/2.0);
		}else if (i==5)
		{
			invRot=AngleXToMatrix4x4(-I_PIE/2.0);
		}
		IMatrix mat = GetPerspectiveMatrix(90,1,m_fRadius,0.01)*invRot*TranslationToMatrix(-GetPosition());
		return mat;
	}
}