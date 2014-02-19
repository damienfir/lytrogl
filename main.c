#include <FreeImage.h>

#include "opengl.h"

const char* fname = "img/macro.tif";

Shader shader;
GLuint texture;

/* struct { */
/* 	VBO vertices; */
/* 	VBO coord; */
/* } vbo; */
struct {
	VBO vertices;
	VBO coord;
} vbo;

struct {
	GLint vertices;
	GLint coord;
} attrib;

struct {
	GLint lf;
	GLint imsize;
	GLint t;
	GLint D;
} unif;


void setup() {
	
	shader = shader_init("lightfield.vs","lightfield.fs");
	shader_use(shader);

	GLfloat vertices[] = {-1.f,-1.f, 1.f,-1.f, -1.f,1.f, 1.f,1.f};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	attrib.vertices = glGetAttribLocation(shader.program, "vertices");
	glVertexAttribPointer(attrib.vertices, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib.vertices);

	GLfloat coord[] = {-0.5,-0.5, 0.5,-0.5, -0.5,0.5, 0.5,0.5};
	GLuint vbo_coord;
	glGenBuffers(1, &vbo_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
	attrib.coord = glGetAttribLocation(shader.program, "coord");
	glEnableVertexAttribArray(attrib.coord);
	glVertexAttribPointer(attrib.coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void load_lf() {
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fname, 0);
	FIBITMAP *img = FreeImage_Load(fif, fname, TIFF_DEFAULT);
	int width = FreeImage_GetWidth(img);
	int height = FreeImage_GetHeight(img);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, FreeImage_GetBits(img));
	FreeImage_Unload(img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unif.lf = glGetUniformLocation(shader.program, "lf");
	glUniform1i(unif.lf, 0);

	/* unif.t = glGetUniformLocation(shader.program, "t"); */
	/* glUniform1i(unif.t, 0); */

	/* unif.D = glGetUniformLocation(shader.program, "D"); */
	/* glUniform1i(unif.D, 9.97); */

	/* unif.imsize = glGetUniformLocation(shader.program, "imsize"); */
	/* glUniform2i(unif.imsize, width, height); */
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glutSwapBuffers();
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024,1024);
	glutCreateWindow("lytrogl");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(&display);

	setup();
	load_lf();

	glutMainLoop();

	return 0;
}
