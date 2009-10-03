"deffered-light"
{	
	"Fragment"
	{
		uniform mat4 InvProjection;
		uniform sampler2D Texture0; // Normal
		uniform sampler2D Texture1; // Specular	
		uniform sampler2D Texture2; // Depth
		uniform sampler2D Texture3; // Buffer
		//uniform sampler2D Texture4; // ShadowMap
		uniform sampler2DShadow Texture4; // ShadowMap
		uniform sampler2D Texture5; // ShadowMap Color
		uniform sampler2D Texture6; // Projected Texture
			
		uniform vec3 LightPos;
		uniform float LightRadius;
		uniform vec3 LightColor;
		
		uniform mat4 HalfLightProjection;
		uniform mat4 LightProjection;
		
		uniform int ShadowEnable;
		
		varying vec3 CamPos;
		
		float CellShading(float val)
		{
			if (val<0.3)
				val=0.0;
			else if (val>=0.3 && val <0.6)
				val=0.3;
			else if (val>=0.6 && val <0.8)
				val=0.6;
			else
				val=1.0;
			return val;
		}
	
		vec3 Lighting(vec4 SpecColor, vec4 NNormal, vec3 V)
		{		
			int LightMode = int(1.0/NNormal.w);
				
			if (LightMode!=2) // 2: Unlit
			{
				vec3 Light=vec3(0.0,0.0,0.0);
				vec3 Spec=vec3(0.0,0.0,0.0);
				vec3 fvLightDirection = LightPos - V;
				float att = clamp(1.0-abs(length(fvLightDirection)/LightRadius),0.0,1.0);
				
				if (att>0.0)
				{
					vec3 fvLightDirectionN = normalize(fvLightDirection);
					
					vec3 fvNormal = normalize( NNormal.xyz * 2.0 - 1.0 );
					float fNDotL;
					if (LightMode==3)
						fNDotL = 1.0;
					else
						fNDotL = clamp( dot( fvNormal, fvLightDirectionN ),0.0,1.0); 
					
					if (fNDotL>0.0)
					{	
						//ShadowMap
						vec3 Shadow=vec3(0.0,0.0,0.0);
						if (ShadowEnable==1)
						{
							vec4 pos = HalfLightProjection*vec4(V,1);
							if (pos.q>0.1)
							{
								/*float tex=1.0/512;
								int ITE=4;
								float fShadow=0.0;
								for (int x=-ITE;x<=ITE;x++)
								{
									for (int y=-ITE;y<=ITE;y++)
									{
										float zShadow = texture2DProj(Texture4,pos+vec4(x*tex,y*tex,0.0,0.0)).x *2.0 - 1.0;
										vec4 posShadow = LightProjection*vec4(V,1.0);
										float val = posShadow.z/posShadow.w-zShadow;
										float b1 = 0.0000025;			
										float b2 = 0.000005;
										fShadow+=clamp(1.0-((val-b1))/(b2-b1),0.0,1.0);
									}
								}
								fShadow/=((ITE*2+1)*(ITE*2+1));*/
								
								/*float zShadow = texture2DProj(Texture4,pos).x *2.0 - 1.0;
								vec4 posShadow = LightProjection*vec4(V,1.0);
								float val = posShadow.z/posShadow.w-zShadow;
								float b1 = 0.0000025;			
								float b2 = 0.000005;
								float fShadow=clamp(1.0-((val-b1))/(b2-b1),0.0,1.0);*/
								
								float fShadow = shadow2DProj(Texture4,pos+vec4(0,0,-0.00005,0)).x;
								
								/*float tex=1.0/512;
								int ITE=1;
								float fShadow=0.0;
								for (int x=-ITE;x<=ITE;x++)
								{
									for (int y=-ITE;y<=ITE;y++)
									{
										fShadow+=shadow2DProj(Texture4,pos+vec4(x*tex,y*tex,-0.00005,0)).x;
									}
								}
								fShadow/=((ITE*2+1)*(ITE*2+1));*/
								
								//Shadow = fShadow*texture2DProj(Texture5,pos).xyz;
								Shadow = fShadow*vec3(1,1,1);
								//Shadow = vec3(1,1,1);
							}
						}else{
							Shadow=vec3(1.0,1.0,1.0);
						}					
					
						vec3 fvViewDir = normalize(CamPos-V);
						float Power = 50;
						Power = 1.0/SpecColor.w;
						float SpecPow = pow( clamp( dot(reflect( -fvViewDir, fvNormal ), fvLightDirectionN ) ,0.0, 1.0 ), Power ) * att;			
						float LightPow = fNDotL * att;
						
						//CellShading
						if (LightMode==4)
						{
							SpecPow = CellShading(SpecPow);
							LightPow = CellShading(LightPow);
						}
						
						vec3 LightColorTemp = LightColor;
						//LightColorTemp *= texture2DProj(Texture6,pos).xyz;
						Spec = LightColorTemp * SpecColor.xyz * SpecPow;
						Light = LightColorTemp * LightPow;
						
						//return Shadow.x*vec3(1,1,1)+(1.0-Shadow.x)*vec3(1,0,0);
						return Shadow*(Light + Spec);
					}
				}
				return vec3(0.0,0.0,0.0);
			}else{
				return vec3(1.0,1.0,1.0);
			}
		}

		void main()
		{
			float x = gl_TexCoord[0].x * 2 - 1;
			float y = (/*1 -*/ gl_TexCoord[0].y) * 2 - 1;
			float z = texture2D(Texture2,gl_TexCoord[0].xy).x * 2 - 1;			
			vec4 vProjectedPos = vec4(x, y, z, 1.0f);
			vec4 vPositionVS = InvProjection*vProjectedPos;
			vec3 V = vPositionVS.xyz / vPositionVS.w;

			vec4 normal = texture2D(Texture0,gl_TexCoord[0].xy);
			vec4 spec = texture2D(Texture1,gl_TexCoord[0].xy);
			vec3 Buffer = texture2D(Texture3,gl_TexCoord[0].xy).xyz;
			vec3 final = Lighting(spec,normal,V);
			gl_FragData[0] = vec4(Buffer+final,1);
			//gl_FragData[0] = spec;
			//gl_FragData[0] =  texture2D(Texture1,gl_TexCoord[0].xy);
			//gl_FragData[0] = vec4(1,1,1,1);
		}
	}
	
	"Vertex"
	{
		uniform mat4 InvProjection;
		varying vec3 CamPos;
		void main()
		{		
			vec4 CamPosT=(InvProjection*vec4(0,0,0,1));
			CamPos = CamPosT.xyz/CamPosT.w;
			
			gl_TexCoord[0]=gl_MultiTexCoord0;
			gl_Position = ftransform();
		}
	}
}