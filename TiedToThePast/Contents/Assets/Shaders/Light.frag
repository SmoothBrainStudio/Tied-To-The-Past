#version 120

uniform sampler2D texture;
uniform sampler2D textureLumiere;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	vec4 pixelLumiere = texture2D(textureLumiere, texcoord);

    gl_FragColor = pixel * pixelLumiere * 1.2;
}