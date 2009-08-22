//---------------------------------------------
//                Initial Engine
//       Material Expression source Module
//
//            By Thibault HENNEQUIN

//---------------------------------------------

#include "Initial/Core/IUtil.h"
#include "Initial/Video/IMaterialExpression.h"
#include "Initial/Video/IMaterial.h"
#include "Initial/IDevice.h"
#include "Initial/IRessourceManager.h"
#include "FlowLib/FlowGraph.h"

//typedef test IMaterialExpressionBase;
using namespace Initial::Core;
using namespace Initial::ShaderExpression;

namespace Initial
{
	namespace Video
	{
		IMPLEMENT_ABSTRACT_OBJECT(IMaterialExpression,IObject)
		IMPLEMENT_OBJECT(IMaterialExpressionFinal,IMaterialExpression)
		IMPLEMENT_ABSTRACT_OBJECT(IMaterialExpressionBase,IMaterialExpression)
			IMPLEMENT_OBJECT(IMaterialExpressionTextureSampler,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionScreenBuffer,IMaterialExpressionBase)			
			IMPLEMENT_OBJECT(IMaterialExpressionTextureCoord,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionScreenCoord,IMaterialExpressionBase)			
			IMPLEMENT_OBJECT(IMaterialExpressionVertexColor,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionIf,IMaterialExpressionBase)			

			//Var
			IMPLEMENT_OBJECT(IMaterialExpressionTime,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionConstant,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionConstantVec2,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionConstantVec3,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionConstantVec4,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionDisassembler,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionAssembler,IMaterialExpressionBase)

			//Math
			IMPLEMENT_OBJECT(IMaterialExpressionAdd,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionSub,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionMult,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionDiv,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionSin,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionASin,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionLength,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionAbs,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionLerp,IMaterialExpressionBase)

			//Coordinator
			IMPLEMENT_OBJECT(IMaterialExpressionRotator,IMaterialExpressionBase)
			IMPLEMENT_OBJECT(IMaterialExpressionPanner,IMaterialExpressionBase)

		IMaterialExpression::IMaterialExpression()
		{
			Deletable=true;
			Preview=true;
			//m_iId=NextId++;
			m_pMaterial=NULL;
		}

		void IMaterialExpression::ClearInputs()
		{
			Inputs.Clear();
		}

		int IMaterialExpression::AddInput(Core::IString Name)
		{
			Inputs.Add(new InOutput(Name.c_str(),false));
			return Inputs.Count()-1;
		}

		void IMaterialExpression::ClearOutputs()
		{
			Outputs.Clear();
		}

		int IMaterialExpression::AddOutput(Core::IString Name)
		{
			Outputs.Add(new InOutput(Name.c_str(),true));
			return Outputs.Count()-1;
		}

		ISEVec4 IMaterialExpression::GetOutput(int OutId)
		{
			return ISEVec4();
		}

		bool IMaterialExpression::InputIsConnect(int id)
		{
			IMaterialExpression* shape=(IMaterialExpression*)GetShapeOnInput(id);
			if (shape)
				return true;
			return false;
		}

		ISEVec4 IMaterialExpression::GetInputExpr(int id)
		{
			IMaterialExpression* shape=(IMaterialExpression*)GetShapeOnInput(id);
			if (shape)
			{
				IString str;
				str.Printf("Func_BL%d_%d()",shape->GetExprId(),GetShapeOutputIdOnInput(id));
				//str.Printf("Func_BL%d_%d_val",shape->GetShapeId(),GetShapeOutputIdOnInput(id));
				return ISEVec4(str);
			}
			return ISEVec4(0,0,0,0);
		}

		ISEVec4 IMaterialExpression::GetPropertyValueExpr(Core::IString name)
		{
			IProperty *prop = GetPropertyList().GetProperty(name);
			if (prop)
			{
				IColor val = prop->GetColor();
				return ISEVec4(val.x,val.y,val.z,val.w);
			}

			return ISEVec4();
		}

		Core::IColor IMaterialExpression::GetPropertyValue(Core::IString name)
		{
			IProperty *prop = GetPropertyList().GetProperty(name);
			if (prop)
			{
				return prop->GetColor();
			}
			return Core::IColor();
		}

		void IMaterialExpression::OnDraw(wxDC& dc)
		{
			// Draw comment
			int fh=12;

			float Zoom=GetCanvas()->GetZoom();
			wxFont font = dc.GetFont();
			font.SetPointSize(fh);
			dc.SetTextForeground(wxColour(0,0,192));
			dc.SetFont(font);
			dc.DrawText(m_sComment.c_str(),GetX()/Zoom+DrawParam.InOutBlockSize,GetY()/Zoom-fh*1.5);
		}

		void IMaterialExpression::ParseNode(IO::XML::IXMLNode *node)
		{
			if (node)
			{
				m_iId = Iatof(node->GetValue("Id"));
				int x = Iatof(node->GetValue("PosX"));
				int y = Iatof(node->GetValue("PosY"));
				SetPos(x,y);
				
				IO::XML::IXMLNode *param = node->GetChild(0);

				while (param)
				{
					if (param->GetName().ToLower()=="parameter")
					{
						SetPropertyValue(param->GetValue("Name"),param->GetValue("Value"));
					}
					param=param->GetNext();
				}
			}
		}

		void IMaterialExpression::OnPropertyChange(Core::IString name)
		{
			if (m_pMaterial)
				m_pMaterial->CompileV2();
			BaseClass::OnPropertyChange(name);
		}

		IString IMaterialExpression::WriteNode(bool writeParams)
		{
			IString out,temp;			
			out.Printf("\t\t<Expression Type=\"%s\" Id=\"%d\" PosX=\"%d\" PosY=\"%d\">\n",
				GetClass()->GetName().c_str(),m_iId,GetX(),GetY());

			if (GetClass()->GetName()=="ImaterialExpressionFinal")
				writeParams=false;

			if (writeParams)
			{
				IPropertyList& list = GetPropertyList();

				for (UINT i=0;i<list.Count();i++)
				{
					IProperty *prop = list.GetProperty(i);
					if (prop)
					{
						temp.Printf("\t\t\t<Parameter Name=\"%s\" Value=\"%s\"/>\n",prop->GetName().c_str(),prop->GetString().c_str());
						out+=temp;
					}
				}
			}

			for (int i=0;i<GetInputCount();i++)
			{
				IMaterialExpression *outshape = (IMaterialExpression*)GetShapeOnInput(i);
				if (outshape)
				{
					temp.Printf("\t\t\t<Connect InputId=\"%d\" ExprId=\"%d\" OutputId=\"%d\"/>\n",i,outshape->GetExprId(),GetShapeOutputIdOnInput(i));
					out+=temp;
				}
			}

			out+="\t\t</Expression>";

			return out;
		}
		
		void IMaterialExpression::GenerateShader(Core::IString& fragment, Core::IString& vertex)
		{
			//printf("GenerateShader\n");
			IList<ExprLink> Linked;

			GetLinkedExpre(Linked);

			IString Func, FuncHeader, Header;

			Header = "uniform sampler2D Texture0;\n\
uniform sampler2D Texture1;\n\
uniform sampler2D Texture2;\n\
uniform sampler2D Texture3;\n\
uniform sampler2D Texture4;\n\
uniform sampler2D Texture5;\n\
uniform sampler2D Texture6;\n\
uniform sampler2D Texture7;\n\n\
uniform sampler2D ScreenDepth;\n\n\
uniform sampler2D ScreenColor;\n\n\
uniform float Time;\n\
uniform vec2 ViewportSize;\n\
\
varying vec3 v_ViewDir;\n\
varying vec3 m_Tangent, m_BiNormal, m_Normal;\n";


			for (IList<ExprLink>::Iterator ite=Linked.Ite();ite;ite++)
			{
				if (ite.GetData().Epxr /*&& ite.GetData().Count>1*/)
				{
					for (UINT i=0;i<ite.GetData().Epxr->GetOutputCount();i++)
					{
						ISEVec4 outVec4 = ite.GetData().Epxr->GetOutput(i);

						//printf("%s %d\n",ite.GetData().Epxr->GetName(),ite.GetData().Count);
						//Func
						IString func,temp,out;
						func.Printf("Func_BL%d_%d",ite.GetData().Epxr->GetExprId(),i);
						//out = "vec4 "+func+"_val = "+outVec4.GetGLSLString()+";\n";

						if (ite.GetData().Count>1)
						{

						temp.Printf("// %s\nvec4 %s_val;\nbool %s_Init=false;\n",ite.GetData().Epxr->GetName().c_str(),func.c_str(),func.c_str());
						out="\
vec4 "+func+"()\n\
{\n\
	if ("+func+"_Init==false){\n\
		"+func+"_Init = true;\n\
		"+func+"_val = "+outVec4.GetGLSLString()+";\n\
	}\n\
	return "+func+"_val;\n\
}\n\n";
						}else{
						out="// "+ite.GetData().Epxr->GetName()+"\n\
vec4 "+func+"()\n\
{\n\
	return "+outVec4.GetGLSLString()+";\n\
}\n\n";
						}
						Func+=temp+out;
						FuncHeader+="vec4 "+func+"();\n";
					}
				}
			}

			
			IString FBO;
			bool withFBO=true;
			if (withFBO)
			{
				ISEVec4 diffuse = GetOutput(0);
				ISEVec4 emissive = GetOutput(1);
				ISEFloat alpha = GetOutput(2).x();
				ISEVec4 specular = GetOutput(4);
				IString AlphaTest;
				float  LightMode=0;
				if (m_pMaterial)
				{
					int mode = m_pMaterial->GetProperty("Blend mode")->GetFloat();
					float clip = m_pMaterial->GetProperty("Mask clip")->GetFloat();
					LightMode = m_pMaterial->GetProperty("Light mode")->GetFloat()+1;
					if (mode==0)
					{
						//Nothing
					}else if (mode==1)
					{
						AlphaTest=
	"if ("+alpha.GetGLSLString()+"<="+IString(clip)+")\n\
		discard;\n";
					}else{
					}
				}
				ISEVec4 normal = ISEVec4((GetOutput(3).xyz()+1.0)*0.5,1.0/LightMode);

				FBO=AlphaTest+"\
	gl_FragData[0]="+diffuse.GetGLSLString()+";\n\
	gl_FragData[1]="+emissive.GetGLSLString()+";\n\
	gl_FragData[2]="+normal.GetGLSLString()+";\n\
	gl_FragData[3]="+specular.GetGLSLString()+";\n";
			}else{
				ISEVec4 diffuse = ISEVec4(GetOutput(0).xyz(),GetOutput(2).x());
				FBO="\
	gl_FragColor="+diffuse.GetGLSLString()+";\n";
			}

			IString Main=
"void main()\n\
{\n"+FBO+"\
}\n";
			fragment = Header+"\n"+FuncHeader+"\n"+Func+"\n"+Main;

			//printf(fragment.c_str());

			vertex = "varying vec3 m_Normal;\
void main( void )\
{\
	gl_BackColor = gl_Color;\
	gl_FrontColor = gl_Color;\
	gl_TexCoord[0]=gl_MultiTexCoord0;\
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
	gl_Position = ftransform();\
	m_Normal = normalize(gl_NormalMatrix * gl_Normal);\
}";

			//printf("%s\n",fragment);
		}

		void IMaterialExpression::GetLinkedExpre(IList<ExprLink>& Linked)
		{
			for (UINT i=0;i<GetInputCount();i++)
			{
				IMaterialExpression *expr = (IMaterialExpression*)GetShapeOnInput(i);
				if (expr)
				{
					bool Found=false;
					for (IList<ExprLink>::Iterator ite=Linked.Ite();ite;ite++)
					{
						if (ite.GetData().Epxr==expr)
						{
							ite.GetData().Count++;
							Found=true;
							break;
						}
					}
					if (Found==false)
					{
						//int OutId = GetShapeOutputIdOnInput(id);
						ExprLink link;
						link.Epxr=expr;
						link.Count=1;
						Linked.PushFront(link);
						expr->GetLinkedExpre(Linked);
					}
				}
			}
		}


		//-------------------------------------------------------------------------
		// IMaterialExpressionFinal
		//-------------------------------------------------------------------------

		IMaterialExpressionFinal::IMaterialExpressionFinal()
		{
			m_iId=0;
			Deletable=false;
			Preview=false;
			Name="Material";

			//For Initial Engine
			AddInput("Diffuse");
			AddInput("Emissive");
			AddInput("Alpha");
			AddInput("Normal");
			AddInput("Specular");
			AddInput("Power");

			/*AddProperty("Texture1",IPT_RESSOURCE_ITX);
			AddProperty("Texture2",IPT_RESSOURCE_ITX);
			AddProperty("Texture3",IPT_RESSOURCE_ITX);
			AddProperty("Texture4",IPT_RESSOURCE_ITX);
			AddProperty("Texture5",IPT_RESSOURCE_ITX);
			AddProperty("Texture6",IPT_RESSOURCE_ITX);
			AddProperty("Texture7",IPT_RESSOURCE_ITX);
			AddProperty("Texture8",IPT_RESSOURCE_ITX);

			Core::IList<Core::IString> strs;
			strs.PushBack(IString("Solid"));
			strs.PushBack(IString("Masked"));
			strs.PushBack(IString("Translucent"));
			AddProperty("Blend mode",IPT_CHOICE,NULL,strs);
			AddProperty("Mask clip",IPT_FLOAT);
			SetPropertyValue("Mask clip","0.5");*/
		}

		ISEVec4 IMaterialExpressionFinal::GetOutput(int OutId)
		{
			if (OutId==0) // Color
			{
				if (InputIsConnect(0))
					return ISEVec4(GetInputExpr(0).xyz(),GetInputExpr(2).x());
				else{ // Damier
					//ISEVec4 grey(0.7,0.7,0.7,GetInputExpr(2).x());
					//ISEVec4 white(0.3,0.3,0.3,GetInputExpr(2).x());
					////Purple and black
					///*ISEVec4 grey(0.7,0,0.7,1.0);
					//ISEVec4 white(0,0,0,1.0);*/
					//return If(TextureCoord().y(),ISEFloat(0.5),"<=",
					//	If(TextureCoord().x(),ISEFloat(0.5),"<=",grey,white),
					//	If(TextureCoord().x(),ISEFloat(0.5),"<=",white,grey));
					return ISEVec4(1.0,1.0,1.0,GetInputExpr(2).x());
				}
				
			}else if (OutId==1)// Emissive
			{
				return GetInputExpr(1); 
			}
			else if (OutId==2) // Alpha
			{
				return GetInputExpr(2); 
			}else if (OutId==3) // Normal
			{
				return ISEVec4(/*GetInputExpr(3).xyz()+*/ISEVec3("m_Normal"),0.0); // Normal
			}else if (OutId==4) // Specular
			{
				ISEFloat pow;
				if (InputIsConnect(5))
					pow = GetInputExpr(5).x();
				else
					pow = 50;
				return ISEVec4(GetInputExpr(4).xyz(),ISEFloat(1.0)/pow); 
			}
		}

		void IMaterialExpressionFinal::OnPropertyChange(IString name)
		{
			if (m_pMaterial && m_pMaterial->GetDevice() && m_pMaterial->GetDevice()->GetRessourceManager())
			{
				int Id=-1;
				if (name=="Texture1")
					Id=0;
				else if (name=="Texture2")
					Id=1;
				else if (name=="Texture3")
					Id=2;
				else if (name=="Texture4")
					Id=3;
				else if (name=="Texture5")
					Id=4;
				else if (name=="Texture6")
					Id=5;
				else if (name=="Texture7")
					Id=6;
				else if (name=="Texture8")
					Id=7;

				if (Id>=0)
				{
					IProperty *prop = GetPropertyList().GetProperty(name);
					if (prop)
					{
						IString tex = prop->GetString();
						ITexture *Tex=m_pMaterial->GetDevice()->GetRessourceManager()->LoadTexture(tex);
						printf("%s\n",m_pMaterial->GetFilename().c_str());
						m_pMaterial->SetTexture(Tex,Id);
					}
				}
			}	
			BaseClass::OnPropertyChange(name);
		}

		//-------------------------------------------------------------------------
		// IMaterialExpressionBase
		//-------------------------------------------------------------------------

		IMaterialExpressionBase::IMaterialExpressionBase()
		{
			Preview=false;
			AddProperty("Comment",IPT_STRING,&m_sComment);
			AddProperty("Preview",IPT_BOOL);
			SetPropertyValue("Preview","1");
		}

		//-------------------------------------------------------------------------
		// Texture sampler
		//-------------------------------------------------------------------------
		IMaterialExpressionTextureSampler::IMaterialExpressionTextureSampler()
		{
			Name="Texture sample";
			AddInput("Coord");
			AddOutput("Color");
			AddOutput("Alpha");

			IList<IString> texs;
			texs.PushBack(IString("Texture1"));
			texs.PushBack(IString("Texture2"));
			texs.PushBack(IString("Texture3"));
			texs.PushBack(IString("Texture4"));
			texs.PushBack(IString("Texture5"));
			texs.PushBack(IString("Texture6"));
			texs.PushBack(IString("Texture7"));
			texs.PushBack(IString("Texture8"));
			AddProperty("Texture",IPT_CHOICE,NULL,texs);

			IList<IString> mode;
			mode.PushBack(IString("RGBA"));
			mode.PushBack(IString("Normal map DXT1"));
			mode.PushBack(IString("Normal map DXT5"));
			AddProperty("Mode",IPT_CHOICE,NULL,mode);
		}

		ISEVec4 IMaterialExpressionTextureSampler::GetOutput(int OutId)
		{
			int mode = GetPropertyValue("Mode").x;			
			int texid = GetPropertyValue("Texture").x;
			IString tex;
			tex.Printf("Texture%d",texid);
			ISEVec4 coord = InputIsConnect(0) ? GetInputExpr(0) : TextureCoord();
			if (mode==0)
			{				
				if (OutId==0)
					return TextureSample(tex,coord.xy());
				else
					return TextureSample(tex,coord.xy()).w(); // just alpha
			}else if (mode==1)
			{
				if (OutId==0)
				{
					ISEVec4 color = TextureSample(tex,coord.xy());
					ISEVec3 normal(	color.x()*color.w(),
						color.y(),
						Sqrt(color.x()*color.x()+color.y()*color.y()+1.0));
					ISEVec3 normal2=Normalize(normal*2.0-1.0);
					return ISEVec4((normal2+1.0)*0.5,1.0);
				}else
					return ISEVec4(1.0,0.0,0.0,0.0);				

			}else if (mode==2)
			{

			}

			return ISEVec4();
		}

		void IMaterialExpressionTextureSampler::OnPropertyChange(IString name)
		{
			int texid=GetPropertyValue("Texture").x;
			Name.Printf("Texture sample %d",texid);
			CalculateRender();
			//Refresh();
			BaseClass::OnPropertyChange(name);
		}

		//-------------------------------------------------------------------------
		// Screen buffer
		//-------------------------------------------------------------------------
		IMaterialExpressionScreenBuffer::IMaterialExpressionScreenBuffer()
		{
			Name="Screen buffer";

			AddInput("uv");
			AddOutput("RGB");
			AddOutput("Depth");
		}

		ISEVec4 IMaterialExpressionScreenBuffer::GetOutput(int OutId)
		{
			ISEVec2 coord = InputIsConnect(0) ? GetInputExpr(0).xy() : ScreenCoord().xy()/ISEVec2("ViewportSize");
			if (OutId==0)
				return TextureSample("ScreenColor",coord);
			else
				return TextureSample("ScreenDepth",coord);
		}
		

		//-------------------------------------------------------------------------
		// Texture coord
		//-------------------------------------------------------------------------
		IMaterialExpressionTextureCoord::IMaterialExpressionTextureCoord()
		{
			Name="Texture coord";

			AddOutput("");
		}

		ISEVec4 IMaterialExpressionTextureCoord::GetOutput(int OutId)
		{
			return TextureCoord();
		}

		//-------------------------------------------------------------------------
		// Screen coord
		//-------------------------------------------------------------------------
		IMaterialExpressionScreenCoord::IMaterialExpressionScreenCoord()
		{
			Name="Screen coord";

			AddOutput("");
		}

		ISEVec4 IMaterialExpressionScreenCoord::GetOutput(int OutId)
		{
			return ScreenCoord().xy()/ISEVec2("ViewportSize");
		}
		

		//-------------------------------------------------------------------------
		// Vertex color
		//-------------------------------------------------------------------------
		IMaterialExpressionVertexColor::IMaterialExpressionVertexColor()
		{
			Name="Vertex color";

			AddOutput("");
		}

		ISEVec4 IMaterialExpressionVertexColor::GetOutput(int OutId)
		{
			return ISEVec4("gl_Color");
		}

		//-------------------------------------------------------------------------
		// If
		//-------------------------------------------------------------------------
		IMaterialExpressionIf::IMaterialExpressionIf()
		{
			Name="If";

			AddInput("A");
			AddInput("B");
			AddInput("<");
			AddInput("=");
			AddInput(">");

			AddOutput("");
		}

		ISEVec4 IMaterialExpressionIf::GetOutput(int OutId)
		{
			return If(GetInputExpr(0),GetInputExpr(1),"==",GetInputExpr(3),
				If(GetInputExpr(0),GetInputExpr(1),"<",GetInputExpr(2),GetInputExpr(4)));
		}
		

		//-------------------------------------------------------------------------
		// Time
		//-------------------------------------------------------------------------
		IMaterialExpressionTime::IMaterialExpressionTime()
		{
			Name="Time";
			Categorie="Var";

			AddOutput("");
		}

		ISEVec4 IMaterialExpressionTime::GetOutput(int OutId)
		{
			ISEFloat time("Time");
			return ISEVec4(time,time,time,time);
		}

		//-------------------------------------------------------------------------
		// Constant float
		//-------------------------------------------------------------------------
		IMaterialExpressionConstant::IMaterialExpressionConstant()
		{
			Name="Constant";
			Categorie="Var";

			AddOutput("");

			AddProperty("Auto Comment",IPT_BOOL);
			AddProperty("Value",IPT_FLOAT);
			//SetPropertyValue("Auto Comment","0");

			OnPropertyChange("");
		}

		ISEVec4 IMaterialExpressionConstant::GetOutput(int OutId)
		{
			return GetPropertyValueExpr("Value");
		}

		void IMaterialExpressionConstant::OnPropertyChange(IString name)
		{
			if (GetPropertyValue("Auto Comment").x)
			{
				IColor val = GetPropertyValue("Value");
				wxString val1;
				val1.Printf("%f",val.x);

				m_sComment.Printf("Value %s",TrimZeroFloat(val1.c_str()).c_str());
			}
			BaseClass::OnPropertyChange(name);
		}

		//-------------------------------------------------------------------------
		// Constant Vec2
		//-------------------------------------------------------------------------
		IMaterialExpressionConstantVec2::IMaterialExpressionConstantVec2()
		{
			Name="Constant Vec2";
			Categorie="Var";

			AddOutput("");

			AddProperty("Auto Comment",IPT_BOOL);
			AddProperty("Value",IPT_VEC2);
		}

		ISEVec4 IMaterialExpressionConstantVec2::GetOutput(int OutId)
		{
			return GetPropertyValueExpr("Value");
		}

		void IMaterialExpressionConstantVec2::OnPropertyChange(IString name)
		{
			if (GetPropertyValue("Auto Comment").x)
			{
				IColor val = GetPropertyValue("Value");
				wxString val1,val2;
				val1.Printf("%f",val.x);
				val2.Printf("%f",val.y);

				//val1=TrimZeroFloat(val1.c_str()).c_str();
				m_sComment.Printf("Value %s ; %s",TrimZeroFloat(val1.c_str()).c_str(),TrimZeroFloat(val2.c_str()).c_str());
			}
			BaseClass::OnPropertyChange(name);
		}

		//-------------------------------------------------------------------------
		// Constant Vec3
		//-------------------------------------------------------------------------
		IMaterialExpressionConstantVec3::IMaterialExpressionConstantVec3()
		{
			Name="Constant Vec3";
			Categorie="Var";

			AddOutput("");

			AddProperty("Auto Comment",IPT_BOOL);
			AddProperty("Value",IPT_VEC3);
		}

		ISEVec4 IMaterialExpressionConstantVec3::GetOutput(int OutId)
		{
			return GetPropertyValueExpr("Value");
		}

		void IMaterialExpressionConstantVec3::OnPropertyChange(IString name)
		{
			if (GetPropertyValue("Auto Comment").x)
			{
				IColor val = GetPropertyValue("Value");
				wxString val1,val2,val3;
				val1.Printf("%f",val.x);
				val2.Printf("%f",val.y);
				val3.Printf("%f",val.z);

				//val1=TrimZeroFloat(val1.c_str()).c_str();
				m_sComment.Printf("Value %s ; %s ; %s",TrimZeroFloat(val1.c_str()).c_str(),TrimZeroFloat(val2.c_str()).c_str(),TrimZeroFloat(val3.c_str()).c_str());
			}
			BaseClass::OnPropertyChange(name);
		}

		//-------------------------------------------------------------------------
		// Constant Vec4
		//-------------------------------------------------------------------------
		IMaterialExpressionConstantVec4::IMaterialExpressionConstantVec4()
		{
			Name="Constant Vec4";
			Categorie="Var";

			AddOutput("");

			AddProperty("Auto Comment",IPT_BOOL);
			AddProperty("Value",IPT_COLOR);
		}

		ISEVec4 IMaterialExpressionConstantVec4::GetOutput(int OutId)
		{
			return GetPropertyValueExpr("Value");
		}

		void IMaterialExpressionConstantVec4::OnPropertyChange(IString name)
		{
			if (GetPropertyValue("Auto Comment").x)
			{
				IColor val = GetPropertyValue("Value");
				wxString val1,val2,val3,val4;
				val1.Printf("%f",val.x);
				val2.Printf("%f",val.y);
				val3.Printf("%f",val.z);
				val4.Printf("%f",val.w);

				//val1=TrimZeroFloat(val1.c_str()).c_str();
				m_sComment.Printf("Value %s ; %s ; %s ; %s",TrimZeroFloat(val1.c_str()).c_str(),TrimZeroFloat(val2.c_str()).c_str(),TrimZeroFloat(val3.c_str()).c_str(),TrimZeroFloat(val4.c_str()).c_str());
			}
			BaseClass::OnPropertyChange(name);
		}

		//-------------------------------------------------------------------------
		// Disassembler
		//-------------------------------------------------------------------------
		IMaterialExpressionDisassembler::IMaterialExpressionDisassembler()
		{
			Name="Disassembler";
			Categorie="Var";

			AddInput("");
			AddOutput("");
			AddOutput("");
			AddOutput("");
			AddOutput("");
		}

		ISEVec4 IMaterialExpressionDisassembler::GetOutput(int OutId)
		{
			if (OutId==0)
				return GetInputExpr(0).x();
			else if (OutId==1)
				return GetInputExpr(0).y();
			else if (OutId==2)
				return GetInputExpr(0).z();
			else
				return GetInputExpr(0).w();
		}

		//-------------------------------------------------------------------------
		// Assembler
		//-------------------------------------------------------------------------
		IMaterialExpressionAssembler::IMaterialExpressionAssembler()
		{
			Name="Assembler";
			Categorie="Var";

			AddInput("");
			AddInput("");
			AddInput("");
			AddInput("");

			AddOutput("");
		}

		ISEVec4 IMaterialExpressionAssembler::GetOutput(int OutId)
		{
			return ISEVec4(GetInputExpr(0).x(),GetInputExpr(1).x(),GetInputExpr(2).x(),GetInputExpr(3).x());
		}

		//-------------------------------------------------------------------------
		// Sinus
		//-------------------------------------------------------------------------
		IMaterialExpressionSin::IMaterialExpressionSin()
		{
			Name="Sin";
			Categorie="Math";

			AddInput("Angle");
			AddOutput("");
		}

		ISEVec4 IMaterialExpressionSin::GetOutput(int OutId)
		{
			return Sin(GetInputExpr(0));
		}

		//-------------------------------------------------------------------------
		// Inverse Sinus
		//-------------------------------------------------------------------------
		IMaterialExpressionASin::IMaterialExpressionASin()
		{
			Name="ASin";
			Categorie="Math";

			AddInput("");
			AddOutput("Angle");
		}

		ISEVec4 IMaterialExpressionASin::GetOutput(int OutId)
		{
			return ASin(GetInputExpr(0));
		}

		//-------------------------------------------------------------------------
		// Length
		//-------------------------------------------------------------------------
		IMaterialExpressionLength::IMaterialExpressionLength()
		{
			Name="Length";
			Categorie="Math";

			AddInput("");
			AddOutput("");
		}

		ISEVec4 IMaterialExpressionLength::GetOutput(int OutId)
		{
			return Len(GetInputExpr(0));
		}

		//-------------------------------------------------------------------------
		// Absolute
		//-------------------------------------------------------------------------
		IMaterialExpressionAbs::IMaterialExpressionAbs()
		{
			Name="Abs";
			Categorie="Math";

			AddInput("");
			AddOutput("");
		}

		ISEVec4 IMaterialExpressionAbs::GetOutput(int OutId)
		{
			return Abs(GetInputExpr(0));
		}

		//-------------------------------------------------------------------------
		// Lerp
		//-------------------------------------------------------------------------
		IMaterialExpressionLerp::IMaterialExpressionLerp()
		{
			Name="Lerp";
			Categorie="Math";

			AddInput("a");
			AddInput("b");
			AddInput("Alpha");
			AddOutput("");
		}

		ISEVec4 IMaterialExpressionLerp::GetOutput(int OutId)
		{
			return Lerp(GetInputExpr(0),GetInputExpr(1),GetInputExpr(2).x());
		}

		//-------------------------------------------------------------------------
		// Addition
		//-------------------------------------------------------------------------
		IMaterialExpressionAdd::IMaterialExpressionAdd()
		{
			Name="Add";
			Categorie="Math";

			AddOutput("");
			AddInput("");
			AddInput("");	
		}

		ISEVec4 IMaterialExpressionAdd::GetOutput(int OutId)
		{
			return (InputIsConnect(0) ? GetInputExpr(0) : ISEVec4(0,0,0,0))+(InputIsConnect(1) ? GetInputExpr(1) : ISEVec4(0,0,0,0));
		}

		//-------------------------------------------------------------------------
		// Subtract
		//-------------------------------------------------------------------------
		IMaterialExpressionSub::IMaterialExpressionSub()
		{
			Name="Sub";
			Categorie="Math";

			AddOutput("A-B");
			AddInput("A");
			AddInput("B");
		}

		ISEVec4 IMaterialExpressionSub::GetOutput(int OutId)
		{
			return GetInputExpr(0)-GetInputExpr(1);
		}

		//-------------------------------------------------------------------------
		// Multiplication
		//-------------------------------------------------------------------------
		IMaterialExpressionMult::IMaterialExpressionMult()
		{
			Name="Mult";
			Categorie="Math";

			AddOutput("");
			AddInput("");
			AddInput("");	
		}

		ISEVec4 IMaterialExpressionMult::GetOutput(int OutId)
		{
			return GetInputExpr(0)*GetInputExpr(1);
		}

		//-------------------------------------------------------------------------
		// Divide
		//-------------------------------------------------------------------------
		IMaterialExpressionDiv::IMaterialExpressionDiv()
		{
			Name="Div";
			Categorie="Math";

			AddOutput("A/B");
			AddInput("A");
			AddInput("B");	
		}

		ISEVec4 IMaterialExpressionDiv::GetOutput(int OutId)
		{
			return GetInputExpr(0)/GetInputExpr(1);
		}

		//-------------------------------------------------------------------------
		// Rotator
		//-------------------------------------------------------------------------
		IMaterialExpressionRotator::IMaterialExpressionRotator()
		{
			Name="Rotator";
			Categorie="Coordinator";

			AddOutput("");
			AddInput("uv");
			AddInput("Center");
			AddInput("Angle");
		}

		ISEVec4 IMaterialExpressionRotator::GetOutput(int OutId)
		{
			ISEVec2 Pos = InputIsConnect(0) ? GetInputExpr(0).xy() : TextureCoord().xy();
			ISEVec2 Origin = InputIsConnect(1) ? GetInputExpr(1).xy() : ISEVec2(0.5,0.5);
			ISEFloat angle = InputIsConnect(2) ? GetInputExpr(2).x() : 0.0;
			ISEFloat cos = Cos(angle);
			ISEFloat sin = Sin(angle);
			ISEFloat test = -sin;
			ISEVec2 RowX(cos,-sin);
			ISEVec2 RowY(sin,cos);
			ISEVec2 BaseCoord = Pos;
			ISEFloat Arg1 = Dot(RowX,(BaseCoord-Origin));
			ISEFloat Arg2 = Dot(RowY,(BaseCoord-Origin));
			ISEVec2 Rotator = ISEVec2(Arg1,Arg2)+Origin;
			return Rotator;
		}

		//-------------------------------------------------------------------------
		// Panner
		//-------------------------------------------------------------------------
		IMaterialExpressionPanner::IMaterialExpressionPanner()
		{
			Name="Panner";
			Categorie="Coordinator";

			AddOutput("");
			AddInput("uv");
			AddInput("Acceleration");
		}

		ISEVec4 IMaterialExpressionPanner::GetOutput(int OutId)
		{
			ISEFloat Time("Time");
			ISEVec2 pos =  InputIsConnect(0) ? GetInputExpr(0).xy() : TextureCoord().xy();
			ISEVec2 move = InputIsConnect(1) ? GetInputExpr(1).xy() : ISEVec2(0,0);
			return pos+move*Time;
		}
	}
}
