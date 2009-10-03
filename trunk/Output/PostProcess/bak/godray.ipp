"godray"
{
	"Fragment"
	{
		uniform mat4 Projection;
		uniform sampler2D Texture0; // Depth
		uniform sampler2D Texture1; // Normal
		uniform vec3 ScreenLightPos;
		uniform vec3 LightPos;
		uniform float LightRadius;
		
		void main()
		{
			vec2 texCoord = gl_TexCoord[0].xy;
			
			vec4 color = texture2D(Texture0,texCoord);
			vec3 normal = texture2D(Texture1,texCoord).xyz;
			float z = (Projection*vec4(ScreenLightPos,1)).z;
			
			vec3 dist = LightPos-V;
			
			if (color.z<z)
				gl_FragData[0]=vec4(1,1,1,val);
			else
				gl_FragData[0]=vec4(0,0,0,val);
		}
	}
}