#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec3 exFragPosition;
out vec3 exLightPosition;

uniform mat4 ModelMatrix;
uniform vec3 LightPosition;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;

	vec4 MCPosition = vec4(inPosition, 1.0);
	exFragPosition = vec3(ViewMatrix * ModelMatrix * MCPosition);
	exLightPosition = vec3(ViewMatrix * vec4(LightPosition, 1.0));
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
