# version 150

in vec2 lf_coord;
out vec4 out_color;

uniform sampler2D lf;

/* uniform float t; */
/* uniform float D; */
/* uniform float offset; */


void main(void) {

	int size = 3280;
	float t = 0;
	float D = 9.97;
	vec2 offset = vec2(-3.3, -5.2);
	float angle = -0.0048796734772622585;
	int lim = 4;

	mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
	vec2 coord_rot = rot * lf_coord + offset / size;
	vec2 xy = round((coord_rot+0.5) * size / D);

	vec4 pixel = vec4(0);

	for(int u = -lim; u <= lim; u++) {
		for(int v = -lim; v <= lim; v++ ){
			vec2 uv = vec2(u,v);
			vec2 xy_t = xy + t*uv;
			vec2 uimage_coord = xy_t*D + uv;
			pixel += texture(lf, uimage_coord / size);
		}
	}

	out_color = pixel / pow(lim*2.0+1, 2.0);

	/* out_color = texture2D(lf, lf_coord); */
	/* out_color = vec4(lf_coord, 0.0, 1.0); */
}
