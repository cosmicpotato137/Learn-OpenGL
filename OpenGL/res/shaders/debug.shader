#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragcolor;

// light uniform block
uniform layout(std140, row_major) Light
{
	vec3 direction;
	vec4 color;
	float intensity;
};

void main()
{
	fragcolor = color;
};