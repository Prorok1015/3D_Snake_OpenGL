#version 400 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;

out vec4 a_color;
out vec2 a_texCoord;

uniform mat4 model;
uniform mat4 projview;

void main(){
	a_color = vec4(1.0f,1.0f,1.0f,1.0f);
	a_texCoord = v_texCoord;
	gl_Position = projview * model * vec4(v_position, 1.0);
}