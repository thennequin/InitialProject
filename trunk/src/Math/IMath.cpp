//---------------------------------------------
//                Initial Engine
//              Math source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/IBBox.h"
#include "Initial/3D/Camera/ICamera.h"
#include "Initial/3D/IPolygon.h"
#include "Initial/Math/IMath.h"
#include <math.h>

using namespace Initial::Core;

namespace Initial
{
	namespace Math
	{
		IMatrix TranslationToMatrix(float x, float y, float z)
		{
			IMatrix trans(4);
			trans.MakeIdentity();
			trans[3][0]=x;
			trans[3][1]=y;
			trans[3][2]=z;
			return IMatrix(trans);
		}

		IMatrix ScaleToMatrix(float x, float y, float z)
		{
			IMatrix scale(4);
			scale.MakeIdentity();
			scale[0][3]=x;
			scale[1][3]=y;
			scale[2][3]=z;
			return IMatrix(scale);
		}

		IMatrix TranslationToMatrix(IVector3D vec)
		{
			return TranslationToMatrix(vec.x,vec.y,vec.z);
		}

		IMatrix ScaleToMatrix(IVector3D vec)
		{
			return ScaleToMatrix(vec.x,vec.y,vec.z);
		}

		IMatrix AngleXToMatrix4x4(float angle)
		{
			IMatrix rot(4);
			rot.MakeIdentity();
			float sinval=sinf(angle);
			float cosval=cosf(angle);
			rot[1][1]=rot[2][2]=cosval;
			rot[2][1]=-sinval;
			rot[1][2]=sinval;
			return IMatrix(rot);
		}

		IMatrix AngleYToMatrix4x4(float angle)
		{
			IMatrix rot(4);
			rot.MakeIdentity();
			float sinval=sinf(angle);
			float cosval=cosf(angle);
			rot[0][0]=rot[2][2]=cosval;
			rot[2][0]=sinval;
			rot[0][2]=-sinval;
			return IMatrix(rot);
		}

		IMatrix AngleZToMatrix4x4(float angle)
		{
			IMatrix rot(4);
			rot.MakeIdentity();
			float sinval=sinf(angle);
			float cosval=cosf(angle);
			rot[0][0]=rot[1][1]=cosval;
			rot[1][0]=-sinval;
			rot[0][1]=sinval;
			return IMatrix(rot);
		}

		IMatrix AngleXToMatrix3x3(float angle)
		{
			IMatrix rot(3);
			rot.MakeIdentity();
			float sinval=sinf(angle);
			float cosval=cosf(angle);
			rot[1][1]=rot[2][2]=cosval;
			rot[2][1]=-sinval;
			rot[1][2]=sinval;
			return IMatrix(rot);
		}

		IMatrix AngleYToMatrix3x3(float angle)
		{
			IMatrix rot(3);
			rot.MakeIdentity();
			float sinval=sinf(angle);
			float cosval=cosf(angle);
			rot[0][0]=rot[2][2]=cosval;
			rot[2][0]=sinval;
			rot[0][2]=-sinval;
			return IMatrix(rot);
		}

		IMatrix AngleZToMatrix3x3(float angle)
		{
			IMatrix rot(3);
			rot.MakeIdentity();
			float sinval=sinf(angle);
			float cosval=cosf(angle);
			rot[0][0]=rot[1][1]=cosval;
			rot[1][0]=-sinval;
			rot[0][1]=sinval;
			return IMatrix(rot);
		}

		IVector3D GetDegreesFromMatrix3x3(IMatrix& mat)
		{
			double EPS=0.00000001;
			double Y = -asin(mat[0][2]);
			double C = cos(Y);
			
			double rotx, roty, X, Z;

			if (fabs(C)>EPS)
			{
				double invC = 1.0/C;
				rotx = mat[2][2] * invC;
				roty = mat[1][2] * invC;
				X = atan2( roty, rotx );
				rotx = mat[0][0] * invC;
				roty = mat[0][1] * invC;
				Z = atan2( roty, rotx );
			}else
			{
				X = 0.0;
				rotx = mat[1][1];
				roty = -mat[1][0];
				Z = atan2( roty, rotx );
			}

			/*if (X < 0.0) X += 2*I_PIE;
			if (Y < 0.0) Y += 2*I_PIE;
			if (Z < 0.0) Z += 2*I_PIE;*/

			return IVector3D(RAD_TO_DEG(X),RAD_TO_DEG(Y),RAD_TO_DEG(Z));
		}

		IMatrix GetMatrix3x3FromDegrees(IVector3D vect)
		{
			IMatrix mat(3,3);

			double cr = cos( DEG_TO_RAD(vect.GetX()) );
			double sr = sin( DEG_TO_RAD(vect.GetX()) );
			double cp = cos( DEG_TO_RAD(vect.GetY()) );
			double sp = sin( DEG_TO_RAD(vect.GetY()) );
			double cy = cos( DEG_TO_RAD(vect.GetZ()) );
			double sy = sin( DEG_TO_RAD(vect.GetZ()) );
			double srsp = sr*sp;
			double crsp = cr*sp;

			mat[0][0]=cp*cy;
			mat[1][0]=cp*sy;
			mat[2][0]=-sp;

			mat[0][1]=srsp*cy-cr*sy;
			mat[1][1]=srsp*sy+cr*cy;
			mat[2][1]=sr*cp;

			mat[0][2]=crsp*cy+sr*sy;
			mat[1][2]=crsp*sy-sr*cy;
			mat[2][2]=cr*cp;

			return mat;
		}

		IMatrix GetInverseMatrix3x3FromDegrees(IVector3D vect)
		{
			IMatrix mat(3,3);

			double cr = cos( DEG_TO_RAD(vect.GetX()) );
			double sr = sin( DEG_TO_RAD(vect.GetX()) );
			double cp = cos( DEG_TO_RAD(vect.GetY()) );
			double sp = sin( DEG_TO_RAD(vect.GetY()) );
			double cy = cos( DEG_TO_RAD(vect.GetZ()) );
			double sy = sin( DEG_TO_RAD(vect.GetZ()) );
			double srsp = sr*sp;
			double crsp = cr*sp;

			mat[0][0]=cp*cy;
			mat[0][1]=cp*sy;
			mat[0][2]=-sp;

			mat[1][0]=srsp*cy-cr*sy;
			mat[1][1]=srsp*sy+cr*cy;
			mat[1][2]=sr*cp;

			mat[2][0]=crsp*cy+sr*sy;
			mat[2][1]=crsp*sy-sr*cy;
			mat[2][2]=cr*cp;

			return mat;
		}

		IVector3D operator* (IMatrix& mat, Core::IVector3D& vec)
		{
			if (mat.GetWidth()==3 && mat.GetHeight()==3)
			{
				IMatrix matVec(1,3);
				matVec[0][0]=vec[0];
				matVec[0][1]=vec[1];
				matVec[0][2]=vec[2];
				matVec=mat*matVec;
				return IVector3D(matVec[0][0],matVec[0][1],matVec[0][2]);
			}else if (mat.GetWidth()==4 && mat.GetHeight()==4)
			{
				IMatrix matVec(1,4);
				matVec[0][0]=vec[0];
				matVec[0][1]=vec[1];
				matVec[0][2]=vec[2];
				matVec[0][3]=1.0;
				matVec=mat*matVec;
				return IVector3D(matVec[0][0]/matVec[0][3],matVec[0][1]/matVec[0][3],matVec[0][2]/matVec[0][3]);
			}else
				return vec;
		}

		Core::IVector3D operator* (Core::IVector3D& vec, IMatrix& mat)
		{
			if (mat.GetWidth()==3 && mat.GetHeight()==3)
			{
				IMatrix matVec(3,1);
				matVec[0][0]=vec[0];
				matVec[1][0]=vec[1];
				matVec[2][0]=vec[2];
				matVec=matVec*mat;
				return IVector3D(matVec[0][0],matVec[1][0],matVec[2][0]);
			}else if (mat.GetWidth()==4 && mat.GetHeight()==4)
			{
				IMatrix matVec(4,1);
				matVec[0][0]=vec[0];
				matVec[1][0]=vec[1];
				matVec[2][0]=vec[2];
				matVec[3][0]=1.0;
				matVec=matVec*mat;
				return IVector3D(matVec[0][0]/matVec[3][03],matVec[1][0]/matVec[3][0],matVec[2][0]/matVec[3][0]);
			}else
				return vec;
		}

		IVector3D PolygonCenter(ITriangle poly)
		{
			float minX,minY,minZ;
			float maxX,maxY,maxZ;
			minX=poly.GetVertex()[0].GetX();
			minY=poly.GetVertex()[0].GetY();
			minZ=poly.GetVertex()[0].GetZ();

			maxX=poly.GetVertex()[0].GetX();
			maxY=poly.GetVertex()[0].GetY();
			maxZ=poly.GetVertex()[0].GetZ();

			IVector3D vert[3] ;
			vert[0] = poly.GetVertex()[0];
			vert[1] = poly.GetVertex()[1];
			vert[2] = poly.GetVertex()[2];
			for (int i=1;i<3;i++)
			{
				if (vert[i].GetX()<minX)
					minX=vert[i].GetX();
				if (vert[i].GetY()<minY)
					minY=vert[i].GetY();
				if (vert[i].GetZ()<minZ)
					minZ=vert[i].GetZ();

				if (vert[i].GetX()>maxX)
					maxX=vert[i].GetX();
				if (vert[i].GetY()>maxY)
					maxY=vert[i].GetY();
				if (vert[i].GetZ()>maxZ)
					maxZ=vert[i].GetZ();
			}
			return IVector3D(maxX-minX,maxY-minY,maxZ-minZ);

			/*float x,y,z;
			x=(poly.GetVertex()[0].GetX()+
				poly.GetVertex()[1].GetX()+
				poly.GetVertex()[2].GetX())/3.0;

			y=(poly.GetVertex()[0].GetY()+
				poly.GetVertex()[1].GetY()+
				poly.GetVertex()[2].GetZ())/3.0;

			z=(poly.GetVertex()[0].GetZ()+
				poly.GetVertex()[0].GetZ()+
				poly.GetVertex()[0].GetZ())/3.0;*/

			//return IVector3D(x,y,z);
		}

		IVector3D NormalFromPolygon(ITriangle poly)
		{
			/*N = {0,0,0};
			for (each vertex in the polygon Vn)
			{
				Nx += (Vny - V(n+1)y) * (Vnz + V(n+1)z);
				Ny += (Vnz - V(n+1)z) * (Vnx + V(n+1)x);
				Nz += (Vnx - V(n+1)x) * (Vny + V(n+1)y);
			}*/

			IVector3D vert[3] ;
			vert[0] = poly.GetVertex()[0];
			vert[1] = poly.GetVertex()[1];
			vert[2] = poly.GetVertex()[2];

			IVector3D normal(0.0,0.0,0.0);

			normal[0] = (vert[0][1] - vert[1][1]) * (vert[0][2] - vert[1][2])
						+(vert[1][1] - vert[2][1]) * (vert[1][2] - vert[2][2])
						+(vert[2][1] - vert[0][1]) * (vert[2][2] - vert[0][2]);

			normal[1] = (vert[0][2] - vert[1][2]) * (vert[0][0] - vert[1][0])
						+(vert[1][2] - vert[2][2]) * (vert[1][0] - vert[2][0])
						+(vert[2][2] - vert[0][2]) * (vert[2][0] - vert[0][0]);

			normal[2] = (vert[0][0] - vert[1][0]) * (vert[0][1] - vert[1][1])
						+(vert[1][0] - vert[2][0]) * (vert[1][1] - vert[2][1])
						+(vert[2][0] - vert[0][0]) * (vert[2][1] - vert[0][1]);

			return normal;
		}

		IVector3D RenderCubicBezier( IVector3D pt1, IVector3D pt2, 
									IVector3D inter1, IVector3D inter2, 
									double pos)
		{
			IVector3D ptt;
			IVector3D Q0,Q1,Q2,Q3,Q4;
			
			Q0=pt1+(inter1-pt1)*pos;
			Q1=inter1+(inter2-inter1)*pos;
			Q2=inter2+(pt2-inter2)*pos;
			Q3=Q0+(Q1-Q0)*pos;
			Q4=Q1+(Q2-Q1)*pos;
			ptt=Q3+(Q4-Q3)*pos;
			return IVector3D(ptt);
		}

		IVector3D RenderConicBezier( IVector3D pt1, IVector3D pt2,
									IVector3D inter,
									double pos)
		{
			IVector3D ptt;
			IVector3D Q0,Q1;

			Q0=pt1+(inter-pt1)*pos;
			Q1=inter+(pt2-inter)*pos;
			ptt=Q0+(Q1-Q0)*pos;
			return IVector3D(ptt);
		}

		bool BBoxVisibleInCameraView(IBBox& box, ICamera& camera, IMatrix boxpos)
		{
			IVector3D Corner[8];
			IPolygon Planes[6];

			Corner[0]=box.GetCenter()+box.GetDimension()*IVector3D(1,1,1);
			Corner[1]=box.GetCenter()+box.GetDimension()*IVector3D(1,1,-1);
			Corner[2]=box.GetCenter()+box.GetDimension()*IVector3D(1,-1,1);
			Corner[3]=box.GetCenter()+box.GetDimension()*IVector3D(1,-1,-1);
			Corner[4]=box.GetCenter()+box.GetDimension()*IVector3D(-1,1,1);
			Corner[5]=box.GetCenter()+box.GetDimension()*IVector3D(-1,1,-1);
			Corner[6]=box.GetCenter()+box.GetDimension()*IVector3D(-1,-1,1);
			Corner[7]=box.GetCenter()+box.GetDimension()*IVector3D(-1,-1,-1);
			
			//Planes[0]=

			return true;
		}

		IVector3D Min(IVector3D a, IVector3D b)
		{
			return IVector3D(	a.x<b.x?a.x:b.x,
								a.y<b.y?a.y:b.y,
								a.z<b.z?a.z:b.z);
		}

		IVector3D Max(IVector3D a, IVector3D b)
		{
			return IVector3D(	a.x>b.x?a.x:b.x,
								a.y>b.y?a.y:b.y,
								a.z>b.z?a.z:b.z);
		}

		IBBox BBoxToScreenBBox(IBBox src,IMatrix projection)
		{
			IVector3D minPos(65535,65535,65535);
			IVector3D maxPos=-minPos;
			IVector3D pos;

			for (int x=-1;x<=1;x+=2)
			{
				for (int y=-1;y<=1;y+=2)
				{
					for (int z=-1;z<=1;z+=2)
					{
						pos = (projection*(src.GetCenter()+src.GetDimension()*0.5*IVector3D(x,y,z)));
						minPos = Min(minPos,pos);
						maxPos = Max(maxPos,pos);
					}
				}
			}
			minPos=minPos*0.5+0.5;
			maxPos=maxPos*0.5+0.5;
			return IBBox(minPos.x,minPos.y,minPos.z,maxPos.x,maxPos.y,maxPos.z);
		}

		IMatrix GetOrthoMatrix(float right, float left, float top, float bottom, float m_fzFar, float m_fzNear)
		{
			double tx = - (right + left)/(right - left);
			double ty = - (top + bottom)/(top - bottom);
			double tz = - (m_fzFar + m_fzNear)/(m_fzFar - m_fzNear);

			IMatrix OrthoMat(4);
			OrthoMat.MakeZero();
			OrthoMat[0][0]=2.0/(right - left);
			OrthoMat[1][1]=2.0/(top - bottom);
			OrthoMat[2][2]=2.0/(m_fzFar - m_fzNear);
			OrthoMat[3][0]=tx;
			OrthoMat[3][1]=ty;
			OrthoMat[3][2]=tz;
			OrthoMat[3][3]=1.0;

			return OrthoMat;
		}

		IMatrix GetPerspectiveMatrix(float fov, float ratio, float Far, float Near)
		{
			double f = 1.0/tan(DEG_TO_RAD(fov)/2.0);
			double a = (Far+Near)/(Near-Far);
			double b = (2*Far*Near)/(Near-Far);

			IMatrix PerspectiveMat(4);
			PerspectiveMat.MakeZero();
			PerspectiveMat[0][0]=f/ratio;
			PerspectiveMat[1][1]=f;
			PerspectiveMat[2][2]=a;
			PerspectiveMat[3][2]=b;
			PerspectiveMat[2][3]=-1;

			//PerspectiveMat[2][2]/=Far;
			//PerspectiveMat[3][2]/=Far;

			/*float Q = Far/(Far-Near);
			PerspectiveMat[0][0]=f/ratio;
			PerspectiveMat[1][1]=f;
			PerspectiveMat[2][2]=Q;
			PerspectiveMat[2][3]=1;
			PerspectiveMat[3][2]=-Q*Near;

			PerspectiveMat[2][2]/=Far;
			PerspectiveMat[3][2]/=Far;*/

			/*w  	0  	0  	0
			0 	h 	0 	0
			0 	0 	Q 	1
			0 	0 	-QN 	0

			Where:

			w = X scaling factor
			h = Y scaling factor
			N = near Z
			F = far Z
			Q = F / (F-N)*/


			return PerspectiveMat;
		}
	}
}