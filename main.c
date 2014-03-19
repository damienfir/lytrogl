#include <FreeImage.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "opengl.h"

const char* fname = "img/macro.tif";

Shader shader;
GLuint texture;
GLuint framebuffer_id;
GLuint renderbuffer_id;

struct
{
    VBO vertices;
} vbo;

struct
{
    GLint vertices;
} attrib;

struct
{
    GLint lf;
    GLint imsize;
    GLint t;
    GLint D;
} unif;


void
shader_setup()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader = shader_init("lightfield.v.glsl","lightfield.f.glsl");
    shader_use(shader);

    GLfloat vertices[] = {-1.f,-1.f, 1.f,-1.f, -1.f,1.f, 1.f,1.f};
    GLuint vbo_vertices;
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    attrib.vertices = glGetAttribLocation(shader.program, "vertices");
    glEnableVertexAttribArray(attrib.vertices);
    glVertexAttribPointer(attrib.vertices, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void
lightfield_setup()
{

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
    unif.t = glGetUniformLocation(shader.program, "t");

    glUniform1i(unif.lf, 0);

    float t[1] = {-2.f};
    glUniform1fv(unif.t, 1, t);

    /* unif.D = glGetUniformLocation(shader.program, "D"); */
    /* printf("%d\n", unif.D); */
    /* glUniform1i(unif.D, 9.97); */

    /* unif.imsize = glGetUniformLocation(shader.program, "imsize"); */
    /* glUniform2i(unif.imsize, width, height); */

}

void
framebuffer_setup()
{
    glGenFramebuffers(1, &framebuffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

    glGenRenderbuffers(1, &renderbuffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer_id);

    int status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("framebuffer not complete\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void
save_output()
{
    int size = sizeof(GLubyte) * 3 * 1024 * 1024;
    BYTE *im = malloc(size);
    glReadPixels(0,0, 1024,1024, GL_RGB, GL_UNSIGNED_BYTE, im);

    FIBITMAP *image = FreeImage_ConvertFromRawBits(im, 1024, 1024, 3*1024, 8,  FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
    FreeImage_Save(FIF_PNG, image, "output.png", 0);
}

static gboolean
display(GtkWidget *drawing, GdkEventConfigure *event, gpointer user_data)
{

    GdkGLContext *gl_context = gtk_widget_get_gl_context(drawing);
    GdkGLDrawable *gl_drawable = gtk_widget_get_gl_drawable(drawing);

    gdk_gl_drawable_gl_begin(gl_drawable, gl_context);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (gdk_gl_drawable_is_double_buffered(gl_drawable))
    {
        gdk_gl_drawable_swap_buffers(gl_drawable);
    }

    gdk_gl_drawable_gl_end(gl_drawable);
}

void
keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 's':
            printf("saving image..\n");
            save_output();
            break;
    }
}

static gboolean
reshape(GtkWidget *drawing, GdkEventConfigure *event, gpointer user_data)
{
    GdkGLContext *gl_context = gtk_widget_get_gl_context(drawing);
    GdkGLDrawable *gl_drawable = gtk_widget_get_gl_drawable(drawing);

    gdk_gl_drawable_gl_begin(gl_drawable, gl_context);

    glViewport(0, 0, drawing->allocation.width, drawing->allocation.height); 

    gdk_gl_drawable_gl_end(gl_drawable);
}

static gboolean
realize(GtkWidget *drawing, GdkEventConfigure *event, gpointer user_data)
{
    GdkGLContext *gl_context = gtk_widget_get_gl_context(drawing);
    GdkGLDrawable *gl_drawable = gtk_widget_get_gl_drawable(drawing);

    gdk_gl_drawable_gl_begin(gl_drawable, gl_context);

    shader_setup();
    lightfield_setup();

    gdk_gl_drawable_gl_end(gl_drawable);
}

int
main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 1024);
    GtkWidget *drawing = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing);
    gtk_widget_set_events(drawing, GDK_EXPOSURE_MASK);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    gtk_gl_init(&argc, &argv);
    GdkGLConfig *gl_config = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB  | GDK_GL_MODE_DOUBLE);
    if (!gl_config) {
        g_assert_not_reached();
    }
    if (!gtk_widget_set_gl_capability(drawing, gl_config, NULL, TRUE, GDK_GL_RGBA_TYPE)) {
        g_assert_not_reached();
    }
    gtk_widget_set_gl_capability(drawing, gl_config, NULL, TRUE, GDK_GL_RGBA_TYPE);
    g_signal_connect(drawing, "realize", G_CALLBACK(realize), NULL);
    g_signal_connect(drawing, "configure-event", G_CALLBACK(reshape), NULL);
    g_signal_connect(drawing, "expose-event", G_CALLBACK(display), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
