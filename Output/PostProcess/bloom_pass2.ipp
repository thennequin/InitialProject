"bloom"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0; // scene
		uniform sampler2D Texture1; // bloom

		void main()
		{
			gl_FragData[0] = texture2D(Texture0, gl_TexCoord[0].xy)+texture2D(Texture1, gl_TexCoord[0].xy);
		}
	}
}