#ifndef renderer_h
#define renderer_h

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "setup.h"
#include "../unit.h"

#define SCR_WIDTH 1080.0
#define SCR_HEIGHT 1080.0
#define UNIT_SIZE 0.025f

typedef struct {
  GLFWwindow* window;
  GLuint program;
  GLuint vbo, vao;
  GLint modelUniform;
  GLint zoomScaleUniform;
  float zoomScale;
  GLint defaultScaleUniform;
  void (*mouse_button_callback)(double, double);
} Renderer;

static float vertices[] = {
  -1.0f, -1.0f,
   1.0f, -1.0f,
   1.0f,  1.0f,
   1.0f,  1.0f,
  -1.0f,  1.0f,
  -1.0f, -1.0f,
};

Renderer *renderer;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  renderer->zoomScale += (float)yoffset/10.0f;
  if (renderer->zoomScale < 0.1)
    renderer->zoomScale = 0.1;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) == false)
    return;
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  if (renderer->mouse_button_callback != NULL)
    renderer->mouse_button_callback((x / (SCR_WIDTH / 2.0)) - 1.0,
                                    1.0 - (y / (SCR_HEIGHT / 2.0)));
}

void make_renderer() {
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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);

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
  GLint zoomScaleUniform = glGetUniformLocation(program, "zoomScale");
  GLint defaultScaleUniform = glGetUniformLocation(program, "defaultScale");

  renderer = (Renderer *)malloc(sizeof(Renderer));
  *renderer = Renderer {
    .window = window,
    .program = program,
    .vbo = vbo,
    .vao = vao,
    .modelUniform = modelUniform,
    .zoomScaleUniform = zoomScaleUniform,
    .zoomScale = 1.0f,
    .defaultScaleUniform = defaultScaleUniform,
    .mouse_button_callback = NULL
  };
}

void render(Renderer *renderer, Unit *units, size_t units_count) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUniform1f(renderer->zoomScaleUniform, renderer->zoomScale);
  glUniform1f(renderer->defaultScaleUniform, 0.025f);
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
