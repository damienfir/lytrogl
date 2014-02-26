# version 420

in vec2 lf_coord;
out vec4 out_color;

uniform sampler2D lf;
uniform float t;
/* uniform float D; */
/* uniform float offset; */

vec2 nearest(vec2 axial) {
	vec3 cube = vec3(axial.x, -axial.x - axial.y, axial.y);
	vec3 cube_r = round(cube);
	vec3 diff = abs(cube_r - cube);

	if (diff.x > diff.y && diff.x > diff.z) {
		cube_r.x = -cube_r.y - cube_r.z;
	} else if (diff.y > diff.z) {
		cube_r.y = -cube_r.x - cube_r.z;
	} else {
		cube_r.z = -cube_r.x - cube_r.y;
	}

	vec2 axial_r = vec2(cube_r.x, cube_r.z);

	return axial_r;
}


void main(void) {

	int lim = 3;
	float zoom = 5;

	// from metadata
	int size = 3280;
	float D = 9.92757132952;
	vec2 offset_xy = vec2(3.3, -5.2);
	vec2 scaling = vec2(1.0, 1.00006866455078125);
	float angle = 0.0048796734772622585;
	mat2 rot = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));

	mat2 hex = rot * (D * mat2(vec2(1, 0), vec2(1/2.0, sqrt(3)/2)));

	vec2 uimage_coord = lf_coord * size + offset_xy;
	vec2 proj = inverse(hex) * uimage_coord;
	vec2 coef = nearest(proj);

	/* out_color = texture(lf, (coef * hex_t) / size + 0.5); */

	vec4 pixel_color = vec4(0);

	for(int u = -lim; u <= lim; u++) {
		for(int v = -lim; v <= lim; v++ ){
			vec2 uv = vec2(u,v);
			vec2 loc = (coef + t*uv) * transpose(hex) + uv;
			pixel_color += texture(lf, loc / size + 0.5);
		}
	}

	out_color = pixel_color / pow(lim*2.0+1, 2.0);

	/* if (length(proj - coef) < 5e-2) { */
	/* 	out_color = vec4(1); */
	/* } */
	/* out_color = vec4(lf_coord, 0.0, 1.0); */
}
