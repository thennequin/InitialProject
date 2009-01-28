//---------------------------------------------
//                Initial Engine
//        Shader expression source Module
//
//            By Thibault HENNEQUIN
//              January 20th  2009
//---------------------------------------------

#include "Core/IShaderExpression.h"

using namespace Initial;
using namespace Initial::Math;
using namespace Initial::ShaderExpression;

DEFINE_NEGATIVE(ISEFloat)
DEFINE_NEGATIVE(ISEVec2)
DEFINE_NEGATIVE(ISEVec3)
DEFINE_NEGATIVE(ISEVec4)
DEFINE_NEGATIVE(ISEMat2x2)
DEFINE_NEGATIVE(ISEMat3x3)
DEFINE_NEGATIVE(ISEMat4x4)

DEFINE_TRIGO_OPERATION(ISEFloat)
DEFINE_TRIGO_OPERATION(ISEVec2)
DEFINE_TRIGO_OPERATION(ISEVec3)
DEFINE_TRIGO_OPERATION(ISEVec4)

DEFINE_OPERATION(ISEFloat,ISEFloat,ISEFloat)
DEFINE_OPERATION(ISEFloat,ISEVec2,ISEVec2)
DEFINE_OPERATION(ISEFloat,ISEVec3,ISEVec3)
DEFINE_OPERATION(ISEFloat,ISEVec4,ISEVec4)
DEFINE_OPERATION(ISEFloat,ISEMat2x2,ISEMat2x2)
DEFINE_OPERATION(ISEFloat,ISEMat3x3,ISEMat3x3)
DEFINE_OPERATION(ISEFloat,ISEMat4x4,ISEMat4x4)

DEFINE_OPERATION(ISEVec2,ISEVec2,ISEVec2)
DEFINE_OPERATION(ISEVec2,ISEFloat,ISEVec2)

DEFINE_OPERATION(ISEVec3,ISEVec3,ISEVec3)
DEFINE_OPERATION(ISEVec3,ISEFloat,ISEVec3)

DEFINE_OPERATION(ISEVec4,ISEVec4,ISEVec4)
DEFINE_OPERATION(ISEVec4,ISEFloat,ISEVec4)

DEFINE_OPERATION(ISEMat2x2,ISEMat2x2,ISEMat2x2)

DEFINE_OPERATION(ISEMat3x3,ISEMat3x3,ISEMat3x3)

DEFINE_OPERATION(ISEMat4x4,ISEMat4x4,ISEMat4x4)

DEFINE_DISASSEMBLER(ISEVec2,x,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec2,y,ISEFloat)

DEFINE_DISASSEMBLER(ISEVec3,x,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec3,y,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec3,z,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec3,xy,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec3,xz,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec3,xw,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec3,yz,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec3,yw,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec3,zw,ISEVec2)

DEFINE_DISASSEMBLER(ISEVec4,x,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec4,y,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec4,z,ISEFloat)
DEFINE_DISASSEMBLER(ISEVec4,xy,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec4,xz,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec4,xw,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec4,yz,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec4,yw,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec4,zw,ISEVec2)
DEFINE_DISASSEMBLER(ISEVec4,xyz,ISEVec3)
DEFINE_DISASSEMBLER(ISEVec4,xyw,ISEVec3)
DEFINE_DISASSEMBLER(ISEVec4,xzw,ISEVec3)
DEFINE_DISASSEMBLER(ISEVec4,yzw,ISEVec3)

DEFINE_DISASSEMBLER_ARRAY(ISEVec2,ISEFloat,1)
DEFINE_DISASSEMBLER_ARRAY(ISEVec3,ISEFloat,2)
DEFINE_DISASSEMBLER_ARRAY(ISEVec4,ISEFloat,3)
DEFINE_DISASSEMBLER_ARRAY(ISEMat2x2,ISEVec2,1)
DEFINE_DISASSEMBLER_ARRAY(ISEMat3x3,ISEVec3,2)
DEFINE_DISASSEMBLER_ARRAY(ISEMat4x4,ISEVec4,3)

DEFINE_IF(ISEFloat)
DEFINE_IF(ISEVec2)
DEFINE_IF(ISEVec3)
DEFINE_IF(ISEVec4)
//DEFINE_IF(ISEMat2x2)
//DEFINE_IF(ISEMat3x3)
//DEFINE_IF(ISEMat4x4)

DEFINE_COMMON_FUNC(ISEFloat)
DEFINE_COMMON_FUNC(ISEVec2)
DEFINE_COMMON_FUNC(ISEVec3)
DEFINE_COMMON_FUNC(ISEVec4)

//-----------------------------------------------------------------------------------------
// IShaderExpression
//-----------------------------------------------------------------------------------------

IShaderExpression::IShaderExpression()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_FLOAT;
	m_fValue[0]=0.0;
}

IShaderExpression::IShaderExpression(const IShaderExpression& expr)
{
	_Copy(expr);
}

void IShaderExpression::_Copy(const IShaderExpression& expr)
{
	m_iType=expr.m_iType;
	m_iVarType=expr.m_iVarType;
	m_sParameter=expr.m_sParameter;
	if (m_iType==ET_CONSTANT)
	{
		if (m_iVarType==VT_MAT3x3 || m_iVarType==VT_MAT4x4)
		{
			m_mMatrix.CopyFrom(expr.m_mMatrix);
		}else{
			m_fValue[0]=expr.m_fValue[0];
			m_fValue[1]=expr.m_fValue[1];
			m_fValue[2]=expr.m_fValue[2];
			m_fValue[3]=expr.m_fValue[3];
		}
	}else{
		IShaderExpression *temp = (IShaderExpression*)&(expr);
		m_eExpr.Clear();
		for (int i=0;i<temp->m_eExpr.Count();i++)
		{
			m_eExpr.Add(temp->m_eExpr.GetItem(i));
		}
	}
}

void IShaderExpression::operator =(const IShaderExpression& expr)
{
	_Copy(expr);
}

IShaderExpression::VarType IShaderExpression::GetVarType()
{
	return m_iVarType;
}

IString IShaderExpression::TrimZeroFloat(IString str)
{
	IString temp;
	
	int pos = str.Length()-1;

	while (str[pos-1]=='0')
		pos--;
	if (str[pos-1]=='.')
		pos++;
	temp=str.Left(pos);
	return temp;
}

IString IShaderExpression::GetGLSLString()
{
	if (m_iType==ET_CONSTANT)
	{
		IString temp;
		switch (m_iVarType)
		{
		case VT_INT:
			return IString((int)m_fValue[0]);
		case VT_FLOAT:
			return TrimZeroFloat(IString(m_fValue[0]));
		case VT_VEC2:
			temp="vec2(";
			temp+=TrimZeroFloat(m_fValue[0]);
			temp+=",";
			temp+=TrimZeroFloat(m_fValue[1]);
			temp+=")";
			return temp;
		case VT_VEC3:
			temp="vec3(";
			temp+=TrimZeroFloat(m_fValue[0]);
			temp+=",";
			temp+=TrimZeroFloat(m_fValue[1]);
			temp+=",";
			temp+=TrimZeroFloat(m_fValue[2]);
			temp+=")";
			return temp;
		case VT_VEC4:
			temp="vec4(";
			temp+=TrimZeroFloat(m_fValue[0]);
			temp+=",";
			temp+=TrimZeroFloat(m_fValue[1]);
			temp+=",";
			temp+=TrimZeroFloat(m_fValue[2]);
			temp+=",";
			temp+=TrimZeroFloat(m_fValue[3]);
			temp+=")";
			return temp;
		case VT_MAT3x3:
			temp="mat3(";
			for (int i=0;i<3;i++)
			{
				for (int j=0;j<3;j++)
				{
					temp+=(float)m_mMatrix.GetValue(j,i);
					if (i==2 && j==2)
						temp+=")";
					else
						temp+=",";
				}
			}
			return temp;
		case VT_MAT4x4:
			temp="mat4(";
			for (int i=0;i<4;i++)
			{
				for (int j=0;j<4;j++)
				{
					temp+=(float)m_mMatrix.GetValue(j,i);
					if (i==3 && j==3)
						temp+=")";
					else
						temp+=",";
				}
			}
			return temp;
		}
	}else if (m_iType==ET_ASSEMBLER)
	{
		IString temp;
		switch (m_iVarType)
		{
		case VT_VEC2:
			temp+="vec2(";
			break;
		case VT_VEC3:
			temp+="vec3(";
			break;
		case VT_VEC4:
			temp+="vec4(";
			break;
		}

		for (int i=0;i<m_eExpr.Count();i++)
		{
			temp+=m_eExpr[i].GetGLSLString();
			if (i==m_eExpr.Count()-1)
				temp+=")";
			else
				temp+=",";
		}
		return temp;
	}else if (m_iType==ET_DISASSEMBLER)
	{
		if (m_eExpr[0].GetVarType()==VT_MAT2x2 || 
			m_eExpr[0].GetVarType()==VT_MAT3x3 || 
			m_eExpr[0].GetVarType()==VT_MAT4x4)
		{
			IString temp;
			temp+=m_eExpr[0].GetGLSLString();
			temp+="[";
			temp+=(int)m_fValue[0];
			temp+="]";
			return temp;
		}

		IString temp;
		temp+=m_eExpr[0].GetGLSLString();
		temp+=".";
		temp+=m_sParameter;
		return temp;
	}else if (m_iType==ET_VARIABLE)
	{
		return m_sParameter;
	}
	//--------------------------------
	// Operation
	//--------------------------------
	else if (m_iType==ET_ADDITION)
	{
		IString temp="(";
		temp+=m_eExpr[0].GetGLSLString();
		temp+="+";
		temp+=m_eExpr[1].GetGLSLString();
		temp+=")";
		return temp;
	}else if (m_iType==ET_SUBSTRACT)
	{
		IString temp="(";
		temp+=m_eExpr[0].GetGLSLString();
		temp+="-";
		temp+=m_eExpr[1].GetGLSLString();
		temp+=")";
		return temp;
	}else if (m_iType==ET_MULTIPLY)
	{
		IString temp="(";
		temp+=m_eExpr[0].GetGLSLString();
		temp+="*";
		temp+=m_eExpr[1].GetGLSLString();
		temp+=")";
		return temp;
	}else if (m_iType==ET_DIVIDE)
	{
		IString temp="(";
		temp+=m_eExpr[0].GetGLSLString();
		temp+="/";
		temp+=m_eExpr[1].GetGLSLString();
		temp+=")";
		return temp;
	}
	//--------------------------------
	// Trigo
	//--------------------------------
	else if (m_iType==ET_COS)
	{
		IString temp;
		temp.Printf("cos(%s)",m_eExpr[0].GetGLSLString());
		return temp;
	}else if (m_iType==ET_SIN)
	{
		IString temp;
		temp.Printf("sin(%s)",m_eExpr[0].GetGLSLString());
		return temp;
	}else if (m_iType==ET_DOT)
	{
		IString temp="dot(";
		temp+=m_eExpr[0].GetGLSLString();
		temp+=",";
		temp+=m_eExpr[1].GetGLSLString();
		temp+=")";
		return temp;
	}else if (m_iType==ET_NORMALIZE)
	{
		IString temp;
		temp.Printf("normalize(%s)",m_eExpr[0].GetGLSLString());
		return temp;
	}
	//--------------------------------
	// Texture
	//--------------------------------
	else if (m_iType==ET_TEXTURESAMPLE)
	{
		IString temp;
		temp.Printf("texture2D(%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_TEXTURECOORD)
	{
		IString temp;
		temp.Printf("gl_TexCoord[%s]",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}
	//--------------------------------
	// Conditions
	//--------------------------------
	else if (m_iType==ET_IF)
	{
		IString temp;
		temp.Printf("(%s%s%s ? %s : %s)",m_eExpr[0].GetGLSLString().c_str(),
			m_sParameter.c_str(),
			m_eExpr[1].GetGLSLString().c_str(),
			m_eExpr[2].GetGLSLString().c_str(),
			m_eExpr[3].GetGLSLString().c_str());
		return temp;
	}
	//--------------------------------
	// Common func
	//--------------------------------
	else if (m_iType==ET_ABS)
	{
		IString temp;
		temp.Printf("abs(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_FLOOR)
	{
		IString temp;
		temp.Printf("floor(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_CEIL)
	{
		IString temp;
		temp.Printf("ceil(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_FRACT)
	{
		IString temp;
		temp.Printf("fract(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_MOD)
	{
		IString temp;
		temp.Printf("mod(%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_MIN)
	{
		IString temp;
		temp.Printf("min(%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_MAX)
	{
		IString temp;
		temp.Printf("max(%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_CLAMP)
	{
		IString temp;
		temp.Printf("clamp(%s,%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str(),m_eExpr[2].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_MIX)
	{
		IString temp;
		temp.Printf("mix(%s,%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str(),m_eExpr[2].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_POW)
	{
		IString temp;
		temp.Printf("POW(%s,%s)",m_eExpr[0].GetGLSLString().c_str(),m_eExpr[1].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_SQRT)
	{
		IString temp;
		temp.Printf("sqrt(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_INVSQRT)
	{
		IString temp;
		temp.Printf("inversesqrt(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_EXP)
	{
		IString temp;
		temp.Printf("exp(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_LOG)
	{
		IString temp;
		temp.Printf("log(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_EXP2)
	{
		IString temp;
		temp.Printf("exp2(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}else if (m_iType==ET_LOG2)
	{
		IString temp;
		temp.Printf("log2(%s)",m_eExpr[0].GetGLSLString().c_str());
		return temp;
	}

	return "";
}

void IShaderExpression::_Cos(const IShaderExpression& expr, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(expr);
		result->m_iType=ET_COS;
	}
}

void IShaderExpression::_Sin(const IShaderExpression& expr, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(expr);
		result->m_iType=ET_SIN;
	}
}

void IShaderExpression::_Dot(const IShaderExpression& expr1, const IShaderExpression& expr2, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(expr1);
		result->m_eExpr.Add(expr2);
		result->m_iType=ET_DOT;
	}
}

void IShaderExpression::_Normalize(const IShaderExpression& expr, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(expr);
		result->m_iType=ET_NORMALIZE;
	}
}

void IShaderExpression::_TextureSample(IString texturename, const ISEVec2& expr, ISEVec4* result)
{
	if (result)
	{
		result->m_eExpr.Add(ISEFloat(texturename));
		result->m_eExpr.Add(expr);
		result->m_iType=ET_TEXTURESAMPLE;
	}
}

void IShaderExpression::_TextureCoord(const ISEInt& expr, ISEVec4* result)
{
	if (result)
	{
		result->m_eExpr.Add(expr);
		result->m_iType=ET_TEXTURECOORD;
	}
}

void IShaderExpression::_If(IShaderExpression* A, IShaderExpression* B, IString op, IShaderExpression* Ok, IShaderExpression* Else, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*A);
		result->m_eExpr.Add(*B);
		result->m_eExpr.Add(*Ok);
		result->m_eExpr.Add(*Else);
		result->m_iType=ET_IF;
		result->m_sParameter=op;
	}
}

void IShaderExpression::_Abs(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_ABS;
	}
}

void IShaderExpression::_Floor(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_FLOOR;
	}
}

void IShaderExpression::_Ceil(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_CEIL;
	}
}

void IShaderExpression::_Fract(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_FRACT;
	}
}

void IShaderExpression::_Mod(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val1);
		result->m_eExpr.Add(*val2);
		result->m_iType=ET_MOD;
	}
}

void IShaderExpression::_Min(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val1);
		result->m_eExpr.Add(*val2);
		result->m_iType=ET_MIN;
	}
}

void IShaderExpression::_Max(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val1);
		result->m_eExpr.Add(*val2);
		result->m_iType=ET_MAX;
	}
}

void IShaderExpression::_Clamp(IShaderExpression* val, IShaderExpression* min, IShaderExpression* max, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_eExpr.Add(*min);
		result->m_eExpr.Add(*max);
		result->m_iType=ET_CLAMP;
	}
}

void IShaderExpression::_Mix(IShaderExpression* x, IShaderExpression* y, IShaderExpression* a, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*x);
		result->m_eExpr.Add(*y);
		result->m_eExpr.Add(*a);
		result->m_iType=ET_MIX;
	}
}

void IShaderExpression::_Pow(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val1);
		result->m_eExpr.Add(*val2);
		result->m_iType=ET_POW;
	}
}

void IShaderExpression::_Sqrt(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_SQRT;
	}
}

void IShaderExpression::_InverseSqrt(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_INVSQRT;
	}
}

void IShaderExpression::_Exp(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_EXP;
	}
}

void IShaderExpression::_Log(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_LOG;
	}
}

void IShaderExpression::_Exp2(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_EXP2;
	}
}

void IShaderExpression::_Log2(IShaderExpression* val, IShaderExpression* result)
{
	if (result)
	{
		result->m_eExpr.Add(*val);
		result->m_iType=ET_LOG2;
	}
}

//-----------------------------------------------------------------------------------------
// Int
//-----------------------------------------------------------------------------------------

ISEInt::ISEInt()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_INT;
	m_fValue[0]=0;
}

ISEInt::ISEInt(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_INT;
	m_sParameter=paramName;
}

ISEInt::ISEInt(int val)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_INT;
	m_fValue[0]=val;
}

//-----------------------------------------------------------------------------------------
// Float
//-----------------------------------------------------------------------------------------

ISEFloat::ISEFloat()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_FLOAT;
	m_fValue[0]=0;
}

ISEFloat::ISEFloat(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_FLOAT;
	m_sParameter=paramName;
}

ISEFloat::ISEFloat(float val)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_FLOAT;
	m_fValue[0]=val;
}

//-----------------------------------------------------------------------------------------
// Vec2
//-----------------------------------------------------------------------------------------

ISEVec2::ISEVec2()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_VEC2;
	m_fValue[0]=m_fValue[1]=0;
}

ISEVec2::ISEVec2(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_VEC2;
	m_sParameter=paramName;
}

ISEVec2::ISEVec2(float val1, float val2)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_VEC2;
	m_fValue[0]=val1;
	m_fValue[1]=val2;
}

ISEVec2::ISEVec2(const ISEFloat& val1, const ISEFloat& val2)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC2;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
}

//-----------------------------------------------------------------------------------------
// Vec3
//-----------------------------------------------------------------------------------------

ISEVec3::ISEVec3()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_VEC3;
	m_fValue[0]=m_fValue[1]=m_fValue[2]=0;
}

ISEVec3::ISEVec3(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_VEC3;
	m_sParameter=paramName;
}

ISEVec3::ISEVec3(float val1, float val2, float val3)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_VEC3;
	m_fValue[0]=val1;
	m_fValue[1]=val2;
	m_fValue[2]=val3;
}

ISEVec3::ISEVec3(const ISEFloat& val1, const ISEFloat& val2, const ISEFloat& val3)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC3;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
}

ISEVec3::ISEVec3(const ISEFloat& val1, const ISEVec2& val2)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC3;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
}

ISEVec3::ISEVec3(const ISEVec2& val1, const ISEFloat& val2)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC3;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
}

//-----------------------------------------------------------------------------------------
// Vec4
//-----------------------------------------------------------------------------------------

ISEVec4::ISEVec4()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_VEC4;
	m_fValue[0]=m_fValue[1]=m_fValue[2]=m_fValue[3]=0;
}

ISEVec4::ISEVec4(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_VEC4;
	m_sParameter=paramName;
}

ISEVec4::ISEVec4(float val1, float val2, float val3, float val4)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_VEC4;
	m_fValue[0]=val1;
	m_fValue[1]=val2;
	m_fValue[2]=val3;
	m_fValue[3]=val3;
}

ISEVec4::ISEVec4(const ISEFloat& val1, const ISEFloat& val2, const ISEFloat& val3, const ISEFloat& val4)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC4;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
	m_eExpr.Add(val4);
}

ISEVec4::ISEVec4(const ISEVec2& val1, const ISEFloat& val2, const ISEFloat& val3)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC4;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
}

ISEVec4::ISEVec4(const ISEFloat& val1, const ISEVec2& val2, const ISEFloat& val3)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC4;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
}

ISEVec4::ISEVec4(const ISEFloat& val1, const ISEFloat& val2, const ISEVec2& val3)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC4;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
}

ISEVec4::ISEVec4(const ISEFloat& val1, const ISEVec3& val2)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC4;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
}

ISEVec4::ISEVec4(const ISEVec3& val1, const ISEFloat& val2)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_VEC4;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
}

//-----------------------------------------------------------------------------------------
// Mat2x2
//-----------------------------------------------------------------------------------------

ISEMat2x2::ISEMat2x2()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_MAT2x2;
	m_mMatrix.MakeZero(2);
}

ISEMat2x2::ISEMat2x2(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_MAT2x2;
	m_sParameter=paramName;
}
ISEMat2x2::ISEMat2x2(Math::IMatrix mat)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_MAT2x2;
	if (mat.GetWidth()==mat.GetHeight()==2)
		m_mMatrix=mat;
	else
		m_mMatrix.MakeZero(2);
}

ISEMat2x2::ISEMat2x2(const ISEVec2& val1, const ISEVec2& val2)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_MAT2x2;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
}

//-----------------------------------------------------------------------------------------
// Mat3x3
//-----------------------------------------------------------------------------------------

ISEMat3x3::ISEMat3x3()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_MAT3x3;
	m_mMatrix.MakeZero(3);
}

ISEMat3x3::ISEMat3x3(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_MAT3x3;
	m_sParameter=paramName;
}
ISEMat3x3::ISEMat3x3(Math::IMatrix mat)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_MAT3x3;
	if (mat.GetWidth()==mat.GetHeight()==3)
		m_mMatrix=mat;
	else
		m_mMatrix.MakeZero(3);
}

ISEMat3x3::ISEMat3x3(const ISEVec3& val1, const ISEVec3& val2, const ISEVec3& val3)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_MAT3x3;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
}

//-----------------------------------------------------------------------------------------
// Mat4x4
//-----------------------------------------------------------------------------------------

ISEMat4x4::ISEMat4x4()
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_MAT4x4;
	m_mMatrix.MakeZero(4);
}

ISEMat4x4::ISEMat4x4(IString paramName)
{
	m_iType=ET_VARIABLE;
	m_iVarType=VT_MAT4x4;
	m_sParameter=paramName;
}

ISEMat4x4::ISEMat4x4(Math::IMatrix mat)
{
	m_iType=ET_CONSTANT;
	m_iVarType=VT_MAT4x4;
	if (mat.GetWidth()==mat.GetHeight()==4)
		m_mMatrix=mat;
	else
		m_mMatrix.MakeZero(4);
}

ISEMat4x4::ISEMat4x4(const ISEVec4& val1, const ISEVec4& val2, const ISEVec4& val3, const ISEVec4& val4)
{
	m_iType=ET_ASSEMBLER;
	m_iVarType=VT_MAT3x3;
	m_eExpr.Add(val1);
	m_eExpr.Add(val2);
	m_eExpr.Add(val3);
	m_eExpr.Add(val4);
}

//-----------------------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------------------

ISEVec4 ShaderExpression::TextureSample(IString texturename,ISEVec2 coord)
{
	ISEVec4 temp;
	IShaderExpression::_TextureSample(texturename,coord,&temp);
	return temp;
}

ISEVec4 ShaderExpression::TextureCoord(ISEInt id)
{
	ISEVec4 temp;
	IShaderExpression::_TextureCoord(id,&temp);
	return temp;
}