/* Abstract Camera Class:
 * Processes input and calculates corresponding Euler angles, vectors and
 * matrices for OpenGL.
 * -- Hao X. July 2021
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// constants & enums -----------------------------------------------------------
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };
enum Camera_Control { USER, ROTATING };

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float RADIUS = 70.0f;

class Camera {
public:
  // attributes ----------------------------------------------------------------
  // camera
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // euler angles
  float Yaw;
  float Pitch;

  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;
  Camera_Control Mode;

  // constructors --------------------------------------------------------------
  // 1. construct using vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Mode = ROTATING;
    updateCameraVectors();
  }

  // 2. construct using scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    Mode = ROTATING;
    updateCameraVectors();
  }

  // functions -----------------------------------------------------------------
  // returns the view matrix calculated using Euler angles and the LookAt matrix
  glm::mat4 GetViewMatrix() {
    glm::mat4 view;

    switch (Mode) {
    case USER: // rotating camera
      view = glm::lookAt(Position, Position + Front, Up);
      break;

    case ROTATING: // user-controlled camera
      float camX = sin(0.25 * glfwGetTime()) * RADIUS;
      float camZ = cos(0.25 * glfwGetTime()) * RADIUS;
      float camY = sin(0.25 * glfwGetTime()) * RADIUS / 4.0 + 30.0f;
      glm::vec3 centre = glm::vec3(0.0f, 20.0f, 0.0f);
      view = glm::lookAt(glm::vec3(camX, camY, camZ), centre, Up);
      break;
    }
    return view;
  }

  // sets the position
  void setMode() {
    switch (Mode) {
    case ROTATING:
      Mode = USER;
      break;
    case USER:
      Yaw = YAW;
      Pitch = PITCH;
      updateCameraVectors();
      Mode = ROTATING;
      break;
    }
  }

  // processes input received from any keyboard-like input system in enum form
  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
      Position += Front * velocity;
    if (direction == BACKWARD)
      Position -= Front * velocity;
    if (direction == LEFT)
      Position -= Right * velocity;
    if (direction == RIGHT)
      Position += Right * velocity;
  }

  // processes input received from a mouse input system
  // expects offset in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event
  void ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if (Zoom > 45.0f)
      Zoom = 45.0f;
  }

private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(
        Front, WorldUp)); // normalize the vectors, because their length gets
                          // closer to 0 the more you look up or down which
                          // results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
  }
};
#endif
