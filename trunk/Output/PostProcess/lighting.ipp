"lighting"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform int ScreenHeight=480;
		uniform float Power=1.0;
		uniform float Mult=1.0;

		void main()
		{		
			vec4 color = vec4(0,0,0,0);
			float t= Power/float(ScreenHeight);
			color = texture2D(Texture0,gl_TexCoord[0].xy)*(20.0/64.0);
			
			/*color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(0,-3.0*t))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(0,-2.0*t))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(0,-t))*(15.0/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(0,t))*(15.0/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(0,2.0*t))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(0,3.0*t))*(3.5/64.0);*/
			
			gl_FragData[0] = color*Mult;
		}
	}
}