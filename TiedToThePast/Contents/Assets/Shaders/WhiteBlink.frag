#version 120

uniform sampler2D texture;
uniform float timer;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	pixel.xyz = pixel.xyz * (timer*2 + 1);

    gl_FragColor = pixel;
}