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
struct Light
{
	vec3 direction;
	vec4 color;
	float intensity;
};

layout(std140) uniform LightBlock
{
	Light lights[2];
};

uniform vec4 u_Diffuse;
uniform vec4 u_Ambient;
uniform vec4 u_Specular;
uniform float u_SpecInt;

vec4 ComputeLight(vec3 light_dir, vec4 light_col, vec3 normal, vec3 half_angle,
	vec4 diffuse, vec4 spec, float spec_size, float light_int)
{
	float surface_int = max(dot(normal, -light_dir), 0.0f);
	vec4 lambert = diffuse * light_col * surface_int * light_int;

	float spec_int = pow(max(dot(normal, half_angle), 0.0f), spec_size);
	vec4 phong = spec * light_col * spec_int * surface_int;

	return lambert + phong;
};

void main()
{
	vec4 lighting = vec4(0, 0, 0, 0);
	vec3 pos = v_Position.xyz / v_Position.w;
	for (int i = 0; i < 2; i++)
	{
		vec3 hlf = -normalize(normalize(lights[i].direction) + normalize(pos));
		lighting += ComputeLight(lights[i].direction, lights[i].color, normalize(v_Normal), hlf, 
			u_Diffuse, u_Specular, u_SpecInt, lights[i].intensity);
	}

	fragcolor = vec4(lighting.xyz, 1) + u_Ambient;
};