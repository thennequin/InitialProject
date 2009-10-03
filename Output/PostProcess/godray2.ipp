"godray"
{
	"Fragment"
	{
		uniform sampler2D Texture0; // Godray
		uniform sampler2D Texture1; // Defered light
		//uniform sampler2D Texture2; // Depth
		uniform vec3 LightColor;
		
		void main()
		{
			vec2 texCoord = gl_TexCoord[0].xy;
			gl_FragData[0] = texture2D(Texture1, gl_TexCoord[0].xy)/*-vec4(LightColor,0.0)*0.5*/ + clamp(1.0-texture2D(Texture0, texCoord).g,0.0,1.0)*vec4(LightColor,1.0)*0.2;
			//gl_FragData[0] = texture2D(Texture0, gl_TexCoord[0].xy);
		}
	}
}