#version 330

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
}

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}

uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;


void main(void) {
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = light.specular * (spec * material.specular);

	fragColor = vec4(vec3(ambient + diffuse + specular), 1.0);

}
