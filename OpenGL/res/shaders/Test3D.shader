#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out float v_Light;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

uniform vec3 u_LightDir;
uniform float u_LightInt;


void main()
{
	vec3 norm = normalize(vec3(transpose(inverse(u_ModelView)) * vec4(normal, 1.0f)));
	vec3 dir = normalize(u_LightDir);

	v_Light = -dot(norm, dir) * u_LightInt;
	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in float v_Light;
layout(location = 0) out vec4 color;

uniform vec4 u_Model;
uniform vec4 u_Light;
uniform vec4 u_Ambient;

void main()
{
	color = (u_Model + u_Light) * v_Light + u_Ambient;
};