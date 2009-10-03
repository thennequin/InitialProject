"godray"
{
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform sampler2D Texture1;
		uniform vec3 LightColor;
		
		void main()
		{
			vec2 texCoord = gl_TexCoord[0].xy;
			gl_FragData[0] = texture2D(Texture1, gl_TexCoord[0].xy) + texture2D(Texture0, texCoord) * vec4(LightColor,1);
			//gl_FragData[0] = texture2D(Texture0, gl_TexCoord[0].xy);
		}
	}
}