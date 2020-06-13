#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; 

in vec2 v_TexCoord;

uniform vec2 u_CoordAdd;
uniform float u_Alpha;

uniform sampler2D u_Texture;

void main()
{
	vec4 texWind = texture(u_Texture, v_TexCoord + u_CoordAdd);
	color = texWind * vec4(1.0, 1.0, 1.0, u_Alpha);
};