"simple"
{
	"Fragment"
	{
		uniform sampler2D Texture0; // scene
		uniform float Mult;

		void main()
		{
			gl_FragData[0] = vec4(texture2D(Texture0, gl_TexCoord[0].xy).xyz,1.0)*Mult;
		}
	}
}