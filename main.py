from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.arrays import vbo
from OpenGL.GL import shaders
from PIL import Image
import numpy as np


def load_image():
	im = Image.open('img/macro.tif')
	tex = glGenTextures(1)
	glBindTexture(GL_TEXTURE_2D, tex)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
	glTexImage2D(GL_TEXTURE_2D, 0, 3, im.size[0], im.size[1], 0, GL_RGB, GL_UNSIGNED_BYTE, im.tostring('raw','RGB',0,-1))
	glEnable(GL_TEXTURE_2D)


def setup_shaders():
	fs = shaders.compileShader(open('lightfield.fs').read(), GL_FRAGMENT_SHADER)
	vs = shaders.compileShader(open('lightfield.vs').read(), GL_VERTEX_SHADER)
	program = shaders.compileProgram(vs,fs)


def setup_vbo():
	vbo.VBO(np.array([-1.0,-1.0, 1.0,-1.0, 1.0,1.0, -1.0,1.0], 'f'))
	

def display():
	glClear(GL_COLOR_BUFFER_BIT)
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)
	glutSwapBuffers()


def reshape(w,h):
	glViewport(0,0,w,h)


def main():
	glutInit()
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA)
	glutInitWindowSize(1024,768)
	glutInitWindowPosition(0,0)
	glutCreateWindow("lytrogl")

	glClearColor(0.0, 0.0, 0.0, 1.0)

	glutDisplayFunc(display)
	glutReshapeFunc(reshape)

	load_image()
	
	glutMainLoop()


main()
