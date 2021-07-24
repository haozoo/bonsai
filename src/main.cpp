#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "bonsai/bgen.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "stb_image.h"
#include "utils/utils.h"

using namespace std;

// function declarations -------------------------------------------------------
void setCallbacks(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void renderCubeArray(vector<glm::vec3> cubeArray, Shader shader,
                     unsigned int tick);

// constants
// -------------------------------------------------------------------
// screen settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1080;

// camera settings
Camera camera(glm::vec3(0.0f, 20.0f, 60.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.0f, 30.0f, 0.0f);

// main ------------------------------------------------------------------------
int main() {
  initialize_glfw(3, 3);
  GLFWwindow *window = create_context("Bonsai Tree", SCR_HEIGHT, SCR_WIDTH);
  setCallbacks(window);

  // use glad to load / manage function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global OpenGL state
  glEnable(GL_DEPTH_TEST);

  // build and compile shader programs from the following
  Shader lightingShader(
      "/home/hao/Documents/github/fun/bonsai/src/shaders/shadervs",
      "/home/hao/Documents/github/fun/bonsai/src/shaders/shaderfs");
  Shader lightCubeShader(
      "/home/hao/Documents/github/fun/bonsai/src/shaders/sourcevs",
      "/home/hao/Documents/github/fun/bonsai/src/shaders/sourcefs");

  // define cube vertices
  /*
    ________
   /⠡      /\
  /  ⠡..../..\
 /___⠌___/   /
 \  ⠌    \  /
  \⠌______\/

 */
  float vertices[] = {
      // positions         // normals     // texture coords //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f  //
  };
  srand(time(NULL));
  vector<glm::vec3> branchPositions, leafPositions, potPositions, soilPositions;
  createBonsai(branchPositions, leafPositions, potPositions, soilPositions);

  // configure cube VBO and VBA
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // configure light cube's VAO (VBO same)
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // note that we update the lamp's position attribute's stride to reflect the
  // updated buffer data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // load in textures (diffuse map + specular map for lighting)
  unsigned int diffuseMap =
      loadTexture("/home/hao/Documents/github/fun/bonsai/img/brown.jpg");
  unsigned int specularMap =
      loadTexture("/home/hao/Documents/github/fun/bonsai/img/brown.jpg");
  unsigned int leaf =
      loadTexture("/home/hao/Documents/github/fun/bonsai/img/leaf4.png");
  unsigned int pot =
      loadTexture("/home/hao/Documents/github/fun/bonsai/img/black.jpg");
  unsigned int soil =
      loadTexture("/home/hao/Documents/github/fun/bonsai/img/moss.jpg");

  // assign texture units to samplers
  lightingShader.use();
  lightingShader.setInt("material.diffuse", 0);
  lightingShader.setInt("material.specular", 1);
  lightingShader.setInt("texture", 2);
  lightingShader.setInt("texture", 3);
  lightingShader.setInt("texture", 4);

  // render loop ---------------------------------------------------------------
  unsigned int tick = 0;
  while (!glfwWindowShouldClose(window)) {

    // frame logic
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // process user input
    processInput(window);

    // render background
    glClearColor(0, 0, 0, 0); // cyan
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader for setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("viewPos", camera.Position);

    // light properties
    lightingShader.setVec3("light.ambient", 0.7f, 0.7f, 0.7f);
    lightingShader.setVec3("light.diffuse", 1.3f, 1.3f, 1.3f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    lightingShader.setFloat("material.shininess", 64.0f);

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);

    // bind and render objects -------------------------------------------------
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glBindVertexArray(cubeVAO);
    renderCubeArray(branchPositions, lightingShader, tick);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, leaf);
    renderCubeArray(leafPositions, lightingShader, tick);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, soil);
    renderCubeArray(soilPositions, lightingShader, tick);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pot);
    renderCubeArray(potPositions, lightingShader, tick);

    // draw light object
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    // model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // swap buffers and check for inputs
    glfwSwapBuffers(window);
    glfwPollEvents();
    tick++;
  }

  // clean-up ------------------------------------------------------------------
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

// functions -------------------------------------------------------------------
// handles user input
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// sets all callbacks
void setCallbacks(GLFWwindow *window) {
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

// callback: ensures viewport resizes accordingly upon window resize
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// callback: handles mouse movement
void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coords go bottom -> top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// callback: handles mouse scroll wheel
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

// renders a vector of cube possitions
void renderCubeArray(vector<glm::vec3> cubeArray, Shader shader,
                     unsigned int tick) {
  for (unsigned int i = 0; i < tick && i < cubeArray.size(); i++) {

    // calc model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubeArray[i]);
    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}