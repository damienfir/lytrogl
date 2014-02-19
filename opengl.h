#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>


typedef struct {
	GLuint vs;
	GLuint fs;
	GLuint program;
} Shader;

Shader shader_init(const char *vs_filename, const char *fs_filename);
void shader_kill(Shader shader);
void shader_use(Shader shader);
void shader_unuse();


typedef struct {
	GLuint vbo;
	GLuint ibo;
} VBO;


VBO vbo_init(GLfloat* vertices);
void vbo_bind(VBO vbo);
void vbo_unbind();
void bind_attribute(Shader shader, GLuint location, const GLchar* name);
