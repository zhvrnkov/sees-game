#include "setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdarg.h>

GLint resolution_location = 0;
GLint time_location = 0;

void panic_errno(const char *fmt, ...)
{
    fprintf(stderr, "ERROR: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, ": %s\n", strerror(errno));

    exit(1);
}

char *slurp_file(const char *file_path)
{
#define SLURP_FILE_PANIC panic_errno("Could not read file `%s`", file_path)
    FILE *f = fopen(file_path, "r");
    if (f == NULL) SLURP_FILE_PANIC;
    if (fseek(f, 0, SEEK_END) < 0) SLURP_FILE_PANIC;

    long size = ftell(f);
    if (size < 0) SLURP_FILE_PANIC;

    char *buffer = (char *)malloc(size + 1);
    if (buffer == NULL) SLURP_FILE_PANIC;

    if (fseek(f, 0, SEEK_SET) < 0) SLURP_FILE_PANIC;

    fread(buffer, 1, size, f);
    if (ferror(f) < 0) SLURP_FILE_PANIC;

    buffer[size] = '\0';

    if (fclose(f) < 0) SLURP_FILE_PANIC;

    return buffer;
#undef SLURP_FILE_PANIC
}

bool compile_shader_source(const GLchar *source, GLenum shader_type, GLuint *shader)
{
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLchar message[1024];
        GLsizei message_size = 0;
        glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
        fprintf(stderr, "%.*s\n", message_size, message);
        return false;
    }

    return true;
}

bool compile_shader_file(const char *file_path, GLenum shader_type, GLuint *shader)
{
    char *source = slurp_file(file_path);
    bool err = compile_shader_source(source, shader_type, shader);
    free(source);
    return err;
}

bool link_program(GLuint vert_shader, GLuint frag_shader, GLuint *program)
{
    *program = glCreateProgram();

    glAttachShader(*program, vert_shader);
    glAttachShader(*program, frag_shader);
    glLinkProgram(*program);

    GLint linked = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei message_size = 0;
        GLchar message[1024];

        glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
        fprintf(stderr, "Program Linking: %.*s\n", message_size, message);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

GLuint reload_shaders(const char *vert_shader_path, const char *frag_shader_path, int oldProgram) {
    glDeleteProgram(oldProgram);

    GLuint program = 0;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint vert = 0;
    compile_shader_file(vert_shader_path, GL_VERTEX_SHADER, &vert);

    GLuint frag = 0;
    compile_shader_file(frag_shader_path, GL_FRAGMENT_SHADER, &frag);

    link_program(vert, frag, &program);

    glUseProgram(program);

    resolution_location = glGetUniformLocation(program, "resolution");
    time_location = glGetUniformLocation(program, "time");

    printf("Successfully Reload the Shaders\n");

    return program;
}

