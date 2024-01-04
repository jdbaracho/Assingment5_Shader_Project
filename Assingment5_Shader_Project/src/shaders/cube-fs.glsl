#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec3 exFragPosition;
in vec3 exLightPosition;

out vec4 FragmentColor;

uniform vec3 Color;

void main(void)
{
/**/
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 n = normalize(exNormal);
	vec3 lightDirection = normalize(exLightPosition - exFragPosition);
	float diffuseImpact = max(dot(n, lightDirection), 0.0);
	vec3 diffuse = diffuseImpact * lightColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(-exFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, n);
	float specularImpact = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
	vec3 specular = specularStrength * specularImpact * lightColor;

	vec3 result = (ambient + diffuse + specular) * Color;
	FragmentColor = vec4(result, 1.0f);
/** /
  // vec3 color = vec3(1.0);
  // vec3 color = (exPosition + vec3(1.0)) * 0.5;
  // vec3 color = vec3(exTexcoord, 0.0);
	vec3 color = (exNormal + vec3(1.0)) * 0.5;
	FragmentColor = vec4(color,1.0);
/** /
	vec3 N = normalize(exNormal);
	vec3 direction = vec3(1.0, 0.5, 0.25);
	float intensity = max(dot(direction, N), 0.0);
	FragmentColor = vec4(vec3(intensity), 1.0);
/**/
}
