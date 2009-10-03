"bloom"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform int ScreenWidth=1280;
		uniform float Power=1.0;
		uniform float Mult=1.0;

		void main()
		{		
			vec4 color = vec4(0,0,0,0);
			float t= Power/float(ScreenWidth);
			color = texture2D(Texture0,gl_TexCoord[0].xy)*(20.0/64.0);
			
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(-3.0*t,0))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(-2.0*t,0))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(-t,0))*(15.0/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(t,0))*(15.0/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(2.0*t,0))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(3.0*t,0))*(3.5/64.0);

			//color.a*=Mult;
			color*=Mult;
			gl_FragData[0] = color;
		}
	}
}