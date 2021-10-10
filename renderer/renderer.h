#ifndef renderer_h
#define renderer_h

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "setup.h"
#include "../unit.h"
#include <glm/glm.hpp>

#define SCR_WIDTH 1080.0
#define SCR_HEIGHT 1080.0
#define UNIT_SIZE 0.025f

using namespace glm;

typedef struct {
  Unit *units;
  size_t units_count;
  vec2 currentCameraPos;
  vec2 targetCameraPos;
  float visibleDistance;
  float visibleSectorAngle;
} RenderingContext;

typedef struct {
  GLuint program;
  GLuint vbo, vao;
  GLint modelUniform;
  GLint zoomScaleUniform;
  GLint defaultScaleUniform;
  GLint isSelectedUniform;
  GLint cameraPosUniform;
  GLint visibleSectorAngleUniform;
} RenderProgram;

typedef struct {
  RenderProgram unitsProgram;
  RenderProgram visibleSectorProgram;
  float zoomScale;
  RenderingContext *context;
} Renderer;

typedef struct {
  void (*mouseButtonCallback)(double, double);
  void (*scrollCallback)(double, double);
} WindowPresenter;

static float vertices[] = {
  -1.0f, -1.0f,
  1.0f, -1.0f,
  1.0f,  1.0f,
  1.0f,  1.0f,
  -1.0f,  1.0f,
  -1.0f, -1.0f,
};

WindowPresenter windowPresenter;
GLFWwindow *window;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  if (windowPresenter.scrollCallback != NULL) {
    windowPresenter.scrollCallback(xoffset, yoffset);
  }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) == false)
    return;
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  if (windowPresenter.mouseButtonCallback != NULL) {
    windowPresenter.mouseButtonCallback((x / (SCR_WIDTH / 2.0)) - 1.0,
                                        1.0 - (y / (SCR_HEIGHT / 2.0)));
  }
}

void setupWindow() {
  glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 256);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"Tutorial 02 - Red triangle", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = 1;
  glewInit();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

static RenderProgram makeUnitsProgram() {
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
  GLint isSelectedUniform = glGetUniformLocation(program, "isSelected");
  GLint cameraPosUniform = glGetUniformLocation(program, "cameraPos");

  RenderProgram output = {
    .program = program,
    .vbo = vbo,
    .vao = vao,
    .modelUniform = modelUniform,
    .zoomScaleUniform = zoomScaleUniform,
    .defaultScaleUniform = defaultScaleUniform,
    .isSelectedUniform = isSelectedUniform,
    .cameraPosUniform = cameraPosUniform,
  };
  return output;
}

static RenderProgram makeVisibleSectorProgram() {
  GLuint program = reload_shaders("./renderer/shaders/main.sector.vert", "./renderer/shaders/main.sector.frag", 0);
  glUseProgram(program);

  GLint modelUniform = glGetUniformLocation(program, "model");
  GLint zoomScaleUniform = glGetUniformLocation(program, "zoomScale");
  GLint defaultScaleUniform = glGetUniformLocation(program, "defaultScale");
  GLint cameraPosUniform = glGetUniformLocation(program, "cameraPos");
  GLint visibleSectorAngleUniform = glGetUniformLocation(program, "visibleSectorAngle");

  GLuint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

  RenderProgram output = {
    .program = program,
    .vbo = vbo,
    .vao = vao,
    .modelUniform = modelUniform,
    .zoomScaleUniform = zoomScaleUniform,
    .defaultScaleUniform = defaultScaleUniform,
    .cameraPosUniform = cameraPosUniform,
    .visibleSectorAngleUniform = visibleSectorAngleUniform
  };
  return output;
}

Renderer makeRenderer() {
  RenderProgram visibleSectorProgram = makeVisibleSectorProgram();
  RenderProgram unitsProgram = makeUnitsProgram();
  Renderer output = {
    .unitsProgram = unitsProgram,
    .visibleSectorProgram = visibleSectorProgram,
    .zoomScale = 1.0f
  };
  return output;
}

static void renderUnitsProgram(Renderer *renderer) {
  RenderingContext *context = renderer->context;
  RenderProgram *program = &renderer->unitsProgram;
  glUseProgram(program->program);
  glBindVertexArray(program->vao);
  glUniform1f(program->zoomScaleUniform, renderer->zoomScale);
  glUniform1f(program->defaultScaleUniform, 0.025f);
  glUniform2fv(program->cameraPosUniform, 1, (float *)&(context->currentCameraPos[0]));
  for (size_t i = 0; i < context->units_count; i++) {
    glUniform2fv(program->modelUniform, 1, (float *)&(context->units[i].pos[0]));
    glUniform1i(program->isSelectedUniform, context->units[i].isSelected);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 2));
  }
}

static void renderVisibleSectorProgram(Renderer *renderer) {
  RenderingContext *context = renderer->context;
  Unit *selectedUnit = NULL;
  for (size_t i = 0; i < context->units_count; i++) {
    if (context->units[i].isSelected) {
      selectedUnit = &context->units[i];
      break;
    }
  }
  if (selectedUnit == NULL)
    return;
  RenderProgram *program = &renderer->visibleSectorProgram;
  glUseProgram(program->program);
  glBindVertexArray(program->vao);
  glUniform1f(program->zoomScaleUniform, renderer->zoomScale);
  glUniform1f(program->defaultScaleUniform, context->visibleDistance);
  glUniform1f(program->visibleSectorAngleUniform, context->visibleSectorAngle);
  glUniform2fv(program->cameraPosUniform, 1, (float *)&(context->currentCameraPos[0]));
  glUniform2fv(program->modelUniform, 1, (float *)&(selectedUnit->pos[0]));
  glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 2));
}

void render(Renderer *renderer) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  renderUnitsProgram(renderer);
  renderVisibleSectorProgram(renderer);
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool should_close() {
  return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && 
    glfwWindowShouldClose(window) == 0;
}

#endif
