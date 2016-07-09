#version 150

in vec4 color;
out vec4 out_color;

void main() {
	out_color = vec4(1.0, .8, 1.0, 1.0) * color;
}