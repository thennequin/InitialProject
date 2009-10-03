"radialblur"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform float Power;
		uniform int Samples;
		uniform vec2 Center;

		void main()
		{		
			vec4 color = vec4(0,0,0,0);
			
			vec2 delta = gl_TexCoord[0].xy - Center;
			
			for (float i=0;i<Samples;i++)
			{
				color += texture2D(Texture0,Center+delta*(1+Power*(i/Samples)))*(i)/Samples;
			}
			
			gl_FragData[0] = color;
		}
	}
}