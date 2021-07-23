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
#include "camera/camera.h"
#include "shaders/shader.h"
#include "stb_image.h"

using namespace std;

/* Callbacks */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

/* Bonsai */
void bonsai(glm::vec3 pos, vector<glm::vec3> &cubePositions, int growth);
void bonsai1(glm::vec3 pos, vector<glm::vec3> &cubePositions, int growth,
             int xdir, int zdir);
int check_collision(vector<glm::vec3> cubePositions, glm::vec3 pos);
int check_touching(vector<glm::vec3> cube_positions, glm::vec3 pos);

/* Screen Settings */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Camera Settings */
Camera camera(glm::vec3(0.0f, 20.0f, 60.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/* Timing */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
  /* 1. Initialize and configure GLFW to OpenGL 3.3 */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* 2. Create GLFW context */
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bonsai", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  /* 3. Set callbacks */
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED); // mouse capture

  /* 4. Use glad to load / manage function pointers */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  /* 5. Config global OpenGL state */
  glEnable(GL_DEPTH_TEST);

  /* 6. Build and compile shader programs from the following */
  Shader ourShader(
      "/home/hao/Documents/github/fun/bonsai/src/shaders/vshader.txt",
      "/home/hao/Documents/github/fun/bonsai/src/shaders/fshader.txt");

  /* 7. Configure VAO and buffers */
  float vertices[] = {
      // Vertex data for a single cube
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  /*
      ________
     /⠡      /\
    /  ⠡..../..\
   /___⠌___/   /
   \  ⠌    \  /
    \⠌______\/

   */

  srand(time(NULL));
  glm::vec3 origin = glm::vec3(0, 0, 0);
  vector<glm::vec3> cubePositions; // World space position of each cube
  bonsai1(origin, cubePositions, 50, 1, 1);
  for (unsigned int i = 0; i < cubePositions.size(); i++) {
    std::cout << cubePositions[i].x << cubePositions[i].y << cubePositions[i].z
              << endl;
  }

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Configure position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Configure texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* 8. Texture handling */
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char *data =
      stbi_load("/home/hao/Documents/github/fun/bonsai/img/block.png", &width,
                &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  /* 9. Assign texture units to samplers */
  ourShader.use();
  ourShader.setInt("texture", 0);

  /* 10. Render loop */
  while (!glfwWindowShouldClose(window)) {
    /* per-frame time logic */
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    /* render */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* textures */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* activate shader */
    ourShader.use();

    /* pass projection frame to shader */
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    /* camera view transform */
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);

    /* render all boxes */
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < cubePositions.size(); i++) {
      // calculate the model matrix for each object and pass it to shader
      // before drawing
      glm::mat4 model = glm::mat4(
          1.0f); // make sure to initialize matrix to identity matrix first
      model = glm::translate(model, cubePositions[i]);
      ourShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  /* Clean - up */
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

/* Function: handles user keyboard input */
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

/* Callback: ensures viewport resizes accordingly upon window resize */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

/* Callback: handles mouse movement */
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
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

/* Callback: handles mouse scroll wheel */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

/* Bonsai function */
void bonsai(glm::vec3 pos, vector<glm::vec3> &cubePositions, int growth) {
  /* Base case */
  if (growth == 0)
    return;

  /* Growth in a singular direction */
  glm::vec3 npos = pos;

  do {
    int dir = rand() % 3, xzdist = rand() % 3 - 1, ydist = rand() % 2;
    switch (dir) {
    case 0: // x-axis
      npos = pos + glm::vec3(xzdist, 0, 0);
      break;
    case 1: // y-axis
      npos = pos + glm::vec3(0, ydist, 0);
      break;
    case 2: // z-axis
      npos = pos + glm::vec3(0, 0, xzdist);
      break;
    }
  } while (check_collision(cubePositions, npos));

  // npos = glm::vec3(sin(pos.y + 1), pos.y + 1, 0.0f);

  cubePositions.push_back(npos);

  /* Branch */
  if (rand() % 10 == 3)
    bonsai(npos, cubePositions, (int)(growth / 3));

  /* Recursive Case */
  bonsai(npos, cubePositions, growth - 1);
}

/* Bonsai function */
void bonsai1(glm::vec3 pos, vector<glm::vec3> &cubePositions, int growth,
             int xdir, int zdir) {
  /* Base case */
  if (growth == 0)
    return;

  /* Growth in a singular direction */
  glm::vec3 npos = pos;

  int i = 3;
  while (i > 0) {
    if (rand() % 2 == 0) {
      npos += glm::vec3(xdir * (rand() % 2), 0, 0);
    } else {
      npos += glm::vec3(0, 0, zdir * (rand() % 2));
    }
    if (check_touching(cubePositions, npos) > 2)
      return;
    cubePositions.push_back(npos);
    i--;
  }

  /* Attach cubes */
  npos += glm::vec3(0, 1, 0);
  cubePositions.push_back(npos);

  /* Branch */
  if (growth % 3 == 0 && rand() % 5 == 0) {
    int nxdir = xdir * -1 * (rand() % 2), nzdir = zdir * -1 * (rand() % 2);
    if (nxdir || nzdir)
      bonsai1(npos, cubePositions, (int)(growth), nxdir, nzdir);
  }
  /* Recursive Case */
  bonsai1(npos, cubePositions, growth - 1, xdir, zdir);
}

/* Function: checks if cube already exists */
int check_collision(vector<glm::vec3> cubePositions, glm::vec3 pos) {
  for (unsigned int i = 0; i < cubePositions.size(); i++) {
    if (cubePositions[i] == pos)
      return 1; // collision
  }
  return 0; // no collision
}

/* Function: checks if cube touchs any adjacent blocks */
int check_touching(vector<glm::vec3> cube_positions, glm::vec3 pos) {
  return (check_collision(cube_positions, pos + glm::vec3(1, 0, 0)) +
          check_collision(cube_positions, pos + glm::vec3(-1, 0, 0)) +
          check_collision(cube_positions, pos + glm::vec3(0, 1, 0)) +
          check_collision(cube_positions, pos + glm::vec3(0, 0, 1)) +
          check_collision(cube_positions, pos + glm::vec3(0, 0, -1)));
}