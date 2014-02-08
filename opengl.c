#include "opengl.h"

VBO vbo_init(GLfloat* vertices, GLuint* indices) {
	VBO out = {0,0};

	if(vertices != NULL) {
		glGenBuffers(1, &(out.vbo));
		glBindBuffer(GL_ARRAY_BUFFER, out.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	}

	if(indices != NULL) {
		glGenBuffers(1, &(out.ibo));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	}

	vbo_unbind();

	return out;
}

void vbo_bind(VBO vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.ibo);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void vbo_unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void vbo_kill(VBO vbo)
{
	glDeleteBuffers(1,vbo.vbo);
}


char* read_file(const char* filename){
	long n, r;
	FILE *fp = fopen(filename, "r");

	fseek(fp, 0L, SEEK_END);
	n = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	
	char * out = (char*)malloc(n);
	r = fread(out, 1, n, fp);
	fclose(fp);
	out[r-1] = '\0';

	return out;
}

GLuint compile(const char* filename, GLenum type)
{
	GLint compile_ok = GL_FALSE;

	const char *source = read_file(filename);
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	free(source);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) {
		GLint log_length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = (char*)malloc(log_length);
		glGetShaderInfoLog(id, log_length, NULL, log);
		fprintf(stderr, "%s", log);
		free(log);
	}

	return id;
}

Shader shader_init(const char *vs_filename, const char *fs_filename)
{
	GLint link_ok = GL_FALSE;

	GLuint vs=compile(vs_filename, GL_VERTEX_SHADER);
	GLuint fs=compile(fs_filename, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		char* log = (char*)malloc(log_length);
		glGetProgramInfoLog(program, log_length, NULL, log);
		fprintf(stderr, "%s", log);
		free(log);
	}

	Shader shader = {.fs=fs,.vs=vs,.program=program};
	return shader;
}


void shader_kill(Shader shader)
{
	glDeleteProgram(shader.program);
}


/* void get_attribute(const char *name) */
/* { */
/* 	attributes[name] = glGetAttribLocation(program, name); */
/* 	if (attributes[name] == -1) { */
/* 		fprintf(stderr, "Could not get attribute %s\n", name); */
/* 	} */
/* } */

/* void get_uniform(const char *name) */
/* { */
/* 	uniforms[name] = glGetUniformLocation(program, name); */
/* 	if (uniforms[name] == -1) { */
/* 		fprintf(stderr, "Could not get uniform %s\n", name); */
/* 	} */
/* } */

/* void bind_attribute(Shader shader, GLuint location, const GLchar* name) */
/* { */
/* 	shader.attrib[location] = location; */
/* 	glBindAttribLocation(shader.program, location, name); */
/* } */

/* void use(Shader shader) */
/* { */
/* 	glUseProgram(shader.program); */
/* 	/1* map<string, GLint>::iterator p; *1/ */
/* 	/1* for (p = attributes.begin(); p != attributes.end(); ++p) { *1/ */
/* 	/1* 	glEnableVertexAttribArray(p->second); *1/ */
/* 	/1* } *1/ */
/* } */
