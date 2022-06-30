#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 l;
in vec4 v;
in float d1; 
in float d2;
in vec2 iTexCoord0;


void main(void) {
	
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	vec4 mr = reflect(-ml, mn);

	vec4 kd = texture(textureMap0, iTexCoord0); 

	float nl1 = clamp(dot(mn, ml), 0, 1);
	float nl2 = clamp(dot(mn, mv), 0, 1);


    float rv = pow(clamp(dot(mr, mv), 0, 1),25);


	pixelColor= vec4(clamp(kd.rgb*nl1*(1/(0.5*d2*d2))+kd.rgb*nl2*(1/(0.5*d1*d1)),0,1),kd.a);
}