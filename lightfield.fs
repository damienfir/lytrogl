# version 440

in vec2 lf_coord;
out vec4 out_color;

uniform sampler2D lf;

/* uniform float t; */
/* uniform float D; */
/* uniform float offset; */


void main(void) {

	int lim = 0;
	float zoom = 5;

	// from user
	float t = 0;

	// from metadata
	int size = 3280;
	float D = 9.92757132952;
	float usize = vec2(D, D*sqrt(3)/2);
	vec2 offset_xy = vec2(-3.3, -5.2);
	float angle = -0.0048796734772622585;
	mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

	vec2 e1 = rot * vec2(D, 0);
	vec2 e2 = rot * vec2(D * sqrt(3)/2, D/2);
	mat2 h = mat2(e1, e2);

	vec2 proj = transpose(h) * lf_coord;
	vec2 coef = vec2(0);
	coef.x = proj.x / length(e1);
	coef.y = proj.y / length(e2);

	out_color = texture(lf, lf_coord);

	/* vec4 pixel_color = vec4(0); */

	/* for(int u = -lim; u <= lim; u++) { */
	/* 	for(int v = -lim; v <= lim; v++ ){ */
	/* 		vec2 uv = vec2(u,v); */
	/* 		vec2 xy_t = xy + t*uv; */
	/* 		vec2 xyuv = (xy_t*D + uv) / size; */
	/* 		pixel_color += texture(lf, xyuv); */
	/* 	} */
	/* } */

	/* out_color = pixel_color / pow(lim*2.0+1, 2.0); */

	if (length(xy - xy_r) < 5e-2) {
		out_color = vec4(1);
	}
	/* out_color = vec4(lf_coord, 0.0, 1.0); */
}
