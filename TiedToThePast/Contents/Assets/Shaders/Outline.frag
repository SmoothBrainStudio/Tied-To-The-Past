#define PI 3.14159265359
#define SAMPLES 20
#define WIDTH 0.02

uniform sampler2D texture;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec2 uv = gl_TexCoord[0].xy;

    vec4 finalCol = vec4(0);

    float rads = ((360.0 / float(SAMPLES)) * PI) / 180.0;

    for (int i = 0; i < SAMPLES; i++)
    {
        if (finalCol.w < 0.1)
        {
        	float r = float(i + 1) * rads;
    		vec2 offset = vec2(cos(r), -sin(r)) * WIDTH;
    		finalCol = texture2D(texture, uv + offset);
            if(finalCol.w > 0.0)
            {
                finalCol.xyz = gl_Color;
            }
        }
    }

    vec4 tex = texture2D(texture, uv);

    if(tex.w > 0.0)
    {
     	finalCol = tex;
    }

    gl_FragColor = finalCol;
}