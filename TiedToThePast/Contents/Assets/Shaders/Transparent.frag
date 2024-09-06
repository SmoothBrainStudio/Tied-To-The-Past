#version 120

uniform sampler2D texture;
uniform sampler2D mask;
uniform sampler2D player;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	vec4 maskColor = texture2D(mask, texcoord);
	vec4 playerColor = texture2D(player, texcoord);

	if (maskColor.a < 0.05 || playerColor.a < 0.05)
	{
		gl_FragColor = pixel;
	}
	else
	{
		gl_FragColor = vec4(maskColor.xyz * 0.5, 1);
	}
}