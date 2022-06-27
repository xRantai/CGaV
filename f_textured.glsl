#version 330


uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 texCoord;

void main(void) {
	pixelColor=vec4(1,1,1,1);texture(tex,texCoord);
}
