# version 420

in vec2 vertices;

out vec2 lf_coord;

void main(void) {
	gl_Position = vec4(vertices, 0.0, 1.0);
	lf_coord = vertices / 2.0;
}
