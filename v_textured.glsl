#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 cameraPos;
uniform vec4 skullPos;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;


//Zmienne interpolowane
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
out float d1;
out float d2;



void main(void) {

    d1 = distance(cameraPos, M*vertex);
    d2 = distance(skullPos, M*vertex);

    v = normalize(V*cameraPos - V*M*vertex);
    l = normalize(V*skullPos - V*M*vertex);
    n = normalize(V*M*normal);

    vec4 n=normalize(V*M*normal);

    iTexCoord0=texCoord0;

    gl_Position=P*V*M*vertex;
}

