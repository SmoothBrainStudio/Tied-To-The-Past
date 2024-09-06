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
	vec2 vecteur = mousePosNorm - texcoord;
	float dist = length(vecteur); //La longueur du vecteur est utile
	vecteur = normalize(vecteur); //Normalization nécessaire pour les traitements suivants
   

	//Lecture d'un pixel décalé le long du vecteur par rapport au pixel courant
	float factor = 0.1*cos(dist * 100 - time*5);	//Atténuation de l'effet avec la distance
	
	float decalage;
	
	decalage =pow(clamp(1-abs(factor1-dist),0,1),8)*  (factor) ;	
	
	vec4 pixel = texture2D(texture, texcoord + vecteur * decalage);

	
    gl_FragColor = pixel;//vec4(decalage,decalage,decalage,1);
}