# version 420

in vec2 vertices;
in vec2 coord;

out vec2 lf_coord;

void main(void) {
	gl_Position = vec4(vertices, 0.0, 1.0);
	lf_coord = coord;
}
