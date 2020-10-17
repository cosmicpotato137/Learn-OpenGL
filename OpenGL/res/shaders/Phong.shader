#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec4 v_Position;
out vec3 v_Normal;
out vec3 v_LightDir;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_LightDir;

void main()
{
	mat4 modelview = u_View * u_Model;
	v_Normal = vec3(transpose(inverse(modelview)) * vec4(normal, 1.0f));
	v_Position = modelview * vec4(position, 1.0f);
	v_LightDir = vec3(u_View * vec4(u_LightDir, 0));

	gl_Position = u_Projection * modelview * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in vec4 v_Position;
in vec3 v_Normal;
in vec3 v_LightDir;
layout(location = 0) out vec4 color;

uniform vec4 u_Diffuse;
uniform vec4 u_Ambient;
uniform vec4 u_Light;
uniform vec4 u_Specular;
uniform float u_SpecInt;

vec4 ComputeLight(vec3 light_dir, vec4 light_col, vec3 normal, vec3 half_angle,
	vec4 diffuse, vec4 spec, float spec_size)
{
	float light_int = max(dot(normal, -light_dir), 0.0f);
	vec4 lambert = diffuse * light_col * light_int;

	float spec_int = pow(max(dot(normal, half_angle), 0.0f), spec_size);
	vec4 phong = spec * light_col * spec_int;

	return lambert + phong;
};

void main()
{
	//vec3 hlf = normalize(normalize(u_LightDir) + normalize(mypos));
	//v_Highlight = pow(max(dot(norm, -hlf), 0.0f), u_HighlightInt);

	vec3 pos = v_Position.xyz / v_Position.w;
	vec3 hlf = -normalize(normalize(v_LightDir) + normalize(pos));
	vec4 light = ComputeLight(v_LightDir, u_Light, normalize(v_Normal), hlf, 
		u_Diffuse, u_Specular, u_SpecInt);

	color = u_Ambient + light;
};