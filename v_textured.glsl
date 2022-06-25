#version 330

//Atrybuty
in vec3 aPos; //wspolrzedne wierzcholka w przestrzeni modelu
in vec3 aNormal;
in vec2 aTexCoord; //wspó³rzêdne teksturowania

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Zmienne interpolowane
out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main(void) {
    fragPos = vec3(M * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(M))) * aNormal;

    gl_Position = P * V * vec4(fragPos, 1.0);
    texCoord = aTexCoord;
}
