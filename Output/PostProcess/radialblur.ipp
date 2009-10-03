"radialblur"
{
	"Fragment"
	{	
		uniform sampler2D Texture0;
		
		uniform vec2 Center;
		uniform int Samples;
		uniform float Power;
		uniform int Original;
		
		uniform mat4 Projection;
		uniform vec3 LightPos;

		void main()
		{
			vec4 pos = Projection*vec4(LightPos,1.0);
			pos.xyz/=pos.w;
			float mult=1.0;
			if (pos.z<=0.5)
				mult=-1.0;
				
			vec3 Color = vec3(0,0,0);
			vec2 texCoord = gl_TexCoord[0].xy;
			
			vec4 ColorTex = texture2D(Texture0,texCoord);
			if (Original!=0)
				Color += ColorTex.xyz;

			vec2 delta = (texCoord-Center)*mult;
			for (float i=0;i<Samples;i++)
				Color += texture2D(Texture0,Center+delta*(1.0+Power*(float(i)/Samples))).xyz * ((Samples-float(i))/(Samples*Samples/2.0));
				
			gl_FragData[0] = vec4(Color,ColorTex.a);
		}
	}
}