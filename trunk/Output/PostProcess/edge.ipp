"edge"
{
	"Fragment"
	{	
		uniform sampler2D Texture0;
		
		uniform int ScreenWidth;
		uniform int ScreenHeight;
		
		void main()
		{
			float offx = 1.0/float(ScreenWidth);
			float offy = 1.0/float(ScreenHeight);
			
			float s02 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(-2*offx, 0)).x;
			float s11 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(-offx, offy)).x;
			float s12 = -2*texture2D(Texture0, gl_TexCoord[0].xy + vec2(-offx, 0)).x;
			float s13 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(-offx, -offy)).x;
			float s20 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(0, 2*offy)).x;
			float s21 = -2*texture2D(Texture0, gl_TexCoord[0].xy + vec2(0, offy)).x;
			float s22 = 16*texture2D(Texture0, gl_TexCoord[0].xy + vec2(0, 0)).x;
			float s23 = -2*texture2D(Texture0, gl_TexCoord[0].xy + vec2(0, -offy)).x;
			float s24 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(0, -2*offy)).x;
			float s31 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(offx, offy)).x;
			float s32 = -2*texture2D(Texture0, gl_TexCoord[0].xy + vec2(offx, 0)).x;
			float s33 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(offx, -offy)).x;
			float s42 = -texture2D(Texture0, gl_TexCoord[0].xy + vec2(2*offx, -offy)).x;
			
			float val = s02+s11+s12+s13+s20+s21+s22+s23+s24+s31+s32+s33+s42;
			if (abs(gl_TexCoord[0].x-gl_TexCoord[0].y)<0.004) // On efface la ligne au milieu de l'écran
				val=0;
			
			float alpha = texture2D(Texture0, gl_TexCoord[0].xy).a;
			val = val * alpha;
			gl_FragData[0] = vec4(val,val,val,1);
		}
	}
}