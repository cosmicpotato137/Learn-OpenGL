#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec4 v_Light;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

uniform vec4 u_Diffuse;
uniform vec4 u_Ambient;
uniform vec3 u_LightDir;
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
	vec4 pos = u_ModelView * vec4(position, 1.0f);
	vec3 hpos = pos.xyz / pos.w;
	vec3 hlf = -normalize(normalize(u_LightDir) + normalize(hpos));
	vec4 light = ComputeLight(u_LightDir, u_Light, normalize(normal), hlf,
		u_Diffuse, u_Specular, u_SpecInt);

	v_Light = u_Ambient + light;

	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in vec4 v_Light;
layout(location = 0) out vec4 color;

void main()
{
	color = v_Light;
};