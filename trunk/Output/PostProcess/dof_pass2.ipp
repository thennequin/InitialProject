"bloom"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform sampler2D Texture1;
		uniform sampler2D Texture2;
		
		uniform float focus = 0.48;
		uniform float range = 0.5;
		uniform float scale = 1.0035;
		uniform int RG = 0;
		
		const float tex = 0.0*1.0/1280.0;

		void main()
		{
			vec4 sharp = texture2D(Texture0, gl_TexCoord[0].xy);
			vec4 blur = texture2D(Texture1, gl_TexCoord[0].xy);
			vec4 depthVec4 = texture2D(Texture2, gl_TexCoord[0].xy);
			float depth = depthVec4.x*scale;
			depth += texture2D(Texture2, gl_TexCoord[0].xy+vec2(0,tex)).x*scale;
			depth += texture2D(Texture2, gl_TexCoord[0].xy+vec2(tex,0)).x*scale;
			depth += texture2D(Texture2, gl_TexCoord[0].xy+vec2(0,-tex)).x*scale;
			depth += texture2D(Texture2, gl_TexCoord[0].xy+vec2(-tex,0)).x*scale;
			depth=depth/5.0;
			
			if (RG!=0)
			{
				blur=vec4(1,0,0,1);
				sharp=vec4(0,1,0,1);
			}
			gl_FragData[0]=mix(sharp, blur, clamp(1.0/range * abs(focus - depth), 0.0, 1.0));
			gl_FragData[0]=sharp;
			//gl_FragData[0]=vec4(depth,depth,depth,1);
		}
	}
}