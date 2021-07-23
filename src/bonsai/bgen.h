/* Collection of functions for generating Bonsai Tree
 * -- Hao X. July 2021
 */

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

const unsigned int MAX_XZ_GROWTH = 4;
const unsigned int BRANCH_COOLDOWN = 2;

// function declarations -------------------------------------------------------
void generateBonsai(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth, int xdir,
                    int zdir);
int checkCollision(std::vector<glm::vec3> branchPositions, glm::vec3 pos);
int checkTouching(std::vector<glm::vec3> cube_positions, glm::vec3 pos);

// functions -------------------------------------------------------------------
// recursively generates a voxel-based bonsai tree */
void generateBonsai(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth, int xdir,
                    int zdir) {
  if (growth == 0) { // base case
    leafPositions.push_back(pos + glm::vec3(0, 1, 0));
    return;
  } else {
    glm::vec3 npos = pos;

    // randomly calculate horizontal movement
    for (unsigned int i = 0; i < MAX_XZ_GROWTH; i++) {
      if (rand() % 2 == 0)
        npos += glm::vec3(xdir * (rand() % 2), 0, 0);
      else
        npos += glm::vec3(0, 0, zdir * (rand() % 2));

      // prevents self-collisions
      if (checkTouching(branchPositions, npos) > 2)
        return;
      branchPositions.push_back(npos);
    }
    npos += glm::vec3(0, 1, 0);
    branchPositions.push_back(npos);

    // (random) potentially makes a branch growth iterations
    if (growth % BRANCH_COOLDOWN == 0 && rand() % 4 == 0) {
      int nxdir = xdir * -1 * (rand() % 2), nzdir = zdir * -1 * (rand() % 2);
      if (nxdir || nzdir)
        generateBonsai(npos, branchPositions, leafPositions, growth - 1, nxdir,
                       nzdir);
    }

    /* recursive case */
    generateBonsai(npos, branchPositions, leafPositions, growth - 1, xdir,
                   zdir);
  }
}

// checks to see if a voxel exists at the given position
int checkCollision(std::vector<glm::vec3> branchPositions, glm::vec3 pos) {
  for (unsigned int i = 0; i < branchPositions.size(); i++) {
    if (branchPositions[i] == pos)
      return 1; // collision
  }
  return 0; // no collision
}

// returns the number of a blocks adjacent to a position
int checkTouching(std::vector<glm::vec3> cube_positions, glm::vec3 pos) {
  return (checkCollision(cube_positions, pos + glm::vec3(1, 0, 0)) +
          checkCollision(cube_positions, pos + glm::vec3(-1, 0, 0)) +
          checkCollision(cube_positions, pos + glm::vec3(0, 1, 0)) +
          checkCollision(cube_positions, pos + glm::vec3(0, 0, 1)) +
          checkCollision(cube_positions, pos + glm::vec3(0, 0, -1)));
}