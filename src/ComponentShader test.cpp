

#include "Core/IShaderExpression.h"

using namespace Initial;
using namespace Initial::ShaderExpression;

void Rotator()
{
	//Rotator
	ISEVec2 Pos("Pos");
	Pos = TextureCoord().xy();
	ISEVec2 Origin("Origin");
	ISEFloat angle("angle");
	ISEFloat cos = Cos(angle);
	ISEFloat sin = Sin(angle);
	ISEFloat test = -sin;
	ISEVec2 RowX(cos,-sin);
	ISEVec2 RowY(sin,cos);
	ISEVec2 BaseCoord = Pos;
	ISEFloat Arg1 = Dot(RowX,(BaseCoord-Origin));
	ISEFloat Arg2 = Dot(RowY,(BaseCoord-Origin));
	ISEVec2 Rotator=ISEVec2(Arg1,Arg2)+Origin;
	printf("Rotator = %s\n",Rotator.GetGLSLString().c_str());

}

void Bump()
{
	//Bump mapping
	ISEVec3 Normal("Normal");
	ISEVec3 Color("Color");
	ISEFloat Time("Time");
	ISEVec3 fvLightDirection = Normalize(ISEVec3(Sin(Time),Cos(Time)*2,1.5)); // Lumiére randomiser avec un sin (temporaire)
	ISEVec3 fvNormal = Normalize(Normal*2-1);
	ISEFloat fNDotL = Dot(fvNormal,fvLightDirection);
	ISEVec3 Bump=fNDotL*Color;
	printf("\nBump mapping = %s\n",Bump.GetGLSLString().c_str());
}

void Parralax()
{
	//Parallax mapping
	ISEFloat height("height");
	ISEFloat offmul("offmul");
	ISEFloat offsub("offsub");
	ISEVec2 pos("pos");
	ISEVec3 v_ViewDir("v_ViewDir");
	ISEVec3 ViewT = Normalize(v_ViewDir);
	ISEVec2 Offset = ISEVec2(height,height);
	Offset = Offset * offmul - offsub;
	ISEVec2 Parralax = Offset*ViewT.xy()+pos;
	printf("\nParallax mapping = %s\n",Parralax.GetGLSLString());
}

void TextureSampler()
{
	//Texure sampler
	ISEVec4 coord = TextureCoord()+ISEVec4(ISEVec2(0.5,0.5),0,0);
	ISEVec4 tex = TextureSample("Texture0",coord.xy());
	printf("\nCoord = %s\n",coord.GetGLSLString().c_str());
	printf("\nTexture = %s\n",tex.GetGLSLString().c_str());
}

void If()
{
	ISEVec2 A("A");
	ISEVec2 B("B");
	ISEVec3 Sup("Sup");
	ISEVec3 Inf("Inf");
	ISEVec3 Equal("Equal");

	If(A,B,"==",Equal,If(A,B,"<",Inf,Sup));
}

void Desat()
{
	ISEVec3 color("color");
	ISEFloat saturation("saturation");

	ISEFloat grey = (color.x()+color.y()+color.z())/3;
	ISEFloat red = grey*(-saturation+1)+color.x()*saturation;
	ISEFloat green = grey*(-saturation+1)+color.y()*saturation;
	ISEFloat blue = grey*(-saturation+1)+color.z()*saturation;
	ISEVec3 Desat(red,green,blue);
}

void Specular()
{
	ISEVec3 v_ViewDir("v_ViewDir");
	ISEFloat Time("Time");

	ISEVec3 SpecColor("SpecColor");
	ISEVec3 Normal("Normal");
	ISEFloat intensity("Intensity");

	ISEVec3 ViewT = Normalize(v_ViewDir);
	ISEVec3 fvLightDirection = Normalize( ISEVec3(Sin(Time),Cos(Time)*2,1.5) );
	ISEVec3 fvNormal = Normalize( Normal*2-1);
	ISEFloat fNDotL = Dot(fvNormal, fvLightDirection );
	ISEVec3 fvReflection = Normalize(((fvNormal*2)*fNDotL)-fvLightDirection);
	ISEVec3  fvViewDirection = Normalize( ViewT );
	ISEFloat fRDotV = Max( 0.0, Dot( fvReflection, fvViewDirection ) );
	ISEVec3 Spec = SpecColor * ( Pow( fRDotV, 10 ) )*intensity;
}

void test()
{
	ISEMat3x3 mat;
	ISEVec3 vec3 = mat[2];
	ISEFloat fl1 = mat[2].x();
	ISEFloat fl2 = mat[2][0];
	printf("Shader expression test\n\n");
}