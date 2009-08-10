//---------------------------------------------
//                Initial Engine
//                IMatrix  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATRIX_HEADER_
#define _IMATRIX_HEADER_

#include "Initial/Core/IVector3D.h"

namespace Initial
{
	namespace Math
	{
		class IMatrix
		{
			class IMatrixColumn
			{
			public:
				IMatrixColumn(double *values,int len)
					: Data(values),Len(len)
				{		
				}

				double& operator[](int n);				
			private:
				double *Data;
				int Len;

				double _temp; //variable temporaire de retour
			};
		public:
			void _Alloc(int width,int height);

			IMatrix();
			IMatrix(IMatrix& mat);
			IMatrix(int width,int height);
			IMatrix(int width,int height,double *values);
			IMatrix(int width,int height,float *values);
			IMatrix(int n);
			~IMatrix();

			void CopyFrom(const IMatrix& mat);

			void SetSize(int width,int height);
			int GetWidth() { return Width; }
			int GetHeight() { return Height; }

			double GetValue(int m, int n);
			const double* GetValues();
			const float* GetFloatValues();
			bool SetValue(int m, int n, double val);

			bool SetValue(const double *values);
			bool SetValue(const float *values);
			bool SetValue(int num,...);
			// Set Matrix 6x6 max
			/*bool SetValue(int num,	double a0=0,
									double a1=0,
									double a2=0,
									double a3=0,
									double a4=0,
									double a5=0,
									double a6=0,
									double a7=0,
									double a8=0,
									double a9=0,
									double a10=0,
									double a11=0,
									double a12=0,
									double a13=0,
									double a14=0,
									double a15=0,
									double a16=0,
									double a17=0,
									double a18=0,
									double a19=0,
									double a20=0,
									double a21=0,
									double a22=0,
									double a23=0,
									double a24=0,
									double a25=0,
									double a26=0,
									double a27=0,
									double a28=0,
									double a29=0,
									double a30=0,
									double a31=0 );*/

			IMatrix GetLine(int line);
			IMatrix GetColumn(int column);

			// Base Matrix;
			bool MakeIdentity(int n=0);
			void MakeZero(int n=0);

			//Basic operations
			bool Add(IMatrix b,IMatrix &res);
			bool Dot(IMatrix b,IMatrix &res);

			static bool Add(IMatrix a,IMatrix b,IMatrix &res);
			static bool Dot(IMatrix a,IMatrix b,IMatrix &res);

			double Det(bool UseGauss=true);
			bool Inv(IMatrix &res);
			IMatrix Transpose();

			IMatrix GaussJordan(bool normalise=false);

			IMatrixColumn operator[](int n);
			double& operator()(int col, int line);

			void operator= (IMatrix& val);

			void operator*= (IMatrix& val);

			IMatrix operator*(IMatrix& B);

			IMatrix operator+ (IMatrix& B);

			Core::IVector3D operator*(const Core::IVector3D& vect);

			void Print();

		protected:
			int Width,Height;
			double *data;
			float *floatdata;

			double _temp; //variable temporaire de retour
		};
	}
}

#endif
