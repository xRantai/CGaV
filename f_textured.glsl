#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 l;
in vec4 v;
in float d; 


in float i_nl;
in vec2 i_tc;

void main(void) {
    vec4 color=texture(textureMap0,i_tc);
	pixelColor=vec4(color.rgb*i_nl*(1/(0.5*d*d)),color.a);
}