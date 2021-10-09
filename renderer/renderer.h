#ifndef renderer_h
#define renderer_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "setup.h"
#include "../unit.h"

#define SCR_WIDTH 1080
#define SCR_HEIGHT 1080

typedef struct {
  GLFWwindow* window;
  GLuint program;
  GLuint vbo, vao;
  GLint modelUniform;
} Renderer;

static float vertices[] = {
  -1.0f, -1.0f,
   1.0f, -1.0f,
   1.0f,  1.0f,
   1.0f,  1.0f,
  -1.0f,  1.0f,
  -1.0f, -1.0f,
};

void processInput(GLFWwindow *window);

Renderer make_renderer() {
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 256);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"Tutorial 02 - Red triangle", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = 1;
  glewInit();
  /* glEnable(GL_DEPTH_TEST); */
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  GLuint program = reload_shaders("./renderer/shaders/main.vert", "./renderer/shaders/main.frag", 0);
  glUseProgram(program);

  GLuint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);


  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  GLint modelUniform = glGetUniformLocation(program, "model");

  Renderer renderer = {
    .window = window,
    .program = program,
    .vbo = vbo,
    .vao = vao,
    .modelUniform = modelUniform
  };
  return renderer;
}

void render(Renderer *renderer, Unit *units, size_t units_count) {
  for (size_t i = 0; i < units_count; i++) {
    glUniform2fv(renderer->modelUniform, 1, (float *)&units[i].pos[0]);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 2));
  }
  glfwSwapBuffers(renderer->window);
  glfwPollEvents();
}

bool should_close(Renderer *renderer) {
  return glfwGetKey(renderer->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && 
    glfwWindowShouldClose(renderer->window) == 0;
}

#endif
