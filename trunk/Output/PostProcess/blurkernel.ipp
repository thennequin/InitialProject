"bloom"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform int ScreenWidth=640;
		uniform float Power=1.0;
		uniform float Mult=1.0;
		uniform int Samples=8;

		void main()
		{		
			vec2 samples00 = vec2(-0.326212, -0.405805);
			vec2 samples01 = vec2(-0.840144, -0.073580);
			vec2 samples02 = vec2(-0.695914,  0.457137);
			vec2 samples03 = vec2(-0.203345,  0.620716);
			vec2 samples04 = vec2( 0.962340, -0.194983);
			vec2 samples05 = vec2( 0.473434, -0.480026);
			vec2 samples06 = vec2( 0.519456,  0.767022);
			vec2 samples07 = vec2( 0.185461, -0.893124);
			vec2 samples08 = vec2( 0.507431,  0.064425);
			vec2 samples09 = vec2( 0.896420,  0.412458);
			vec2 samples10 = vec2(-0.321940, -0.932615);
			vec2 samples11 = vec2(-0.791559, -0.597705);
			vec4 color = vec4(0,0,0,0);
			float t= Power/float(ScreenWidth);
			color = texture2D(Texture0,gl_TexCoord[0].xy)*(20.0/64.0);
			
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(-3.0*t,0))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(-2.0*t,0))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(-t,0))*(15.0/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(t,0))*(15.0/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(2.0*t,0))*(3.5/64.0);
			color += texture2D(Texture0,gl_TexCoord[0].xy+vec2(3.0*t,0))*(3.5/64.0);

			gl_FragData[0] = color*Mult;
		}
	}
}