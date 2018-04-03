#include <cstdio>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <src/renderer/material/Shader.h>
#include <src/game/Camera.h>
#include <src/renderer/geometry/ObjLoader.h>
#include <iostream>
#include <src/renderer/Renderer.h>

// OpenGL messages
void MessageCallback(GLenum source, GLenum type, GLuint id,
                     GLenum severity, GLsizei length,
                     const GLchar *message, const void *userParam) {
  std::cerr << message << std::endl;
}

int main() {
  try {
    auto window = std::make_shared<Window>();
    Renderer renderer(window);
    Game game(window);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC) MessageCallback, nullptr);

    double lastFrameTime = glfwGetTime();

    while (glfwGetKey(window->handle, GLFW_KEY_ESCAPE) != GLFW_PRESS
        && glfwWindowShouldClose(window->handle) == 0) {
      // Get elapsed time
      auto currentFrame = glfwGetTime();
      auto deltaTime = static_cast<float>(currentFrame - lastFrameTime);
      lastFrameTime = currentFrame;

      game.update(deltaTime);

      renderer.renderFrame(game);

      glfwPollEvents();
    }

    glfwTerminate();
    return 0;
  } catch (std::exception &exception) {
    std::cerr << exception.what() << std::endl;
    return 1;
  }
}
