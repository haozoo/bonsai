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
#include "bonsai/bonsai.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "stb_image.h"
#include "texture/texture.h"
#include "utils/utils.h"

using namespace std;

// function declarations -------------------------------------------------------
void setCallbacks(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void renderCubeArray(vector<glm::vec3> cubeArray, Shader shader,
                     unsigned int tick, unsigned int texture);
void configureVertexObjects(unsigned int &VBO, unsigned int &cubeVAO,
                            unsigned int &lightCubeVAO);

// constants
// -------------------------------------------------------------------
// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera settings
Camera camera(glm::vec3(0.0f, 20.0f, 60.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
unsigned int tick = 0;

// lighting
glm::vec3 lightPos(0.0f, 30.0f, 0.0f);
float red = 0.02f, green = 0.02f, blue = 0.03f, alpha = 1.0f;

// bonsai
Bonsai tree;

/*
   ________
  /⠡      /\
 /  ⠡..../..\
/___⠌___/   /   cube vertices
\  ⠌    \  /    - in OpenGl a cube is best rendered as a set of triangles
 \⠌______\/     - therefore, 12 triangles with 3 points each = 36 vertices

*/
const int VERTEX_LENGTH = 8;
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

// main ------------------------------------------------------------------------
int main() {
  srand(time(NULL));

  initialize_glfw(3, 3);
  GLFWwindow *window = create_context("Bonsai", SCR_HEIGHT, SCR_WIDTH);
  setCallbacks(window);

  // use glad to load / manage function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global OpenGL state
  glEnable(GL_DEPTH_TEST);

  // build and compile shader programs from the following
  Shader lightingShader("src/shaders/shadervs", "src/shaders/shaderfs");
  Shader lightCubeShader("src/shaders/sourcevs", "src/shaders/sourcefs");

  // configure cube VBO and VBA
  unsigned int VBO, cubeVAO, lightCubeVAO;
  configureVertexObjects(VBO, cubeVAO, lightCubeVAO);

  // load in textures (diffuse map + specular map for lighting)
  unsigned int bark = loadTexture("img/log.jpg");
  unsigned int leaf = loadTexture("img/leaf.png");
  unsigned int pot = loadTexture("img/pot.jpg");
  unsigned int soil = loadTexture("img/moss.jpg");

  // assign texture units to samplers
  lightingShader.use();
  lightingShader.setInt("texture", 0);
  lightingShader.setInt("texture", 1);
  lightingShader.setInt("texture", 2);
  lightingShader.setInt("texture", 3);

  // render loop ---------------------------------------------------------------
  while (!glfwWindowShouldClose(window)) {

    // frame logic
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // process user input
    processInput(window);

    // render background
    glClearColor(red, green, blue, alpha); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader for setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.configure(camera.Position, lightPos);

    // set projection
    float fovy = glm::radians(camera.Zoom);
    float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    glm::mat4 projection = glm::perspective(fovy, aspect, 0.1f, 100.0f);
    lightingShader.setMat4("projection", projection);

    // set view matrix
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);

    // bind and render objects -------------------------------------------------
    // bonsai objects
    glBindVertexArray(cubeVAO);
    renderCubeArray(tree.BranchPositions, lightingShader, tick, bark);
    renderCubeArray(tree.LeafPositions, lightingShader, tick, leaf);
    renderCubeArray(tree.SoilPositions, lightingShader, tick, soil);
    renderCubeArray(tree.PotPositions, lightingShader, tick, pot);

    // light object
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.5f));
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

// input functions -------------------------------------------------------------
// handles user input
void processInput(GLFWwindow *window) {
  // movement controls
  if (camera.Mode == USER) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
  }

  // general controls
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // switchs camera mode
    camera.switchMode();
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // exits
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // creates new tree
    Bonsai newTree;
    tree = newTree;
    tick = 0;
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // re-animates tree
    tick = 0;
  }
}

// callbacks -------------------------------------------------------------------
// sets all callbacks
void setCallbacks(GLFWwindow *window) {
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

// callback to ensure viewport resizes accordingly upon window resize
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// callback to handle mouse movement
void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (camera.Mode == USER) { // only enable mouse movement in USER mode
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
}

// callback to handle mouse scroll wheel
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

// OpenGL helper functions -----------------------------------------------------
// renders a vector of cube positions based on current tick
// - number of cubes rendered equals the current tick, this animates the model
void renderCubeArray(vector<glm::vec3> cubeArray, Shader shader,
                     unsigned int tick, unsigned int texture) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  for (unsigned int i = 0; i < tick && i < cubeArray.size(); i++) {
    // calc model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubeArray[i]);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}

// binds and configures vertex buffer and attribute objects for each cube
// - VBO is shared between cube and light cube
// - VAO must be configured separately
void configureVertexObjects(unsigned int &VBO, unsigned int &cubeVAO,
                            unsigned int &lightCubeVAO) {
  // configure default cubes VAO & VBO
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindVertexArray(cubeVAO);

  // inform openGL how to intepret vertex attribute data, for example:
  // vertex = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f}
  //           -------------------  -----------   -----------------
  //                    |                |                |
  //             position coords      normals       texture coords
  int len = VERTEX_LENGTH * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, len, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, len,
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, len,
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // configure light cube's VAO (VBO same)
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // adjust stride in attributes for light cube VAO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}