/* Collection of functions for generating Bonsai Tree
 * -- Hao X. July 2021
 */

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// constants -------------------------------------------------------------------
const unsigned int MAX_XZ_GROWTH = 4;
const unsigned int BRANCH_COOLDOWN = 2;

const int POT_HEIGHT = 3;
const int POT_RADIUS = 3;

int LEAF_HEIGHT = 3;
int LEAF_RADIUS = 6;

// function declarations -------------------------------------------------------
void generateBonsai(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir);
void generateBranch(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir);
void generateLeaves(glm::vec3 pos, std::vector<glm::vec3> &leafPositions,
                    int leafHeight, int leafRadius);
void generatePot(glm::vec3 origin, std::vector<glm::vec3> &potPositions);

int checkCollision(std::vector<glm::vec3> branchPositions, glm::vec3 pos);
int checkTouching(std::vector<glm::vec3> cube_positions, glm::vec3 pos);

// functions -------------------------------------------------------------------
// recursively generates a voxel-based bonsai tree */
void generateBonsai(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir) {
  std::cout << growth << " " << branchTier << std::endl;
  if (branchTier == 0) { // base case
    generateLeaves(pos, leafPositions, LEAF_HEIGHT, LEAF_RADIUS);
    return;
  } else if (growth == 0) {
    generateBonsai(pos, branchPositions, leafPositions,
                   pow(2, (branchTier - 1)), branchTier - 1, xdir, zdir);
  } else {
    glm::vec3 npos = pos;

    // randomly calculate horizontal movement
    for (unsigned int i = 0; i < MAX_XZ_GROWTH; i++) {
      if (rand() % 2 == 0)
        npos += glm::vec3(xdir * (rand() % 2), 0, 0);
      else
        npos += glm::vec3(0, 0, zdir * (rand() % 2));

      // // prevents self-collisions
      // if (checkTouching(branchPositions, npos) > 2)
      //   return;
      branchPositions.push_back(npos);
    }
    npos += glm::vec3(0, 1, 0);
    branchPositions.push_back(npos);

    // (random) potentially makes a branch growth iterations
    if (growth % BRANCH_COOLDOWN == 0 && rand() % branchTier == 0) {
      generateBranch(npos, branchPositions, leafPositions, growth, branchTier,
                     xdir, zdir);
    }

    /* recursive case */
    generateBonsai(npos, branchPositions, leafPositions, growth - 1, branchTier,
                   xdir, zdir);
  }
}

// generate branch
void generateBranch(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir) {
  // makes a branch with a new direction
  int nxdir, nzdir;

  if (xdir == 0) // xdir
    nxdir = (rand() % 2 == 0) ? -1 : 1;
  else
    nxdir = xdir * -1 * (rand() % 2);

  if (zdir == 0) // zdir
    nzdir = (rand() % 2 == 0) ? -1 : 1;
  else
    nzdir = zdir * -1 * (rand() % 2);

  if (nxdir || nzdir)
    generateBonsai(pos, branchPositions, leafPositions,
                   pow(2, (branchTier - 1)), branchTier - 1, nxdir, nzdir);
}

// generate bonsai leaves
void generateLeaves(glm::vec3 pos, std::vector<glm::vec3> &leafPositions,
                    int leafHeight, int leafRadius) {
  if (!leafHeight)
    return;
  else {
    for (int x = -leafRadius; x <= leafRadius; x++) {
      for (int z = -leafRadius; z <= leafRadius; z++) {
        if (x * x + z * z <= leafRadius * leafRadius)
          leafPositions.push_back(pos + glm::vec3(x, 1, z));
      }
    }
  }
  generateLeaves(pos + glm::vec3(0, 1, 0), leafPositions, leafHeight - 1,
                 leafRadius - 2);
}

// general a cylindrical pot
void generatePot(glm::vec3 origin, std::vector<glm::vec3> &potPositions) {
  for (int x = -POT_RADIUS; x <= POT_RADIUS; x++) {
    for (int y = origin.y; y > -POT_HEIGHT; y--) {
      for (int z = -POT_RADIUS; z <= POT_RADIUS; z++) {
        if (x * x + z * z <= POT_RADIUS * POT_RADIUS)
          potPositions.push_back(origin + glm::vec3(x, y, z));
      }
    }
  }
}

// utility ---------------------------------------------------------------------
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