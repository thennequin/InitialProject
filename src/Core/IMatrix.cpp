//---------------------------------------------
//                Initial Engine
//             Matrix source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "Initial.h"

using namespace Initial;

#define EPS 0.001

void IMatrix::_Alloc(int width,int height)
{
	Width=width;
	Height=height;
	if (Width<1)
		Width=1;

	if (Height<1)
		Height=1;

	//data = (double*)malloc(sizeof(double)*Width*Height);
	data = new double[Width*Height];
}

IMatrix::IMatrix()
{
	_Alloc(1,1);
}

IMatrix::IMatrix(IMatrix& mat)
{
	_Alloc(1,1);
	CopyFrom(mat);
}

IMatrix::IMatrix(int width, int height)
{
	_Alloc(width,height);	
}

IMatrix::IMatrix(int width,int height,double *values)
{
	_Alloc(width,height);
	SetValue(values);
}

IMatrix::IMatrix(int width,int height,float *values)
{
	_Alloc(width,height);
	SetValue(values);
}

IMatrix::IMatrix(int n)
{
	_Alloc(n,n);
}

IMatrix::~IMatrix()
{
	if (data) delete data;
}

void IMatrix::SetSize(int width, int height)
{
	if (data) delete data;
	_Alloc(width,height);
}

double IMatrix::GetValue(int m, int n)
{
	if ( (m>=0 || m<Width) 
		&& (n>=0 || n<Height)) 
		return data[m*Height+n];
	else return 0;
}

const double* IMatrix::GetValues()
{
	//IMatrix trans=Transpose();
	return data;
}

bool IMatrix::SetValue(int m, int n, double val)
{
	if ( (m>=0 || m<Width) 
		&& (n>=0 || n<Height)) 
	{
		data[m*Height+n]=val;
		return true;
	}else return false;
}

bool IMatrix::SetValue(const double *values)
{
	if (values)
	{
		int j=0;
		for (int j=0;j<Height;j++)
			for (int i=0;i<Width;i++)		
				data[i*Height+j]=values[i+j*Width];

		return true;
	}
	return false;
}

bool IMatrix::SetValue(const float *values)
{
	if (values)
	{
		int j=0;
		for (int j=0;j<Height;j++)
			for (int i=0;i<Width;i++)		
				data[i*Height+j]=values[i+j*Width];

		return true;
	}
	return false;
}

bool IMatrix::SetValue(int num,...)
{
	va_list ap;
	va_start(ap, num);

	int val=0;
	for (int j=0;j<Height;j++)
	{
		if (val>num)
			break;
		for (int i=0;i<Width;i++)		
		{
			data[i*Height+j]=(double)va_arg(ap,double);
			val++;
			if (val>num)
				break;
		}
	}

	va_end(ap);
	return true;
}

/*bool IMatrix::SetValue(int num,	double a0,
								double a1,
								double a2,
								double a3,
								double a4,
								double a5,
								double a6,
								double a7,
								double a8,
								double a9,
								double a10,
								double a11,
								double a12,
								double a13,
								double a14,
								double a15,
								double a16,
								double a17,
								double a18,
								double a19,
								double a20,
								double a21,
								double a22,
								double a23,
								double a24,
								double a25,
								double a26,
								double a27,
								double a28,
								double a29,
								double a30,
								double a31 )
{
	return true;
}*/

IMatrix IMatrix::GetLine(int line)
{
	if (line>=0 && line <Height)
	{
		IMatrix temp(Width,1);
		for (int i=0;i<Width;i++)
			temp[i][0]=(*this)[i][line];
		return IMatrix(temp);
	}
	return IMatrix();
}

IMatrix IMatrix::GetColumn(int column)
{
	if (column>=0 && column<Width)
	{
		return IMatrix(1,Height,data+(column*Height));
	}
	return IMatrix();
}

bool IMatrix::MakeIdentity(int n)
{
	if (n<=0)
	{
		if (Width!=Height)
			return false;
	}else{
		free(data);
		_Alloc(n,n);
	}

	for (int i=0;i<Width;i++)
	{
		for (int j=0;j<Height;j++)	
		{
			SetValue(i,j, i==j ? 1 : 0);
		}
	}

	return true;
}

void IMatrix::MakeZero(int n)
{
	if (n>0)
	{
		free(data);
		_Alloc(n,n);
	}

	for (int i=0;i<Width;i++)
	{
		for (int j=0;j<Height;j++)	
		{
			SetValue(i,j,0);
		}
	}
}

bool IMatrix::Add(IMatrix b,IMatrix &res)
{
	IMatrix temp;
	if (Width==b.Width 
		&& Height==b.Height)
	{
		temp.SetSize(Width,Height);

		for (int i=0;i<Width;i++)
		{
			for (int j=0;j<Height;j++)	
			{
				//temp[i][j]=GetValue(i,j)+b.GetValue(i,j);
				temp.SetValue(i,j,GetValue(i,j)+b.GetValue(i,j));
			}
		}
		res=temp;
		return true;
	}
	return false;
}

bool IMatrix::Dot(IMatrix b,IMatrix &res)
{
	IMatrix temp;
	if (Width==b.Height)
	{
		int resW = b.Width;
		int resH = Height;

		temp.SetSize(resW,resH);

		double val;
		for (int i=0;i<resW;i++)
		{
			for (int j=0;j<resH;j++)	
			{
				val=0;
				for (int k=0;k<Width;k++)
				{
					val+=GetValue(k,j)*b.GetValue(i,k);
				}
				temp.SetValue(i,j,val);
			}
		}
		res=temp;
		return true;
	}
	return false;
}

bool IMatrix::Add(IMatrix a,IMatrix b,IMatrix &res)
{
	return a.Add(b,res);
}

bool IMatrix::Dot(IMatrix a,IMatrix b,IMatrix &res)
{
	return a.Dot(b,res);
}


void IMatrix::Print()
{
	for (int j=0;j<Height;j++)	
	{
		for (int i=0;i<Width;i++)
		{
			printf("%f\t",GetValue(i,j));
			//printf("%f\t",(*this)[i][j]);
		}
		printf("\n");
	}
}

double IMatrix::Det(bool Gauss)
{
	if (Width!=Height)
		return 0;

	if (Width==1)
		return (*this)[0][0];
	else if (Width==2)
		return (*this)[0][0]*(*this)[1][1]-(*this)[0][1]*(*this)[1][0];
	else if (Width==3)
	{
		return (GetValue(0,0)*GetValue(1,1)*GetValue(2,2)+
			    GetValue(0,1)*GetValue(1,2)*GetValue(2,0)+
			    GetValue(0,2)*GetValue(1,0)*GetValue(2,1))
			   -
			   (GetValue(2,0)*GetValue(1,1)*GetValue(0,2)+
			    GetValue(1,0)*GetValue(0,1)*GetValue(2,2)+
			    GetValue(0,0)*GetValue(2,1)*GetValue(1,2));
	}else{	
		if (Gauss)
		{
			IMatrix temp=GaussJordan();
			double det=1;
			for (int i=0;i<temp.Width;i++)
				det*=temp[i][i];
			return det;
		}else{
			double res=0;
			int pow=1;
			IMatrix sub(Width-1,Width-1);

			for (int i=0;i<Width;i++)
			{
				int posX=0;
				
				for (int x=1;x<Width;x++)
				{
					int posY=0;
					for (int y=0;y<Width;y++)
					{
						if (y!=i)
						{
							sub[posX][posY]	= (*this)[x][y];
							posY++;
						}
					}
					posX++;
				}
				double det = sub.Det();
				res=res+pow*det*(*this)[0][i];
				pow*=-1;			
			}
			return res;
		}
	}
}

bool IMatrix::Inv(IMatrix &res) // Inversion avec la reduction de Gauss Jordan
{
	if (Width!=Height)
		return false;

	IMatrix temp(Width*2,Height);
	for (int i=0;i<Width;i++)
		for (int j=0;j<Height;j++)
			temp[i][j]=GetValue(i,j);

	for (int i=0;i<Width;i++)
		for (int j=0;j<Height;j++)
			if (i==j)
				temp[i+Width][j]=1;
			else
				temp[i+Width][j]=0;

	IMatrix temp2;
	temp2 = temp.GaussJordan(true);	
	temp=temp2;
	temp2.SetSize(Width,Height);
	
	for (int i=0;i<Width;i++)
		for (int j=0;j<Height;j++)
			temp2[i][j]=temp[i+Width][j];

	res.SetSize(Width,Width);
	res=temp2;

	return true;
}

IMatrix IMatrix::Transpose()
{
	IMatrix res;
	res.SetSize(Height,Width);
	for (int i=0;i<Width;i++)
		for (int j=0;j<Height;j++)
			res[j][i]=(*this)[i][j];

	return IMatrix(res);
}

void IMatrix::CopyFrom(const IMatrix& mat)
{
	SetSize(mat.Width,mat.Height);
	memcpy(data,mat.data,Width*Height*sizeof(double));
}

/* Calcul Gauss Jordan */
IMatrix IMatrix::GaussJordan(bool normalise)
{
	bool Det=false;
	IMatrix temp1(*this);
	IMatrix temp2=temp1;

	int start = normalise ? 0 : 1;

	int lastblock=-1;
	for (int i=0;i<Height;i++)
	{
		start = normalise ? 0 : i+1;

		for (int j=start;j<Height;j++) 
		{
			if (normalise && j==i)
			{
				if (temp1[j][j]!=0)
				{
					for (int k=0;k<Width;k++)
						temp2[k][j]=temp1[k][j]/temp1[j][j];
					temp1=temp2;
				}
			}else{
				double fact=temp1[i][j]/temp1[i][i];
				// On traite la ligne
				for (int k=0;k<Width;k++)
					temp2[k][j]=temp1[k][j]-fact*temp1[k][i];
				
				if (temp2[j][j]==0 && Det) // on échange la ligne avec la ligne du dessous si elle existe
				{
					if (lastblock!=j) // On ne boucle pas sur le meme alors on passe au suivant
					{
						lastblock=j;
						if (j+1<Height)
						{
							for (int k=0;k<Width;k++)
							{
								temp2[k][j]=-temp1[k][j+1];
								temp2[k][j+1]=-temp1[k][j];
								temp1[k][j]=temp2[k][j];
								temp1[k][j+1]=temp2[k][j+1];
							}
							j--;
						}
					}
				}
			}
		}

		temp1=temp2;
	}
	return IMatrix(temp1);
}

/*IMatrix IMatrix::GaussJordan(bool normalise)
{
	IMatrix temp1(*this);
	IMatrix temp2=temp1;

	if (temp1[0][0]==0) // On échange la ligne
	{
		int line=0;
		for (int i=1;i<Height;i++)
		{
			if (temp1[0][i]!=0)
			{
				line = i;
				break;
			}
		}
		if (line==0)
			return IMatrix();

		for (int i=1;i<Width;i++)
		{
			temp2[i][0]=temp1[i][line];
			temp2[i][line]=temp1[i][0];
		}
	}

	temp1=temp2;

	for (int i=0;i<Height;i++)
	{
		for (int j=i+1;j<Height;j++) 
		{			
			double fact=temp1[i][i]/temp1[i][j];
			// On traite la ligne
			for (int k=0;k<Width;k++)
			{
				temp2[k][j]=temp1[k][j]-fact*temp1[k][i];
				if (temp2[k][j]<EPS)
					temp2[k][j]=0;
			}
		}
	}
	return IMatrix(temp2);
}*/

IMatrix::IMatrixColumn IMatrix::operator[](int n)
{
	if (n>=0 || n<Height) 
		return IMatrixColumn(data+(n*Height),Height);
	else return IMatrixColumn(0,0);
}

void IMatrix::operator= (IMatrix& val)
{
	CopyFrom(val);
}

void IMatrix::operator*= (IMatrix& val)
{
	Dot(val,*this);
}

IMatrix IMatrix::operator*(IMatrix& B)
{
	IMatrix temp(*this);
	temp.Dot(B,temp);
	return IMatrix(temp);
}

IMatrix IMatrix::operator+ (IMatrix& B)
{
	IMatrix temp(*this);
	temp.Add(B,temp);
	return IMatrix(temp);
}

double& IMatrix::IMatrixColumn::operator[](int n)
{
	double temp=0;
	if (n>=0 || n<Len) 
		return *(Data+(n));
	else return temp;
}