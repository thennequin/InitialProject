//---------------------------------------------
//                Initial Engine
//              Math source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/3D/IBBox.h"
#include "Initial/Math/IMathIntersect.h"
#include <math.h>

using namespace Initial::Core;

namespace Initial
{
	namespace Math
	{
		namespace Intersect
		{
			void RayIntersectRay(IVector3D start1, IVector3D dir1, IVector3D start2, IVector3D dir2, IVector3D& res1, IVector3D& res2)
			{
				dir1.Normalize();
				dir2.Normalize();
				float t1, t2;
				IVector3D cross = Cross(dir1,dir2);
				t1 = Determinant((start2-start1),dir2,cross) / (cross.Length()*cross.Length());
				t2 = Determinant((start2-start1),dir1,cross) / (cross.Length()*cross.Length());
				IVector3D L1 = start1 + dir1*t1;
				IVector3D L2 = start2 + dir2*t2;
				res1=L1;
				res2=L2;
			}

			bool CheckSameClockDir(IVector3D pt1, IVector3D pt2, IVector3D pt3, IVector3D norm)
			{  
			   float testi, testj, testk;
			   float dotprod;
			   // normal of trinagle
			   testi = (((pt2.GetY() - pt1.GetY())*(pt3.GetZ() - pt1.GetZ())) - ((pt3.GetY() - pt1.GetY())*(pt2.GetZ() - pt1.GetZ())));
			   testj = (((pt2.GetZ() - pt1.GetZ())*(pt3.GetX() - pt1.GetX())) - ((pt3.GetZ() - pt1.GetZ())*(pt2.GetX() - pt1.GetX())));
			   testk = (((pt2.GetX() - pt1.GetX())*(pt3.GetY() - pt1.GetY())) - ((pt3.GetX() - pt1.GetX())*(pt2.GetY() - pt1.GetY())));

			   // Dot product with triangle normal
			   dotprod = norm.GetX()*testi + norm.GetY()*testj + norm.GetZ()*testk;

			   //answer
			   if(dotprod < 0) 
				   return false;
			   else 
				   return true;
			}

			bool RayIntersectTriangle(IVector3D tri[3], IVector3D linestart, IVector3D lineend, IVector3D& result, bool allray)
			{
				IVector3D pt1=tri[0];
				IVector3D pt2=tri[1];
				IVector3D pt3=tri[2];
				IVector3D linept=linestart;
				IVector3D vect=lineend-linestart;
				
				vect.Normalize();

				IVector3D V1, V2, norm;
				double dotprod;
				double t;

				// vector form triangle pt1 to pt2
				V1 = pt2 - pt1;
				V2 = pt3 - pt1;
				
				// vector normal of triangle
				norm=Cross(V1,V2);
				norm.Normalize();

				// dot product of normal and line's vector if zero line is parallel to triangle
				dotprod = Dot(norm,vect);

				if(dotprod != 0)
				{
					//Find point of intersect to triangle plane.
					//find t to intersect point
					t = -(norm.GetX()*(linept.GetX()-pt1.GetX())+norm.GetY()*(linept.GetY()-pt1.GetY())+norm.GetZ()*(linept.GetZ()-pt1.GetZ()))/
					(norm.GetX()*vect.GetX()+norm.GetY()*vect.GetY()+norm.GetZ()*vect.GetZ());

					// if ds is neg line started past triangle so can't hit triangle.
					if(t < 0 && allray==false)
						return false;

					IVector3D PT_INTEGER;
					PT_INTEGER.Set(	linept.GetX() + vect.GetX()*t,
								linept.GetY() + vect.GetY()*t,
								linept.GetZ() + vect.GetZ()*t);

					if(CheckSameClockDir(pt1, pt2, PT_INTEGER, norm) == true)
					{
						if(CheckSameClockDir(pt2, pt3, PT_INTEGER, norm) == true)
						{
							if(CheckSameClockDir(pt3, pt1, PT_INTEGER, norm) == true)
							{
								result=PT_INTEGER;
								return true;
							}
						}
					}
				}
				return false;
			}

			bool RayIntersectTriangle(ITriangle& tri, IVector3D linestart, IVector3D lineend, IVector3D& result, bool allray)
			{
				IVector3D pt[3];
				pt[0]=tri.GetVertex()[0];
				pt[1]=tri.GetVertex()[1];
				pt[2]=tri.GetVertex()[2];
				return RayIntersectTriangle(pt,linestart,lineend,result,allray);
			}

			bool RayIntersectPlane(IVector3D startray, IVector3D raydir, IVector3D planeorig, IVector3D planedir1, IVector3D planedir2, IVector3D& res)
			{
				IVector3D pt1=planeorig;
				IVector3D pt2=planeorig+planedir1;
				IVector3D pt3=planeorig+planedir2;
				IVector3D linept=startray;
				IVector3D vect=raydir;
				
				vect.Normalize();

				IVector3D V1, V2, norm;
				double dotprod;
				double t;

				// vector form triangle pt1 to pt2
				V1 = pt2 - pt1;
				V2 = pt3 - pt1;
				
				// vector normal of triangle
				norm=Cross(V1,V2);
				norm.Normalize();

				// dot product of normal and line's vector if zero line is parallel to triangle
				dotprod = Dot(norm,vect);

				if(dotprod != 0)
				{
					//Find point of intersect to triangle plane.
					//find t to intersect point
					t = -(norm.GetX()*(linept.GetX()-pt1.GetX())+norm.GetY()*(linept.GetY()-pt1.GetY())+norm.GetZ()*(linept.GetZ()-pt1.GetZ()))/
					(norm.GetX()*vect.GetX()+norm.GetY()*vect.GetY()+norm.GetZ()*vect.GetZ());

					IVector3D PT_INTEGER;
					PT_INTEGER.Set(	linept.GetX() + vect.GetX()*t,
								linept.GetY() + vect.GetY()*t,
								linept.GetZ() + vect.GetZ()*t);

					res=PT_INTEGER;

					return true;
				}
				return false;
			}

			bool RayIntersectBox(IBBox box, IVector3D linestart, IVector3D lineend, IVector3D& result, bool allray)
			{
				IVector3D res;
				ITriangle tri;

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				//

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,-0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,-0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				//

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				//

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				//

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				//

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				tri.SetVertex(0,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,0.5));
				tri.SetVertex(1,box.GetCenter()+box.GetDimension()*IVector3D(0.5,-0.5,-0.5));
				tri.SetVertex(2,box.GetCenter()+box.GetDimension()*IVector3D(-0.5,-0.5,-0.5));
				if (RayIntersectTriangle(tri,linestart,lineend,res,allray))
				{
					result=res;
					return true;
				}

				return false;
			}

			bool PlaneIntersect(IPlane a, IPlane b, IPlane c, IVector3D& res)
			{
				IVector3D n0=a.GetNormal();
				IVector3D n1=b.GetNormal();
				IVector3D n2=c.GetNormal();

				IVector3D n1_n2, n2_n0, n0_n1;  
			    
				n1_n2 = Cross(n1, n2);
				n2_n0 = Cross(n2, n0);
				n0_n1 = Cross(n0, n1);

				float cosTheta = Dot(n0, n1_n2);
			    
				//#define FLT_AS_DW(F) (*(DWORD*)&(F))
				//#define ALMOST_ZERO(F) ((FLT_AS_DW(F) & 0x7f800000L)==0)
				//#define IS_SPECIAL(F)  ((FLT_AS_DW(F) & 0x7f800000L)==0x7f800000L)
				//if ( ALMOST_ZERO(cosTheta) || IS_SPECIAL(cosTheta) )
				//	return false;

				float secTheta = 1.f / cosTheta;

				n1_n2 = n1_n2*a.GetD();
				n2_n0 = n2_n0*b.GetD();
				n0_n1 = n0_n1*c.GetD();

				res = -(n1_n2 + n2_n0 + n0_n1) * secTheta;
				return true;
			}

			bool PointOnRay(Core::IVector3D point, Core::IVector3D start, Core::IVector3D end)
			{
				IVector3D dir1, dir2, sub;
				dir1=point-start;
				dir2=point-end;
				sub=end-start;

				if (dir2.Length()+dir1.Length()>sub.Length()+0.001)
					return false;

				return true;
			}
		}
	}
}