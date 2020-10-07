#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out float v_Light;
out float v_Highlight;
out vec3 v_Normal;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

uniform vec3 u_LightDir;
uniform float u_LightInt;
uniform float u_HighlightInt;


void main()
{
	vec3 norm = normalize( vec3(transpose(inverse(u_ModelView)) * vec4(normal, 1.0f)) );
	v_Normal = norm;
	vec3 dir = normalize(u_LightDir);
	vec3 lightpos = u_LightDir * -1;
	vec4 pos = u_ModelView * vec4(position, 1.0f);
	vec3 mypos = pos.xyz / pos.w;

	// eye posn is 0
	vec3 eye = vec3(0.0f, 0.0f, 0.0f);
	vec3 eyedirn = normalize(eye - mypos);
	vec3 lightdirn = normalize(lightpos - mypos);
	vec3 hlf = normalize(normalize(u_LightDir) + normalize(mypos));

	v_Light = max(-dot(norm, dir), 0.0f) * u_LightInt;
	v_Highlight = pow(max(dot(norm, -hlf), 0.0f), u_HighlightInt);

	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

in float v_Light;
in float v_Highlight;
in vec3 v_Normal;
layout(location = 0) out vec4 color;

uniform vec4 u_Model;
uniform vec4 u_Light;
uniform vec4 u_Ambient;

void main()
{
	vec3 mColor = u_Model.xyz * v_Light;
	vec3 aColor = u_Ambient.xyz;
	vec3 lColor = u_Light.xyz * ((v_Light + v_Highlight) / 2);
	//color = vec4(aColor + lColor + mColor, 1.0f);
	color = vec4(v_Normal, 1.0f);
};