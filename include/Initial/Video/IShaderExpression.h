//---------------------------------------------
//                Initial Engine
//            Shader expression Module
//
//            By Thibault HENNEQUIN
//              January 20th  2009
//---------------------------------------------

#ifndef _ISHADEREXPRESSION_HEADER_
#define _ISHADEREXPRESSION_HEADER_
#include "Initial/Config.h"

#include "Initial/Math/IMatrix.h"
#include "Initial/Core/IArray.h"
#include "Initial/Core/IString.h"
#include "Initial/IDebugMemoryOn.h"

//Déclaration

#define DECLARE_FRIEND \
	friend class ISEFloat; \
	friend class ISEVec2; \
	friend class ISEVec3; \
	friend class ISEVec4; \
	friend class ISEMat2x2; \
	friend class ISEMat3x3; \
	friend class ISEMat4x4;

#define DECLARE_ADD(Type,Ret) Ret operator +(const Type& expr);
#define DECLARE_SUB(Type,Ret) Ret operator -(const Type& expr);
#define DECLARE_MUL(Type,Ret) Ret operator *(const Type& expr);
#define DECLARE_DIV(Type,Ret) Ret operator /(const Type& expr);

#define DECLARE_OPERATION(Type,Ret) DECLARE_ADD(Type,Ret) \
	DECLARE_SUB(Type,Ret) \
	DECLARE_MUL(Type,Ret) \
	DECLARE_DIV(Type,Ret)

#define DECLARE_OPERATION_SIMPLE(Type) DECLARE_OPERATION(Type,Type)

#define DECLARE_TRIGO_OPERATION(Base) \
	Base Cos(const Base& val); \
	Base Sin(const Base& val); \
	Base ACos(const Base& val); \
	Base ASin(const Base& val); \
	ISEFloat Dot(const Base& val1, const Base& val2); \
	Base Normalize(const Base& val);

#define DECLARE_COMMON_FUNC(Base) \
	Base Abs(const Base& val); \
	Base Floor(const Base& val); \
	Base Ceil(const Base& val); \
	Base Fract(const Base& val); \
	Base Mod(const Base& val1, const Base& val2); \
	Base Min(const Base& val1, const Base& val2); \
	Base Max(const Base& val1, const Base& val2); \
	Base Clamp(const Base& val, const Base& min, const Base& max); \
	Base Mix(const Base& x, const Base& y, const Base& a); \
	Base Pow(const Base& val1, const Base& val2); \
	Base Sqrt(const Base& val); \
	Base InverseSqrt(const Base& val); \
	Base Exp(const Base& val); \
	Base Log(const Base& val); \
	Base Exp2(const Base& val); \
	Base Log2(const Base& val); \
	ISEFloat Len(const Base& val); \
	Base Lerp(const Base& a, const Base& b, const ISEFloat& alpha);

#define DECLARE_IF(Base) \
	Base If(const ISEFloat& A, const ISEFloat& B, Core::IString op, const Base& Ok, const Base& Else); \
	Base If(const ISEVec2& A, const ISEVec2& B, Core::IString op, const Base& Ok, const Base& Else); \
	Base If(const ISEVec3& A, const ISEVec3& B, Core::IString op, const Base& Ok, const Base& Else); \
	Base If(const ISEVec4& A, const ISEVec4& B, Core::IString op, const Base& Ok, const Base& Else);

//Définition
#define DEFINE_OP_BASE(Base,Type,Ret,op,opsymb) Ret Base::operator opsymb(const Type& expr) \
{ \
	Ret temp; \
	temp.m_iType=op; \
	temp.m_eExpr.Add(new IShaderExpression(*this)); \
	temp.m_eExpr.Add(new IShaderExpression(expr)); \
	return temp; \
}

#define DEFINE_NEGATIVE(Base) Base Base::operator -(void) \
{ \
	Base temp; \
	temp.m_iType=ET_SUBSTRACT; \
	temp.m_eExpr.Add(new IShaderExpression(Base())); \
	temp.m_eExpr.Add(new IShaderExpression(*this)); \
	return temp; \
}

#define DEFINE_ADD(Base,Type,Ret) DEFINE_OP_BASE(Base,Type,Ret,ET_ADDITION,+)
#define DEFINE_SUB(Base,Type,Ret) DEFINE_OP_BASE(Base,Type,Ret,ET_SUBSTRACT,-)
#define DEFINE_MUL(Base,Type,Ret) DEFINE_OP_BASE(Base,Type,Ret,ET_MULTIPLY,*)
#define DEFINE_DIV(Base,Type,Ret) DEFINE_OP_BASE(Base,Type,Ret,ET_DIVIDE,/)

#define DEFINE_OPERATION(Base,Type,Ret) DEFINE_ADD(Base,Type,Ret) \
DEFINE_SUB(Base,Type,Ret) \
DEFINE_MUL(Base,Type,Ret) \
DEFINE_DIV(Base,Type,Ret)

#define DEFINE_TRIGO_OPERATION(Base) \
Base Cos(const Base& val) \
{ \
	Base temp; \
	IShaderExpression::_Cos(val,&temp); \
	return temp; \
} \
Base Sin(const Base& val) \
{ \
	Base temp; \
	IShaderExpression::_Sin(val,&temp); \
	return temp; \
} \
Base ACos(const Base& val) \
{ \
	Base temp; \
	IShaderExpression::_ACos(val,&temp); \
	return temp; \
} \
Base ASin(const Base& val) \
{ \
	Base temp; \
	IShaderExpression::_ASin(val,&temp); \
	return temp; \
} \
ISEFloat Dot(const Base& val1, const Base& val2) \
{ \
	ISEFloat temp; \
	IShaderExpression::_Dot(val1,val2,&temp); \
	return temp; \
}\
Base Normalize(const Base& val) \
{ \
	Base temp; \
	IShaderExpression::_Normalize(val,&temp); \
	return temp; \
}

#define DEFINE_DISASSEMBLER(Base,To,Ret) \
Ret Base::To() \
{ \
	Ret temp; \
	temp.m_iType=ET_DISASSEMBLER; \
	temp.m_sParameter=#To; \
	temp.m_eExpr.Add(new IShaderExpression(*this)); \
	return temp; \
}

#define DEFINE_DISASSEMBLER_ARRAY(Base,Ret,Max) \
Ret Base::operator[](int pos) \
{ \
	if (pos<0 || pos>Max) \
		pos=0; \
	Ret temp; \
	temp.m_iType=ET_DISASSEMBLER; \
	temp.m_eExpr.Add(new IShaderExpression(*this)); \
	temp.m_fValue[0]=pos; \
	return temp; \
}

#define DEFINE_IF_BASE(Base,Ret) \
Ret If(const Base& A, const Base& B, Core::IString op, const Ret& Ok, const Ret& Else) \
{ \
	Ret temp; \
	IShaderExpression::_If((IShaderExpression*)&A,(IShaderExpression*)&B,op,(IShaderExpression*)&Ok,(IShaderExpression*)&Else,&temp); \
	return temp; \
}

#define DEFINE_IF(Base) DEFINE_IF_BASE(Base,ISEFloat) \
	DEFINE_IF_BASE(Base,ISEVec2) \
	DEFINE_IF_BASE(Base,ISEVec3) \
	DEFINE_IF_BASE(Base,ISEVec4) \
	DEFINE_IF_BASE(Base,ISEMat2x2) \
	DEFINE_IF_BASE(Base,ISEMat3x3) \
	DEFINE_IF_BASE(Base,ISEMat4x4)

#define DEFINE_COMMON_FUNC_BASE_ONE(ReturnBase,Base,Name) \
ReturnBase Name(const Base& A) \
{ \
	ReturnBase temp; \
	IShaderExpression::_##Name((IShaderExpression*)&A,&temp); \
	return temp; \
}

#define DEFINE_COMMON_FUNC_BASE_TWO(Base,Name) \
Base Name(const Base& A, const Base& B) \
{ \
	Base temp; \
	IShaderExpression::_##Name((IShaderExpression*)&A,(IShaderExpression*)&B,&temp); \
	return temp; \
}

#define DEFINE_COMMON_FUNC_BASE_THREE(Base,Name) \
Base Name(const Base& A, const Base& B, const Base& C) \
{ \
	Base temp; \
	IShaderExpression::_##Name((IShaderExpression*)&A,(IShaderExpression*)&B,(IShaderExpression*)&C,&temp); \
	return temp; \
}

#define DEFINE_COMMON_FUNC_BASE_THREE_DIFF(Base,Base2,Base3,Name) \
Base Name(const Base& A, const Base2& B, const Base3& C) \
{ \
	Base temp; \
	IShaderExpression::_##Name((IShaderExpression*)&A,(IShaderExpression*)&B,(IShaderExpression*)&C,&temp); \
	return temp; \
}

#define DEFINE_COMMON_FUNC(Base) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Abs) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Floor) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Ceil) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Fract) \
	DEFINE_COMMON_FUNC_BASE_TWO(Base,Mod) \
	DEFINE_COMMON_FUNC_BASE_TWO(Base,Min) \
	DEFINE_COMMON_FUNC_BASE_TWO(Base,Max) \
	DEFINE_COMMON_FUNC_BASE_THREE(Base,Clamp) \
	DEFINE_COMMON_FUNC_BASE_THREE(Base,Mix) \
	DEFINE_COMMON_FUNC_BASE_TWO(Base,Pow) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Sqrt) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,InverseSqrt) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Exp) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Log) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Exp2) \
	DEFINE_COMMON_FUNC_BASE_ONE(Base,Base,Log2) \
	DEFINE_COMMON_FUNC_BASE_ONE(ISEFloat,Base,Len) \
	DEFINE_COMMON_FUNC_BASE_THREE_DIFF(Base,Base,ISEFloat,Lerp)

namespace Initial
{
	namespace ShaderExpression
	{
		class ISEInt;
		class ISEFloat;
		class ISEVec2;
		class ISEVec3;
		class ISEVec4;
		class ISEMat2x2;
		class ISEMat3x3;
		class ISEMat4x4;

		DECLARE_TRIGO_OPERATION(ISEFloat)
		DECLARE_TRIGO_OPERATION(ISEVec2)
		DECLARE_TRIGO_OPERATION(ISEVec3)
		DECLARE_TRIGO_OPERATION(ISEVec4)

		DECLARE_COMMON_FUNC(ISEFloat)
		DECLARE_COMMON_FUNC(ISEVec2)
		DECLARE_COMMON_FUNC(ISEVec3)
		DECLARE_COMMON_FUNC(ISEVec4)

		class IShaderExpression
		{
		protected:
			enum VarType
			{
				VT_INT,
				VT_FLOAT,
				VT_VEC2,
				VT_VEC3,
				VT_VEC4,
				VT_MAT2x2,
				VT_MAT3x3,
				VT_MAT4x4
			};

			enum ExpressionType
			{
				ET_CONSTANT,
				ET_ASSEMBLER,
				ET_DISASSEMBLER,
				ET_VARIABLE,
				
				ET_ADDITION,
				ET_SUBSTRACT,
				ET_MULTIPLY,
				ET_DIVIDE,
				ET_DOT,
				ET_CROSS,

				ET_COS,
				ET_SIN,
				ET_ACOS,
				ET_ASIN,
				
				ET_NORMALIZE,

				ET_TEXTURESAMPLE,
				ET_TEXTURECOORD,
				ET_SCREENCOORD,

				ET_IF,

				ET_ABS,
				ET_FLOOR,
				ET_CEIL,
				ET_FRACT,
				ET_MOD,
				ET_MIN,
				ET_MAX,
				ET_CLAMP,
				ET_MIX,
				ET_POW,
				ET_SQRT,
				ET_INVSQRT,
				ET_EXP,
				ET_LOG,				
				ET_EXP2,
				ET_LOG2,
				ET_LEN,
				ET_LERP,
				
				//ET_LINK,
			};

		public:
			IShaderExpression();
			IShaderExpression(const IShaderExpression& expr);
			~IShaderExpression();

			virtual void Optimise();
			virtual Core::IString GetGLSLString()/*=0*/;
			VarType GetVarType();

			void operator =(const IShaderExpression& expr);

			void operator +=(const IShaderExpression& expr);
			void operator -=(const IShaderExpression& expr);
			void operator *=(const IShaderExpression& expr);
			void operator /=(const IShaderExpression& expr);

			static void _Cos(const IShaderExpression& expr, IShaderExpression* result);
			static void _Sin(const IShaderExpression& expr, IShaderExpression* result);
			static void _ACos(const IShaderExpression& expr, IShaderExpression* result);
			static void _ASin(const IShaderExpression& expr, IShaderExpression* result);
			static void _Dot(const IShaderExpression& expr1, const IShaderExpression& expr2, IShaderExpression* result);
			static void _Normalize(const IShaderExpression& expr, IShaderExpression* result);

			static void _TextureSample(Core::IString texturename, const ISEVec2& expr, ISEVec4* result);
			static void _TextureCoord(const ISEInt& expr, ISEVec4* result);
			static void _ScreenCoord(ISEVec4* result);

			static void _If(IShaderExpression* A, IShaderExpression* B, Core::IString op, IShaderExpression* Ok, IShaderExpression* Else, IShaderExpression* result);

			static void _Abs(IShaderExpression* val, IShaderExpression* result);
			static void _Floor(IShaderExpression* val, IShaderExpression* result);
			static void _Ceil(IShaderExpression* val, IShaderExpression* result);
			static void _Fract(IShaderExpression* val, IShaderExpression* result);
			static void _Mod(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result);
			/*Base mod(const Base& val1, const ISEFloat& val2);*/
			static void _Min(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result);
			static void _Max(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result);
			static void _Clamp(IShaderExpression* val, IShaderExpression* min, IShaderExpression* max, IShaderExpression* result);
			static void _Mix(IShaderExpression* x, IShaderExpression* y, IShaderExpression* a, IShaderExpression* result);
			/*Base mix(const Base& x, const Base& y, const ISEFloat& a);*/
			static void _Pow(IShaderExpression* val1, IShaderExpression* val2, IShaderExpression* result);
			static void _Sqrt(IShaderExpression* val, IShaderExpression* result);
			static void _InverseSqrt(IShaderExpression* val, IShaderExpression* result);
			static void _Exp(IShaderExpression* val, IShaderExpression* result);
			static void _Log(IShaderExpression* val, IShaderExpression* result);
			static void _Exp2(IShaderExpression* val, IShaderExpression* result);
			static void _Log2(IShaderExpression* val, IShaderExpression* result);
			static void _Len(IShaderExpression* val, IShaderExpression* result);
			static void _Lerp(IShaderExpression* a, IShaderExpression* b, IShaderExpression* Alpha, IShaderExpression* result);

		protected:
			void _Copy(const IShaderExpression& expr);
		protected:
			ExpressionType				m_iType;
			VarType						m_iVarType;

			Core::IArray<IShaderExpression*>	m_eExpr;

			Core::IString						m_sParameter;
			float						m_fValue[4];
			Math::IMatrix				m_mMatrix;
		};

		class ISEInt : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEInt();
			ISEInt(Core::IString paramName);
			ISEInt(int val);
		};

		class ISEFloat : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEFloat();
			ISEFloat(Core::IString paramName);
			ISEFloat(float val);
			ISEFloat(ISEInt val);

			DECLARE_OPERATION_SIMPLE(ISEFloat)
			DECLARE_OPERATION_SIMPLE(ISEVec2)
			DECLARE_OPERATION_SIMPLE(ISEVec3)
			DECLARE_OPERATION_SIMPLE(ISEVec4)
			DECLARE_OPERATION_SIMPLE(ISEMat2x2)
			DECLARE_OPERATION_SIMPLE(ISEMat3x3)
			DECLARE_OPERATION_SIMPLE(ISEMat4x4)

			ISEFloat operator -(void);
		};

		class ISEVec2 : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEVec2();
			ISEVec2(Core::IString paramName);
			ISEVec2(float val1, float val2);
			ISEVec2(const ISEFloat& val1, const ISEFloat& val2);

			ISEFloat x();
			ISEFloat y();

			DECLARE_OPERATION_SIMPLE(ISEVec2)
			DECLARE_OPERATION(ISEFloat,ISEVec2)

			ISEFloat operator[] (int pos);
			ISEVec2 operator -(void);
		};

		class ISEVec3 : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEVec3();
			ISEVec3(Core::IString paramName);
			ISEVec3(float val1, float val2, float val3);
			ISEVec3(const ISEFloat& val1, const ISEFloat& val2, const ISEFloat& val3);
			ISEVec3(const ISEFloat& val1, const ISEVec2& val2);
			ISEVec3(const ISEVec2& val1, const ISEFloat& val2);

			ISEFloat	x();
			ISEFloat	y();
			ISEFloat	z();
			ISEVec2		xy();
			ISEVec2		xz();
			ISEVec2		xw();
			ISEVec2		yz();
			ISEVec2		yw();
			ISEVec2		zw();

			DECLARE_OPERATION_SIMPLE(ISEVec3)
			DECLARE_OPERATION(ISEFloat,ISEVec3)

			ISEFloat operator[] (int pos);
			ISEVec3 operator -(void);
		};

		class ISEVec4 : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEVec4();
			ISEVec4(Core::IString paramName);
			ISEVec4(float val1, float val2, float val3, float val4);
			ISEVec4(const ISEFloat& val1, const ISEFloat& val2, const ISEFloat& val3, const ISEFloat& val4);
			ISEVec4(const ISEVec2& val1, const ISEFloat& val2, const ISEFloat& val3);
			ISEVec4(const ISEFloat& val1, const ISEVec2& val2, const ISEFloat& val3);
			ISEVec4(const ISEFloat& val1, const ISEFloat& val2, const ISEVec2& val3);
			ISEVec4(const ISEFloat& val1, const ISEVec3& val2);
			ISEVec4(const ISEVec3& val1, const ISEFloat& val2);

			ISEVec4(const ISEFloat& val);
			ISEVec4(const ISEVec2& val);
			ISEVec4(const ISEVec3& val);

			ISEFloat	x();
			ISEFloat	y();
			ISEFloat	z();
			ISEFloat	w();
			ISEVec2		xy();
			ISEVec2		xz();
			ISEVec2		xw();
			ISEVec2		yz();
			ISEVec2		yw();
			ISEVec2		zw();
			ISEVec3		xyz();
			ISEVec3		xyw();
			ISEVec3		xzw();
			ISEVec3		yzw();

			DECLARE_OPERATION_SIMPLE(ISEVec4)
			DECLARE_OPERATION(ISEFloat,ISEVec4)

			ISEFloat operator[] (int pos);
			ISEVec4 operator -(void);
		};

		class ISEMat2x2 : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEMat2x2();
			ISEMat2x2(Core::IString paramName);
			ISEMat2x2(Math::IMatrix mat);
			ISEMat2x2(const ISEVec2& val1, const ISEVec2& val2);

			DECLARE_OPERATION_SIMPLE(ISEMat2x2)
			DECLARE_OPERATION(ISEFloat,ISEMat2x2)

			ISEVec2 operator[] (int pos);
			ISEMat2x2 operator -(void);
		};

		class ISEMat3x3 : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEMat3x3();
			ISEMat3x3(Core::IString paramName);
			ISEMat3x3(Math::IMatrix mat);
			ISEMat3x3(const ISEVec3& val1, const ISEVec3& val2, const ISEVec3& val3);		

			DECLARE_OPERATION_SIMPLE(ISEMat3x3)
			DECLARE_OPERATION(ISEFloat,ISEMat3x3)

			ISEVec3 operator[] (int pos);
			ISEMat3x3 operator -(void);
		};

		class ISEMat4x4 : public IShaderExpression
		{
			DECLARE_FRIEND
		public:
			ISEMat4x4();
			ISEMat4x4(Core::IString paramName);
			ISEMat4x4(Math::IMatrix mat);
			ISEMat4x4(const ISEVec4& val1, const ISEVec4& val2, const ISEVec4& val3, const ISEVec4& val4);		

			DECLARE_OPERATION_SIMPLE(ISEMat4x4)
			DECLARE_OPERATION(ISEFloat,ISEMat4x4)

			ISEVec4 operator[] (int pos);
			ISEMat4x4 operator -(void);
		};

		enum IfOperator
		{
			IIFO_LESS,
			IIFO_LESS_EQUAL,
			IIFO_GREATER,
			IIFO_GREATER_EQUAL,
			IIFO_EQUAL,
			IIFO_NOT_EQUAL
		};

		ISEVec4 TextureCoord(ISEInt id=ISEInt(0));
		ISEVec4 TextureSample(Core::IString texturename,ISEVec2 coord=TextureCoord().xy());
		ISEVec4 ScreenCoord();

		DECLARE_IF(ISEFloat)
		DECLARE_IF(ISEVec2)
		DECLARE_IF(ISEVec3)
		DECLARE_IF(ISEVec4)
		DECLARE_IF(ISEMat2x2)
		DECLARE_IF(ISEMat3x3)
		DECLARE_IF(ISEMat4x4)
	}
}


//Core::IString easy conversion
//Core::IString Iftoa(float val);

#endif