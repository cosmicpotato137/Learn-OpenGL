#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec4 v_Position;
out vec4 v_Normal;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
    v_Position = u_ModelView * vec4(position, 1.0f);
    v_Normal = transpose(inverse(u_ModelView)) * vec4(normal, 1.0f);
};

#shader fragment
#version 330 core

in vec4 v_Position;
in vec4 v_Normal;
layout(location = 0) out vec4 color;

// color and position
uniform vec3 light0dirn;
uniform vec4 light0color;
uniform vec4 light1posn;
uniform vec4 light1color;

// material params
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

vec4 ComputeLight(const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec4 mydiffuse) 
{
    float nDotL = dot(normal, direction);
    vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

    return lambert;
}

void main()
{
    // They eye is always at (0,0,0) looking down -z axis 
    // Also compute current fragment position and direction to eye 

    const vec3 eyepos = vec3(0, 0, 0);
    vec3 mypos = v_Position.xyz / v_Position.w; // Dehomogenize current location 
    vec3 eyedirn = normalize(eyepos - mypos); // Direction to eye from vertex

    // Compute normal, needed for shading. 
    vec3 normal = normalize(v_Normal.xyz);

    // Light 0, directional
    vec3 direction0 = normalize(light0dirn);
    vec3 half0 = normalize(direction0 + eyedirn);
    vec4 col0 = ComputeLight(direction0, light0color, normal, diffuse);

    // Light 1, point 
    vec3 position = light1posn.xyz / light1posn.w;
    vec3 direction1 = normalize(position - mypos); // no attenuation 
    vec3 half1 = normalize(direction1 + eyedirn);
    vec4 col1 = ComputeLight(direction1, light1color, normal, diffuse);

    color = ambient + col0 + col1;
};