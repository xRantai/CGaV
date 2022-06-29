#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 cameraPos;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;


//Zmienne interpolowane
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
out float d;


out vec2 i_tc;
out float i_nl;

void main(void) {

    d = distance(cameraPos, M*vertex);

    //vec4 lightDir = normalize(V*vec4(7.0f, 1.2f, 2.0f,1.0f) - V*M*vertex); //wersja ze nieruchomym swiatlem

    vec4 lightDir = normalize(V*cameraPos - V*M*vertex); // wersja z ruchomym œwiat³em
    gl_Position=P*V*M*vertex;


    mat4 G=mat4(inverse(transpose(mat3(M))));
    vec4 n=normalize(V*G*normal);

    i_nl=clamp(dot(n,lightDir),0,1);
    i_tc=texCoord0;
}

