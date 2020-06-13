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

uniform vec2 u_CoordAdd1;
uniform vec2 u_CoordAdd2;
uniform float u_Alpha;

uniform sampler2D u_TextureSmoke1;
uniform sampler2D u_TextureSmoke2;
uniform sampler2D u_TextureSmokeAlphaMask;

void main()
{
	vec4 texSmoke1 = texture(u_TextureSmoke1, v_TexCoord + u_CoordAdd1);
	vec4 texSmoke2 = texture(u_TextureSmoke2, v_TexCoord + u_CoordAdd2);
	vec4 texSmokeAlphaMask = texture(u_TextureSmokeAlphaMask, v_TexCoord);
	color = ((texSmoke1 + texSmoke2) * texSmokeAlphaMask) * u_Alpha;
};