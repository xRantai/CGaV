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


out vec2 i_tc;
out float i_nl1;
out float i_nl2;

void main(void) {

    d1 = distance(cameraPos, M*vertex);
    d2 = distance(skullPos, M*vertex);

    vec4 lightDir1 = normalize(V*cameraPos - V*M*vertex);
    vec4 lightDir2 = normalize(V*skullPos - V*M*vertex);


    gl_Position=P*V*M*vertex;


    mat4 G=mat4(inverse(transpose(mat3(M))));
    vec4 n=normalize(V*G*normal);

    i_nl1=clamp(dot(n,lightDir1),0,1);
    i_nl2=clamp(dot(n,lightDir2),0,1);
    i_tc=texCoord0;
}

