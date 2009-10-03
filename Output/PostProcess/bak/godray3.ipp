"godray"
{
	"Fragment"
	{	
		uniform mat4 InvProjection;
		uniform sampler2D Texture0; // Radial blur
		uniform sampler2D Texture1; // depth
		uniform sampler2D Texture2; // Previous buffer
		
		uniform vec3 LightColor;
		uniform vec3 LightPos;

		void main()
		{
			vec3 ray = texture2D(Texture0,gl_TexCoord[0].xy).xyz;				
			gl_FragData[0] = texture2D(Texture1,gl_TexCoord[0].xy)+vec4(ray*LightColor,1)*0.5;
			//gl_FragData[0] = texture2D(Texture0,gl_TexCoord[0].xy);
		}
	}
}