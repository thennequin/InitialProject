"godray"
{
	"Fragment"
	{
		uniform mat4 Projection;
		uniform sampler2D Texture0; // Depth
		uniform vec3 ScreenLightPos;
		uniform vec3 LightPos;
		
		void main()
		{
			vec2 texCoord = gl_TexCoord[0].xy;
			
			vec4 depth = texture2D(Texture0,texCoord);
			vec4 pos = Projection*vec4(LightPos,1.0);
			pos.xyz/=pos.w;
			
			if (depth.z-pos.z<=0.0 /*&& pos.z>0.0*/)
				gl_FragData[0]=vec4(1,1,1,1);
			else
				gl_FragData[0]=vec4(0,0,0,1);
				
			if (depth.z!=1.0)
				gl_FragData[0]=vec4(depth.x,1.0,0.0,0.0);
			else
				gl_FragData[0]=vec4(0.0,0.0,0.0,0.0);
		}
	}
}