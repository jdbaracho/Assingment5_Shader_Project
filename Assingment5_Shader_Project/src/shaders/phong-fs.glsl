#version 330 core

in vec3 exNormal;
in vec3 exFragPosition;

out vec4 FragmentColor;

uniform vec3 Color;
uniform vec3 LightPosition;
uniform vec3 EyePosition;

void main(void)
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	// ambient component
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse component
	vec3 norm = normalize(exNormal);
	vec3 lightDirection = normalize(LightPosition - exFragPosition);
	float diffuseImpact = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diffuseImpact * lightColor;

	// specular component
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(EyePosition - exFragPosition);
	// phong
	// vec3 reflectDirection = reflect(-lightDirection, norm);
	// float specularImpact = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	// blinn-phong
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularImpact = pow(max(dot(norm, halfwayDirection), 0.0), 32);
	vec3 specular = specularStrength * specularImpact * lightColor;

	// vec3 result = ambient * Color;
	// vec3 result = diffuse * Color;
	// vec3 result = specular * Color;
	vec3 result = (ambient + diffuse + specular) * Color;
	FragmentColor = vec4(result, 1.0f);
}
