#version 120

uniform sampler2D texture;
uniform vec2 mousepos;
uniform vec2 resolution;
uniform float time;
uniform float factor1;


void main()
{
    // récupère le pixel dans la texture
	vec2 texcoord = gl_TexCoord[0].xy;
	vec2 mousePosNorm = mousepos / resolution; //Mettre la position de la souris entre 0 et 1
	
	//Détermination du vecteur allant du pixel courant à la souris
	vec2 vecteur=mousePosNorm - texcoord;
	float dist = length(vecteur); //La longueur du vecteur est utile
	vecteur = normalize(vecteur); //Normalization nécessaire pour les traitements suivants
   

	//Lecture d'un pixel décalé le long du vecteur par rapport au pixel courant
	float factor = 0.1*cos(dist * 100 - time*5)/ pow((1+dist), 5);	//Atténuation de l'effet avec la distance
	
	float decalage;
	
	
	decalage =  factor * clamp((factor1 - dist)* 5,0,1);	
	
	vec4 pixel = texture2D(texture, texcoord + vecteur * decalage);

	
    gl_FragColor = pixel;//vec4(factor,factor,factor,1);
}