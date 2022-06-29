#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 l;
in vec4 v;
in float d1; 
in float d2; 


in float i_nl1;
in float i_nl2;
in vec2 i_tc;

void main(void) {
    vec4 color=texture(textureMap0,i_tc);

	pixelColor=vec4(clamp(color.rgb*i_nl1*(1/(0.5*d1*d1))+color.rgb*i_nl2*(1/(0.5*d2*d2)),0,1),color.a);
}