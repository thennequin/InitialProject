//---------------------------------------------
//                Initial Engine
//              Math source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/IBBox.h"
#include "3D/Camera/ICamera.h"
#include "3D/IPolygon.h"
#include "Core/IMath.h"
#include <math.h>

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

		IMatrix AngleXToMatrix(float angle)
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

		IMatrix AngleYToMatrix(float angle)
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

		IMatrix AngleZToMatrix(float angle)
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

		IVertex PolygonCenter(ITriangle poly)
		{
			float minX,minY,minZ;
			float maxX,maxY,maxZ;
			minX=poly.GetVertex()[0].GetX();
			minY=poly.GetVertex()[0].GetY();
			minZ=poly.GetVertex()[0].GetZ();

			maxX=poly.GetVertex()[0].GetX();
			maxY=poly.GetVertex()[0].GetY();
			maxZ=poly.GetVertex()[0].GetZ();

			IVertex vert[3] ;
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
			return IVertex(maxX-minX,maxY-minY,maxZ-minZ);

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

			//return IVertex(x,y,z);
		}

		INormal NormalFromPolygon(ITriangle poly)
		{
			/*N = {0,0,0};
			for (each vertex in the polygon Vn)
			{
				Nx += (Vny - V(n+1)y) * (Vnz + V(n+1)z);
				Ny += (Vnz - V(n+1)z) * (Vnx + V(n+1)x);
				Nz += (Vnx - V(n+1)x) * (Vny + V(n+1)y);
			}*/

			IVertex vert[3] ;
			vert[0] = poly.GetVertex()[0];
			vert[1] = poly.GetVertex()[1];
			vert[2] = poly.GetVertex()[2];

			INormal normal(0.0,0.0,0.0);

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

		void CalcultatePolygonNormals(ITriangle& poly)
		{
			// on calcule 2 vecteurs formant les bords du triangle
			// le vecteur side0 est celui allant du vertex 0 au vertex 1
			IVertex side0 = poly.GetVertex()[1] - poly.GetVertex()[0];
			// le vecteur side1 est celui allant du vertex 0 au vertex 2
			IVertex side1 = poly.GetVertex()[2] - poly.GetVertex()[0];

			// ici on calcule la normale à la face
			IVertex normal = side0*side1;
			normal.Normalize();

			// maintenant on calcule les coefficients des tangentes
			// ces coefficients sont calcules en fonction des 
			// coordonnées de textures du polygone
			float deltaT0 = poly.GetCoords()[1].GetV()-
							poly.GetCoords()[0].GetU();

			float deltaT1 = poly.GetCoords()[2].GetV()-
							poly.GetCoords()[0].GetU();
			
			// on effectue la meme chose pour la binormale
			float deltaB0 = poly.GetCoords()[1].GetU()-
							poly.GetCoords()[0].GetU();
			float deltaB1 = poly.GetCoords()[2].GetU()-
							poly.GetCoords()[0].GetU();
			// le facteur permettant de rendre les tangentes et binormales
			// proportionnelles aux coordonnees de textures.
			// nous aurons donc des vecteurs dont la norme depend des coordonnees 
			// de textures.
			float scale = 1.0/ ((deltaB0 * deltaT1) - (deltaB1 * deltaT0));

			// on calcule la tangente temporaire
			//IVertex tmpTangente = (side0*deltaT1)*scale;
			IVertex tmpTangente = ((side0*deltaT1)  - (side1*deltaT0))*scale;
			tmpTangente.Normalize();

			// on calcule la binormale temporaire
			IVertex tmpBinormal = ((side0*(-deltaB1)) + (side1*deltaB0))*scale;
			tmpBinormal.Normalize();

			// nous avons donc maintenant les tangentes, binormales et normales de la face.
			// on les additionne a celles deja presentes dans les tableaux
			poly.GetNormals()[0]=IVertex(normal.GetX(),normal.GetY(),normal.GetZ());
			poly.GetNormals()[1]=IVertex(tmpTangente.GetX(),tmpTangente.GetY(),tmpTangente.GetZ());
			poly.GetNormals()[2]=IVertex(tmpBinormal.GetX(),tmpBinormal.GetY(),tmpBinormal.GetZ());

			/*tangents[face.vertexIndex[0]] += tmpTangente;
			tangents[face.vertexIndex[1]] += tmpTangente;
			tangents[face.vertexIndex[2]] += tmpTangente;

			binormals[face.vertexIndex[0]] += tmpBinormal;
			binormals[face.vertexIndex[1]] += tmpBinormal;
			binormals[face.vertexIndex[2]] += tmpBinormal;

			normals[face.vertexIndex[0]] += normal;
			normals[face.vertexIndex[1]] += normal;
			normals[face.vertexIndex[2]] += normal;*/

			// il ne faudra pas oublier de renormaliser les vecteurs par la suite.
		}


		IVertex RenderCubicBezier( IVertex pt1, IVertex pt2, 
									IVertex inter1, IVertex inter2, 
									double pos)
		{
			IVertex ptt;
			IVertex Q0,Q1,Q2,Q3,Q4;
			
			Q0=pt1+(inter1-pt1)*pos;
			Q1=inter1+(inter2-inter1)*pos;
			Q2=inter2+(pt2-inter2)*pos;
			Q3=Q0+(Q1-Q0)*pos;
			Q4=Q1+(Q2-Q1)*pos;
			ptt=Q3+(Q4-Q3)*pos;
			return IVertex(ptt);
		}

		IVertex RenderConicBezier( IVertex pt1, IVertex pt2,
									IVertex inter,
									double pos)
		{
			IVertex ptt;
			IVertex Q0,Q1;

			Q0=pt1+(inter-pt1)*pos;
			Q1=inter+(pt2-inter)*pos;
			ptt=Q0+(Q1-Q0)*pos;
			return IVertex(ptt);
		}

		bool BBoxVisibleInCameraView(IBBox& box, ICamera& camera, IMatrix boxpos)
		{
			IVertex Corner[8];
			IPolygon Planes[6];

			Corner[0]=box.GetCenter()+box.GetDimension()*IVertex(1,1,1);
			Corner[1]=box.GetCenter()+box.GetDimension()*IVertex(1,1,-1);
			Corner[2]=box.GetCenter()+box.GetDimension()*IVertex(1,-1,1);
			Corner[3]=box.GetCenter()+box.GetDimension()*IVertex(1,-1,-1);
			Corner[4]=box.GetCenter()+box.GetDimension()*IVertex(-1,1,1);
			Corner[5]=box.GetCenter()+box.GetDimension()*IVertex(-1,1,-1);
			Corner[6]=box.GetCenter()+box.GetDimension()*IVertex(-1,-1,1);
			Corner[7]=box.GetCenter()+box.GetDimension()*IVertex(-1,-1,-1);
			
			//Planes[0]=

			return true;
		}
	}
}