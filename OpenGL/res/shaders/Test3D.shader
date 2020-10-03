#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 v_Color;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

void main()
{
	v_Color = color;
	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in vec3 v_Color;
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(v_Color, 1.0f);
};