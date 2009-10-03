"edge"
{
	"Fragment"
	{	
		uniform sampler2D Texture0;
		
		uniform int ScreenWidth;
		uniform int ScreenHeight;
		
		void main()
		{
			float offx = 1.0/ScreenWidth;
			float offy = 1.0/ScreenHeight;
			
			float s00 = texture2D(Texture0, gl_TexCoord[0].xy + vec2(-offx, -offy)).x;
			float s01 = texture2D(Texture0, gl_TexCoord[0].xy + vec2( 0,   -offy)).x;
			float s02 = texture2D(Texture0, gl_TexCoord[0].xy + vec2( offx, -offy)).x;
			
			float s10 = texture2D(Texture0, gl_TexCoord[0].xy + vec2(-offx,  0)).x;
			float s12 = texture2D(Texture0, gl_TexCoord[0].xy + vec2( offx,  0)).x;
			
			float s20 = texture2D(Texture0, gl_TexCoord[0].xy + vec2(-offx,  offy)).x;
			float s21 = texture2D(Texture0, gl_TexCoord[0].xy + vec2( 0,    offy)).x;
			float s22 = texture2D(Texture0, gl_TexCoord[0].xy + vec2( offx,  offy)).x;
			
			float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
			float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

			float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
			float max = 0.07;
			float val = (edgeSqr > max * max) ? 1.0 : 0.0;

			float alpha = texture2D(Texture0, gl_TexCoord[0].xy).a;
			val = val * alpha;
			gl_FragData[0] = vec4(val,val,val,1);
			//gl_FragData[0] = texture2D(Texture0, gl_TexCoord[0].xy);
		}
	}
}