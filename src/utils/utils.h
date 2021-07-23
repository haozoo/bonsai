/* Collection of helper functions for using OpenGL and GLFW
 * -- Hao X. July 2021
 */

#include <GLFW/glfw3.h>
#include <iostream>

// function declarations -------------------------------------------------------
void initialize_glfw(unsigned int major, unsigned int minor);
GLFWwindow *create_context(std::string name, unsigned int scr_height,
                           unsigned int scr_width);

// functions -------------------------------------------------------------------
// initializes and sets window hints
void initialize_glfw(unsigned int major, unsigned int minor) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// creates glfw context and returns window
GLFWwindow *create_context(const char *name, unsigned int scr_height,
                           unsigned int scr_width) {
  GLFWwindow *window =
      glfwCreateWindow(scr_width, scr_height, name, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return NULL;
  }
  glfwMakeContextCurrent(window);
  return window;
}