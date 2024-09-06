#version 120

uniform sampler2D texture;
uniform vec3 color;
uniform float intensity;

void main() 
{
    vec2 texCoord = gl_TexCoord[0].xy;

    // Calculer la distance du pixel au centre du cercle
    float dist = distance(texCoord, vec2(0, 0));

    // Calculer la transparence en fonction de la distance
    float transparency = 1.0 - smoothstep(0.0, intensity, dist);

    // Combinaison de la couleur d'origine avec la transparence
    gl_FragColor = vec4(color / 255.0, transparency);
}