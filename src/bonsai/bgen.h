#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

const unsigned int MAX_XZ_GROWTH = 3;

/* Function Declarations */
void bonsai(glm::vec3 pos, std::vector<glm::vec3> &cubePositions, int growth,
            int xdir, int zdir);

int check_collision(std::vector<glm::vec3> cubePositions, glm::vec3 pos);
int check_touching(std::vector<glm::vec3> cube_positions, glm::vec3 pos);

/* Recursive Bonsai Generation */
void bonsai(glm::vec3 pos, std::vector<glm::vec3> &cubePositions, int growth,
            int xdir, int zdir) {
  if (growth == 0)
    return;
  else {
    glm::vec3 npos = pos; // vector translation

    /* Bonsai horizontal growth per-iteration */
    for (unsigned int i = 0; i < MAX_XZ_GROWTH; i++) {
      if (rand() % 2 == 0) {
        npos += glm::vec3(xdir * (rand() % 2), 0, 0);
      } else {
        npos += glm::vec3(0, 0, zdir * (rand() % 2));
      }
      if (check_touching(cubePositions, npos) > 2)
        return;
      cubePositions.push_back(npos);
    }
    npos += glm::vec3(0, 1, 0);
    cubePositions.push_back(npos);

    /* Branch */
    if (growth % 3 == 0 && rand() % 5 == 0) {
      int nxdir = xdir * -1 * (rand() % 2), nzdir = zdir * -1 * (rand() % 2);
      if (nxdir || nzdir)
        bonsai(npos, cubePositions, growth - 1, nxdir, nzdir);
    }

    /* Recursive Case */
    bonsai(npos, cubePositions, growth - 1, xdir, zdir);
  }
}

/* Utility: checks if cube already exists */
int check_collision(std::vector<glm::vec3> cubePositions, glm::vec3 pos) {
  for (unsigned int i = 0; i < cubePositions.size(); i++) {
    if (cubePositions[i] == pos)
      return 1; // collision
  }
  return 0; // no collision
}

/* Utility: checks if cube touchs any adjacent blocks */
int check_touching(std::vector<glm::vec3> cube_positions, glm::vec3 pos) {
  return (check_collision(cube_positions, pos + glm::vec3(1, 0, 0)) +
          check_collision(cube_positions, pos + glm::vec3(-1, 0, 0)) +
          check_collision(cube_positions, pos + glm::vec3(0, 1, 0)) +
          check_collision(cube_positions, pos + glm::vec3(0, 0, 1)) +
          check_collision(cube_positions, pos + glm::vec3(0, 0, -1)));
}