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
			//pos=pos*0.5+0.5;
			
			float mult=1.0;
			//if (pos.z<=0.5)
			//	mult=-1.0;
				
			float Color = 0.0;
			vec2 texCoord = gl_TexCoord[0].xy;
			
			vec4 ColorTex = texture2D(Texture0,texCoord);
			if (Original!=0)
				Color += ColorTex.g;
				
			vec2 Centers=pos.xy;
			//Centers=Center;
			float Powers=4*Power*(1.0/pos.z);
			Powers=Power;

			vec2 delta = (texCoord-Centers)*mult;
			for (float i=0;i<Samples;i++)
				Color += texture2D(Texture0,Centers+delta*(1.0+Powers*(i/Samples))).g * ((Samples-i)/(Samples*Samples/2.0));
				
			gl_FragData[0] = vec4(ColorTex.r,Color,0.0,0.0);
		}
	}
}