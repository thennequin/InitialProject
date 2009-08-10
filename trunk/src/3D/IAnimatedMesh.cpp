//---------------------------------------------
//                Initial Engine
//        3D Animated Object source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/3D/IAnimatedMesh.h"
#include "Initial/Math/IMath.h"

#include "cal3d/cal3d.h"

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;
using namespace Initial::Math;

IAnimatedMesh::IAnimatedMesh(IRenderDriver *Device, IString Mesh/*, IString Skeleton, IString Animation*/)
: IRessource(Mesh)
{
	_Init(Device);
	int mesh;
	int skel;
	int anim;
	IString Skeleton="Models/Skeleton/skeleton.csf";
	IString Animation="Models/Skeleton/skeleton_walk.caf";
	
	// Load skeleton
	skel = m_calCoreModel->loadCoreSkeleton(Skeleton.c_str());	
	if (CalError::getLastErrorCode()!=0)
		ILogger::LogError("%s\n",CalError::getLastErrorText().c_str());	

	// Load Meshs
	//mesh = m_calCoreModel->loadCoreMesh(Mesh.c_str());
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_calf_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_calf_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_calf_right.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_chest.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_foot_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_foot_right.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_hair.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_hand_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_hand_right.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_head.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_lowerarm_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_lowerarm_right.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_neck.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_pelvis.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_spine_lower.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_spine_upper.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_thigh_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_thigh_right.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_upperarm_left.cmf");
	mesh = m_calCoreModel->loadCoreMesh("Models/Skeleton/skeleton_upperarm_right.cmf");
	//m_calCoreModel->addCoreMesh(CalLoader::loadCoreMesh())

	if (CalError::getLastErrorCode()!=0)
		ILogger::LogError("%s\n",CalError::getLastErrorText().c_str());

	// Load animation
	anim = m_calCoreModel->loadCoreAnimation(Animation.c_str());
	if (CalError::getLastErrorCode()!=0)
		ILogger::LogError("%s\n",CalError::getLastErrorText().c_str());
	

	m_calCoreModel->getCoreSkeleton()->calculateBoundingBoxes(m_calCoreModel);

	//m_calCoreModel->getCoreMesh(0)->getCoreSubmesh(0)->getCoreSubMorphTarget(0)->getVectorBlendVertex();

	ILogger::LogDebug("%d %d %d\n",mesh,skel,anim);

	m_calModel = new CalModel(m_calCoreModel);
	if (m_calModel)
	{
		 // attach all meshes to the model
		int meshId;
		for(meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
		{
			m_calModel->attachMesh(meshId);
		}

		// set the material set of the whole model
		m_calModel->setMaterialSet(0);

		if (anim)
		{
			m_calModel->getMixer()->clearCycle(anim,0);
			m_calModel->getMixer()->blendCycle(anim, 1, 0.0f);
		}
		m_calModel->setLodLevel(1.0);
	}

	//return;
	StartTime = GetTickCount();
}

void IAnimatedMesh::_Init(IRenderDriver *Device)
{
	m_pDevice = Device;	
	///m_aObjects.Add(this);
	m_mPosition.MakeIdentity(4);
	m_mOrigPosition.MakeIdentity(4);
	m_fScale=1.0;
	m_bUpdated=false;
	m_iFlags=IRF_ALWAYS_RENDER|IRF_WHITE_WIREFRAME; // Default flags
	m_pMaterial=NULL;

	m_calCoreModel = new CalCoreModel("dummy");
}

IAnimatedMesh::~IAnimatedMesh()
{
	//m_aObjects.DeleteItem(this);
}

void IAnimatedMesh::Reload()
{

}

int IAnimatedMesh::LoadAnimation(IString animation)
{
	if (m_calCoreModel && 0)
	{
		int anim = m_calCoreModel->loadCoreAnimation(animation.c_str());
		ILogger::LogDebug("anim %d\n",anim);
		if (CalError::getLastErrorCode()!=0)
			ILogger::LogError("Error %d : %s\n",CalError::getLastErrorCode(),CalError::getLastErrorText().c_str());
		m_calModel->getMixer()->clearCycle(anim,0);
		m_calModel->getMixer()->blendCycle(anim, 1, 0.0f);
		return anim;
	}
	return -1;
}

int IAnimatedMesh::GetAnimation(IString name)
{
	return 0;
}

void IAnimatedMesh::Render(IRenderDriver *Device, unsigned int Flags)
{
	//return;
	m_calModel->update((GetTickCount()-StartTime)/1000.0);
	StartTime=GetTickCount();

	CalRenderer *pCalRenderer;
	pCalRenderer = m_calModel->getRenderer();

	// begin the rendering loop
	if(!pCalRenderer->beginRendering()) return;

	IRenderDriver *dev;
	if (Device)
		dev=Device;
	else
		dev=m_pDevice;

	unsigned int flags=m_iFlags;
	if (Flags)
		m_iFlags=Flags;	

	if (dev)
	{
		/*RF_ALWAYS_RENDER
		RF_SHOW_BOUNDING_BOX
		RF_SHOW_NORMAL
		RF_WIREFRAME
		RF_WIREFRAME_ONLY*/
		dev->_SetModelViewMatrix();

		dev->_PushMatrix();

			IMatrix ScaleMat(4);
			ScaleMat.MakeIdentity();
			ScaleMat[0][0]=ScaleMat[1][1]=ScaleMat[2][2]=m_fScale;

			dev->_MultMatrix(m_mOrigPosition);
			dev->_MultMatrix(m_mPosition);
			dev->_MultMatrix(ScaleMat);			

			//Solid
			//if (!GetFlag(IRF_WIREFRAME_ONLY))
			{		
				dev->_SetColor(m_cColor);
				dev->UseMaterial(m_pMaterial);

				dev->_SetColor(IColor(1,1,1));

				// draw the bone lines
				float lines[1024][2][3];
				int nrLines;
				nrLines =  m_calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);
				//nrLines = m_calModel->getSkeleton()->getBoneLinesStatic(&lines[0][0][0]);

				int currLine;
				for(currLine = 0; currLine < nrLines; currLine++)
				{
					dev->_DrawLine(	IVector3D(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]),
									IVector3D(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]));
				}

				// draw the bone points
				float points[1024][3];
				int nrPoints;
				nrPoints =  m_calModel->getSkeleton()->getBonePoints(&points[0][0]);
				//nrPoints = m_calModel->getSkeleton()->getBonePointsStatic(&points[0][0]);

				dev->_SetColor(IColor(0,0,1));
				int currPoint;
				for(currPoint = 0; currPoint < nrPoints; currPoint++)
				{
				  dev->_DrawLine(IVector3D(points[currPoint][0], points[currPoint][1], points[currPoint][2]),IVector3D(points[currPoint][0], points[currPoint][1], points[currPoint][2])+IVector3D(0.01,0.01,0.01));
				}

				dev->_SetColor(IColor(1,1,1));
				// get the number of meshes
				int meshCount;
				meshCount = pCalRenderer->getMeshCount();
				//printf("Mesh count %d\n",meshCount);

				//pCalRenderer->getVertices();

				// render all meshes of the model
				int meshId;
				for(meshId = 0; meshId < meshCount; meshId++)
				{
					// get the number of submeshes
					int submeshCount;
					submeshCount = pCalRenderer->getSubmeshCount(meshId);
					//printf("\tSubMesh count %d\n",submeshCount);

					// render all submeshes of the mesh
					int submeshId;
					for(submeshId = 0; submeshId < submeshCount; submeshId++)
					{
						// select mesh and submesh for further data access
						if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
						{
							// get the transformed vertices of the submesh
							static float meshVertices[30000][3];
							int vertexCount;
							vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

							//printf("\tVertex Count %d\n",vertexCount);

							// get the transformed normals of the submesh
							static float meshNormals[30000][3];
							pCalRenderer->getNormals(&meshNormals[0][0]);

							// get the texture coordinates of the submesh
							static float meshTextureCoordinates[30000][2];
							int textureCoordinateCount;
							textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

							// get the faces of the submesh
							static CalIndex meshFaces[50000][3];
							int faceCount;
							faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

							ITriangle tri;
							for (int i=0;i<faceCount;i++)
							{
								tri.SetVertex(0,IVector3D(meshVertices[meshFaces[i][0]][0],meshVertices[meshFaces[i][0]][1],meshVertices[meshFaces[i][0]][2]));
								tri.SetVertex(1,IVector3D(meshVertices[meshFaces[i][1]][0],meshVertices[meshFaces[i][1]][1],meshVertices[meshFaces[i][1]][2]));
								tri.SetVertex(2,IVector3D(meshVertices[meshFaces[i][2]][0],meshVertices[meshFaces[i][2]][1],meshVertices[meshFaces[i][2]][2]));

								tri.SetNormal(0,IVector3D(meshNormals[meshFaces[i][0]][0],meshNormals[meshFaces[i][0]][1],meshNormals[meshFaces[i][0]][2]));
								tri.SetNormal(1,IVector3D(meshNormals[meshFaces[i][1]][0],meshNormals[meshFaces[i][1]][1],meshNormals[meshFaces[i][1]][2]));
								tri.SetNormal(2,IVector3D(meshNormals[meshFaces[i][2]][0],meshNormals[meshFaces[i][2]][1],meshNormals[meshFaces[i][2]][2]));

								tri.SetCoord(0,IVector2D(meshTextureCoordinates[meshFaces[i][0]][0],meshTextureCoordinates[meshFaces[i][0]][1]));
								tri.SetCoord(1,IVector2D(meshTextureCoordinates[meshFaces[i][1]][0],meshTextureCoordinates[meshFaces[i][1]][1]));

								dev->_StartTriangleDraw(false);
									dev->_DrawTriangle(&tri);
								dev->_EndPolyDraw();
							}

						}
					}
				}
			}
		
		dev->_PopMatrix();
	}
	m_iFlags=flags;

	pCalRenderer->endRendering();
}

void IAnimatedMesh::SetFlags(unsigned int flag,bool enable) 
{ 
	m_iFlags = enable ? m_iFlags|flag : m_iFlags&~flag; 
}

bool IAnimatedMesh::GetFlag(unsigned int flag) 
{ 
	return m_iFlags&flag ? true : false; 
}

void IAnimatedMesh::SetMaterial(IMaterial *mat, bool ToChildren)
{
	m_pMaterial = mat;
}

void IAnimatedMesh::SetColor(IColor color, bool ToChildren)
{
	m_cColor=color;
}

IMaterial* IAnimatedMesh::GetMaterial()
{
	return m_pMaterial;
}

IColor IAnimatedMesh::GetColor()
{
	return m_cColor;
}

void IAnimatedMesh::Translate(float x, float y, float z)
{
	m_mPosition*=TranslationToMatrix(x,y,z);;
}

void IAnimatedMesh::RotateX(float x)
{
	m_mPosition*=AngleXToMatrix4x4(x);
}

void IAnimatedMesh::RotateY(float y)
{
	m_mPosition*=AngleYToMatrix4x4(y);
}

void IAnimatedMesh::RotateZ(float z)
{
	m_mPosition*=AngleZToMatrix4x4(z);
}

void IAnimatedMesh::InitPosition()
{
	m_mPosition.MakeIdentity();
}

void IAnimatedMesh::TranslateOrig(float x, float y, float z)
{
	m_mOrigPosition*=TranslationToMatrix(x,y,z);
}

void IAnimatedMesh::RotateOrigX(float x)
{
	m_mOrigPosition*=AngleXToMatrix4x4(x);
}

void IAnimatedMesh::RotateOrigY(float y)
{
	m_mOrigPosition*=AngleYToMatrix4x4(y);
}

void IAnimatedMesh::RotateOrigZ(float z)
{
	m_mOrigPosition*=AngleZToMatrix4x4(z);
}

void IAnimatedMesh::InitOrigPosition()
{
	m_mPosition.MakeIdentity();
}

void IAnimatedMesh::SetScale(float scale)
{
	m_fScale=scale;
}

/*IBBox IAnimatedMesh::GetBBox()
{
	return m_BBox;
}*/
