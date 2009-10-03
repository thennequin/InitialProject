"bloom"
{
	"Translucent" "1"
	"Fragment"
	{
		uniform sampler2D Texture0;
		uniform sampler2D Texture1;
		//uniform float sampleDist0;

		void main()
		{
			float sampleDist0 =0.001;
			vec2 samples00 = vec2(-0.326212, -0.405805);
			vec2 samples01 = vec2(-0.840144, -0.073580);
			vec2 samples02 = vec2(-0.695914,  0.457137);
			vec2 samples03 = vec2(-0.203345,  0.620716);
			vec2 samples04 = vec2( 0.962340, -0.194983);
			vec2 samples05 = vec2( 0.473434, -0.480026);
			vec2 samples06 = vec2( 0.519456,  0.767022);
			vec2 samples07 = vec2( 0.185461, -0.893124);
			vec2 samples08 = vec2( 0.507431,  0.064425);
			vec2 samples09 = vec2( 0.896420,  0.412458);
			vec2 samples10 = vec2(-0.321940, -0.932615);
			vec2 samples11 = vec2(-0.791559, -0.597705);

			vec2 newCoord;
			vec4 sum = texture2D(Texture0,  gl_TexCoord[0].xy);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples00;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples01;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples02;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples03;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples04;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples05;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples06;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples07;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples08;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples09;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples10;
			sum += texture2D(Texture0, newCoord);

			newCoord =  gl_TexCoord[0].xy + sampleDist0 * samples11;
			sum += texture2D(Texture0, newCoord);

			sum /= 13.0;

			//gl_FragColor = sum;
			//gl_FragColor = /*vec4(0.5,0.5,0.5,1)*/+sum;
			gl_FragData[0] = /*vec4(0.5,0.5,0.5,1)*/+sum;
			gl_FragData[0] = texture2D(Texture0, gl_TexCoord[0].xy);
		}
	}
}