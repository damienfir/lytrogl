#include <FreeImage.h>

#include "opengl.h"


void setup() {
	GLfloat v[] = {-1.f,-1.f, 1.f,-1.f, 1.f,1.f, -1.f,1.f};
	VBO vertices = vbo_init(v, NULL);

	GLfloat c[] = {0.f,3280.f, 3280.f,3280.f, 3280.f, 0.f};
	VBO coord = vbo_init(v, NULL);

	Shader shader = shader_init("lightfield.vs","lightfield.fs");
	shader_use(shader);

	vbo_bind(coord);
	GLint coord = glGetAttribLocation(program, "coord");
	glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void load_lf() {
	FreeImage_Initialise(1);
	FIBITMAP *img = FreeImage_ConvertTo32Bits(FreeImage_Load(FIF_TIFF, "img/macro.tif", TIFF_DEFAULT));
	
	GLint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(img), FreeImage_GetHeight(img), 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(img));
	glEnable(GL_TEXTURE_2D);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1024,768);
	glutCreateWindow("lytrogl");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(&display);

	load_lf();
	setup();

	glutMainLoop();

	return 0;
}
