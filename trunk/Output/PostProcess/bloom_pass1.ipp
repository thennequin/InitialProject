"bloom"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		
		uniform int ScreenWidth=1280;
		uniform int ScreenHeight=1024;

		void main()
		{
			vec4 color=vec4(0,0,0,0);
			vec2 texScale = 1.0/vec2(ScreenWidth,ScreenHeight);
			
			/*vec2 gaussFilter[5];			
			gaussFilter[0] = vec2(-1.0, 0.0);
			gaussFilter[1] = vec2( 1.0, 0.0);
			gaussFilter[2] = vec2( 0.0, 0.0);
			gaussFilter[3] = vec2( 0.0,-1.0);
			gaussFilter[4] = vec2( 0.0, 1.0);

			for (int i=0;i<7;i++)
			{				
			  color += texture2D(Texture0,gl_TexCoord[0].xy+gaussFilter[i]*texScale);
			}
			color*=0.20;*/
			
			color = texture2D(Texture0, gl_TexCoord[0].xy);			
			color.a = 1.0;
			gl_FragData[0] = (color-vec4(1.0,1.0,1.0,0.0));
		}
	}
}