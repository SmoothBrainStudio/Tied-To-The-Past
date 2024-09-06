uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 texturePos = gl_TexCoord[0].xy;

    float periods = 10.0;
    float amplitude = 0.005;
    float speed = 0.25;
    texturePos.x += sin((texturePos.y + time * speed) * periods) * amplitude * 2;

    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, texturePos);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel;
}