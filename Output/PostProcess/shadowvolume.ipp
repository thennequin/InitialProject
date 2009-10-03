"shadowvolume"
{
	"Vertex"
	{
		uniform vec3 LightPos;
		void main()
		{
			const float Infiny = 99999999999999999999999999999999999999.0;
			vec4 pos = ftransform();
			gl_Position = pos + (pos - vec4(LightPos,1)) * Infiny;
		}
	}
}