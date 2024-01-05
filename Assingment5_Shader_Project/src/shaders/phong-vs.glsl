#version 330 core

in vec3 inPosition;
in vec3 inNormal;

out vec3 exNormal;
out vec3 exFragPosition;

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	vec4 MCPosition = vec4(inPosition, 1.0);
	
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;
	exFragPosition = vec3(ModelMatrix * MCPosition);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
