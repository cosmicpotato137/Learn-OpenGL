#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec4 v_Position;
out vec3 v_Normal;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

void main()
{
	v_Normal = vec3(transpose(inverse(u_ModelView)) * vec4(normal, 1.0f));
	v_Position = u_ModelView * vec4(position, 1.0f);

	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in vec4 v_Position;
in vec3 v_Normal;
layout(location = 0) out vec4 fragcolor;

// light uniform block
uniform layout(std140, row_major) Light
{
	vec3 direction;
	vec4 color;
	float intensity;
};

uniform vec4 u_Diffuse;
uniform vec4 u_Ambient;
uniform vec4 u_Specular;
uniform float u_SpecInt;

vec4 ComputeLight(vec3 light_dir, vec4 light_col, vec3 normal, vec3 half_angle,
	vec4 diffuse, vec4 spec, float spec_size)
{
	float surface_int = max(1 / dot(normal, -light_dir), 0);
	vec4 lambert = diffuse * surface_int * spec_size * length(light_dir)/1000;

	//float spec_int = pow(max(dot(normal, half_angle), 0.0f), spec_size);
	//vec4 phong = spec * light_col * spec_int * surface_int;

	return lambert;
};

void main()
{
	//vec3 hlf = normalize(normalize(u_LightDir) + normalize(mypos));
	//v_Highlight = pow(max(dot(norm, -hlf), 0.0f), u_HighlightInt);

	vec3 pos = v_Position.xyz / v_Position.w;
	vec3 hlf = -normalize(normalize(direction) + normalize(pos));
	vec4 light = ComputeLight(pos, color, normalize(v_Normal), hlf,
		u_Diffuse, u_Specular, u_SpecInt);

	fragcolor = light;
};