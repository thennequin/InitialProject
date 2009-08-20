//---------------------------------------------
//                Initial Engine
//          Material Expression Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATERIALEXPRESSION_HEADER_
#define _IMATERIALEXPRESSION_HEADER_

#include "Initial/IObject.h"
#include "Initial/Video/IShaderExpression.h"
#include "FlowLib/GraphShapeSky.h"
#include "Initial/io/IXMLParser.h"

class MaterialGraph;

namespace Initial
{
	namespace Video
	{
		class IRenderDriver;
		class ITexture;
		class IShader;
		class IMaterial;

		class IMaterialExpression : public IObject, public GraphShape
		{
			friend class IMaterial;
			friend class MaterialGraph;
			DECLARE_OBJECT(IMaterialExpression,IObject)

			typedef struct
			{
				IMaterialExpression *Epxr;
				UINT Count;
			}ExprLink;
		public:
			IMaterialExpression();

			int GetExprId() { return m_iId; }
			Core::IString GetCategorie() { return Categorie; }

			void ClearInputs();
			int AddInput(Core::IString Name);

			void ClearOutputs();
			int AddOutput(Core::IString Name);

			///*void AddProperty(Core::IString Name,int Type,wxArrayString choices=wxArrayString());
			//PropertyList GetPropertyList() { return Properties; }
			//void SetPropertyValue(Core::IString Name,Core::IString Value);*/

			bool InputIsConnect(int id);
			//
			ShaderExpression::ISEVec4 GetInputExpr(int id);
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);

			ShaderExpression::ISEVec4 GetPropertyValueExpr(Core::IString name);
			Core::IColor GetPropertyValue(Core::IString name);

			/*void OnPropertyChange();
			virtual void OnPropertyChange(Core::IString name);*/

			virtual void OnPropertyChange(Core::IString name);

			virtual void OnDraw(wxDC& dc);

			void ParseNode(IO::XML::IXMLNode *node);
			virtual Core::IString WriteNode(bool writeParams=true);

			void GenerateShader(Core::IString& fragment, Core::IString& vertex);
		protected:
			void GetLinkedExpre(Core::IList<ExprLink>& Linked);
			//UINT LinkedCount(); // Get the number of expression linked to outputs
		protected:
			Video::IMaterial *m_pMaterial;
			//static unsigned int NextId;
			unsigned int m_iId;

			Core::IString Categorie;
			Core::IString m_sComment;

			IPropertyList m_aProperties;
		};

		//-------------------------------------------------------------------------
		// Final shader
		//-------------------------------------------------------------------------
		class IMaterialExpressionFinal : public IMaterialExpression
		{
			DECLARE_OBJECT(IMaterialExpressionFinal,IMaterialExpression)
		public:
			IMaterialExpressionFinal();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
			virtual void OnPropertyChange(Core::IString name);
		};

		//-------------------------------------------------------------------------
		// Base Expression
		//-------------------------------------------------------------------------
		class IMaterialExpressionBase : public IMaterialExpression
		{
			DECLARE_OBJECT(IMaterialExpressionBase,IMaterialExpression)
		public:
			IMaterialExpressionBase();
		};



		//-------------------------------------------------------------------------
		// Texture sampler
		//-------------------------------------------------------------------------
		class IMaterialExpressionTextureSampler : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionTextureSampler,IMaterialExpression)
		public:
			IMaterialExpressionTextureSampler();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
			virtual void OnPropertyChange(Core::IString name);
		};

		//-------------------------------------------------------------------------
		// Texture sampler
		//-------------------------------------------------------------------------
		class IMaterialExpressionScreenBuffer : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionScreenBuffer,IMaterialExpression)
		public:
			IMaterialExpressionScreenBuffer();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Texture coord
		//-------------------------------------------------------------------------
		class IMaterialExpressionTextureCoord : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionTextureCoord,IMaterialExpression)
		public:
			IMaterialExpressionTextureCoord();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Screen coord
		//-------------------------------------------------------------------------
		class IMaterialExpressionScreenCoord : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionScreenCoord,IMaterialExpression)
		public:
			IMaterialExpressionScreenCoord();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};


		//-------------------------------------------------------------------------
		// Texture coord
		//-------------------------------------------------------------------------
		class IMaterialExpressionVertexColor : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionVertexColor,IMaterialExpression)
		public:
			IMaterialExpressionVertexColor();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// If
		//-------------------------------------------------------------------------
		class IMaterialExpressionIf : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionIf,IMaterialExpression)
		public:
			IMaterialExpressionIf();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Time
		//-------------------------------------------------------------------------
		class IMaterialExpressionTime : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionTime,IMaterialExpression)
		public:
			IMaterialExpressionTime();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Constant float
		//-------------------------------------------------------------------------
		class IMaterialExpressionConstant : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionConstant,IMaterialExpression)
		public:
			IMaterialExpressionConstant();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
			virtual void OnPropertyChange(Core::IString name);
		};

		//-------------------------------------------------------------------------
		// Constant Vec2
		//-------------------------------------------------------------------------
		class IMaterialExpressionConstantVec2 : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionConstantVec2,IMaterialExpression)
		public:
			IMaterialExpressionConstantVec2();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
			virtual void OnPropertyChange(Core::IString name);
		};

		//-------------------------------------------------------------------------
		// Constant Vec3
		//-------------------------------------------------------------------------
		class IMaterialExpressionConstantVec3 : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionConstantVec3,IMaterialExpression)
		public:
			IMaterialExpressionConstantVec3();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
			virtual void OnPropertyChange(Core::IString name);
		};

		//-------------------------------------------------------------------------
		// Constant Vec4
		//-------------------------------------------------------------------------
		class IMaterialExpressionConstantVec4 : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionConstantVec4,IMaterialExpression)
		public:
			IMaterialExpressionConstantVec4();
			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
			virtual void OnPropertyChange(Core::IString name);
		};

		//-------------------------------------------------------------------------
		// Disassembler
		//-------------------------------------------------------------------------
		class IMaterialExpressionDisassembler : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionDisassembler,IMaterialExpression)
		public:
			IMaterialExpressionDisassembler();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Assembler
		//-------------------------------------------------------------------------
		class IMaterialExpressionAssembler : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionAssembler,IMaterialExpression)
		public:
			IMaterialExpressionAssembler();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Sinus
		//-------------------------------------------------------------------------
		class IMaterialExpressionSin : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionSin,IMaterialExpression)
		public:
			IMaterialExpressionSin();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Inverse Sinus
		//-------------------------------------------------------------------------
		class IMaterialExpressionASin : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionASin,IMaterialExpression)
		public:
			IMaterialExpressionASin();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Length
		//-------------------------------------------------------------------------
		class IMaterialExpressionLength : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionLength,IMaterialExpression)
		public:
			IMaterialExpressionLength();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Absolute
		//-------------------------------------------------------------------------
		class IMaterialExpressionAbs : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionAbs,IMaterialExpression)
		public:
			IMaterialExpressionAbs();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Addition
		//-------------------------------------------------------------------------
		class IMaterialExpressionAdd : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionAdd,IMaterialExpression)
		public:
			IMaterialExpressionAdd();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Subtract
		//-------------------------------------------------------------------------
		class IMaterialExpressionSub : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionSub,IMaterialExpression)
		public:
			IMaterialExpressionSub();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Multiplication
		//-------------------------------------------------------------------------
		class IMaterialExpressionMult : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionMult,IMaterialExpression)
		public:
			IMaterialExpressionMult();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Divide
		//-------------------------------------------------------------------------
		class IMaterialExpressionDiv : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionDiv,IMaterialExpression)
		public:
			IMaterialExpressionDiv();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Lerp
		//-------------------------------------------------------------------------
		class IMaterialExpressionLerp : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionLerp,IMaterialExpression)
		public:
			IMaterialExpressionLerp();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Rotator
		//-------------------------------------------------------------------------
		class IMaterialExpressionRotator : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionRotator,IMaterialExpression)
		public:
			IMaterialExpressionRotator();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};

		//-------------------------------------------------------------------------
		// Panner
		//-------------------------------------------------------------------------
		class IMaterialExpressionPanner : public IMaterialExpressionBase
		{
			DECLARE_OBJECT(IMaterialExpressionPanner,IMaterialExpression)
		public:
			IMaterialExpressionPanner();

			virtual ShaderExpression::ISEVec4 GetOutput(int OutId);
		};
	}
}

#endif
