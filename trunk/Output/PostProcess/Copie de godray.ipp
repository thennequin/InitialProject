"simple"
{
	"Fragment"
	{
		uniform sampler2D Texture0; // scene
		
		varying vec3 lightpos;

		void main()
		{
			gl_FragData[0] = vec4(texture2D(Texture0, gl_TexCoord[0].xy).xyz,1.0);
			gl_FragData[0] = vec4(0,0,0,0);
		}
	}
	
	"Vertex"
	{
		uniform vec4 m_LightPos;
		varying vec3 lightpos;
		
		void main()
		{
			lightpos = (m_LightPos*gl_PorjectionMatrix).xyz;
		}
	}
}