"deffered-light2"
{
	"Fragment"
	{
		uniform sampler2D Texture0; // Light
		uniform sampler2D Texture1; // Diffuse
		uniform sampler2D Texture2; // Emissive
		uniform sampler2D Texture3; // Normal
		
		void main()
		{
			vec4 light = texture2D(Texture0,gl_TexCoord[0].xy);
			vec4 color = texture2D(Texture1,gl_TexCoord[0].xy);
			vec4 emissive = texture2D(Texture2,gl_TexCoord[0].xy);
			int LightMode = int(1.0/texture2D(Texture3,gl_TexCoord[0].xy).w);
			
			if (LightMode!=2) // 2: Unlit
				gl_FragData[0] = (light*color + emissive);
			else
				gl_FragData[0] = color+emissive;
				
			//gl_FragData[0] = light;
			
			//Night Vision
			//gl_FragData[0] = ((0.5+light*2.0)*color + emissive)*vec4(0.1,0.7,0.1,1.0)+vec4(0.05,0.35,0.05,0.0);
		}
	}
}
