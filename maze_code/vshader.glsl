#version 120

attribute vec4 vPosition;
attribute vec2 vTexCoord;

varying vec2 texCoord;
varying float f_use_texture;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform int use_texture;

void main()
{
	texCoord = vTexCoord;
	f_use_texture = use_texture;
	gl_Position = model_view_matrix * vPosition;
}
