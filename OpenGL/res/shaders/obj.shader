#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
out vec4 v_Position;
out vec4 v_Normal;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in vec4 v_Position;
in vec4 v_Normal;
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.0f, 0.5f, 0.7f, 1.0f);
};