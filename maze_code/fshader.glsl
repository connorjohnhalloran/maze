#version 120

varying vec2 texCoord;
varying float f_use_texture;

uniform sampler2D texture;

void main()
{
	vec4 final_color = vec4(0.4f, 0.75f, 0.8f, 1.0f);

	if (f_use_texture == 1.0f) {
		final_color = texture2D(texture, texCoord);
	}
	gl_FragColor = final_color;
}
