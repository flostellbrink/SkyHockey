#version 330 core

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(gl_FragCoord.z, gl_FragCoord.z * gl_FragCoord.z, 0, 0);
}
