/* Collection of functions for generating Bonsai Tree
 * -- Hao X. July 2021
 */

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// constants -------------------------------------------------------------------
const unsigned int MAX_XZ_GROWTH = 3;
const unsigned int BRANCH_COOLDOWN = 2;

const int MAX_POT_DEPTH = 4;
const int POT_RADIUS = 3;

int LEAF_HEIGHT = 3;
int LEAF_RADIUS = 6;

// bonsai max growth
unsigned int BONSAI_GROWTH = 8;
unsigned int BONSAI_BRANCHES_TIERS = 4;

// function declarations -------------------------------------------------------
void createBonsai(std::vector<glm::vec3> &branchPositions,
                  std::vector<glm::vec3> &leafPositions,
                  std::vector<glm::vec3> &potPositions,
                  std::vector<glm::vec3> &soilPositions);

void generateBonsai(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir);
void generateBranch(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir);
void generateLeaves(glm::vec3 pos, std::vector<glm::vec3> &leafPositions,
                    int height, int radius);
void generatePot(glm::vec3 origin, std::vector<glm::vec3> &potPositions,
                 int depth);
void generateSoil(glm::vec3 pos, std::vector<glm::vec3> &soilPositions,
                  int radius);

int checkCollision(std::vector<glm::vec3> branchPositions, glm::vec3 pos);
int checkTouching(std::vector<glm::vec3> cube_positions, glm::vec3 pos);

// functions -------------------------------------------------------------------
// bonsai helper function
void createBonsai(std::vector<glm::vec3> &branchPositions,
                  std::vector<glm::vec3> &leafPositions,
                  std::vector<glm::vec3> &potPositions,
                  std::vector<glm::vec3> &soilPositions) {

  // call bonsai generation with random start direction
  int xdir = rand() % 3 - 1, zdir = rand() % 3 - 1;
  generateBonsai(glm::vec3(0, 0, 0), branchPositions, leafPositions,
                 BONSAI_GROWTH, BONSAI_BRANCHES_TIERS, xdir, zdir);

  // call pot generation
  generatePot(glm::vec3(0, -1, 0), potPositions, 0);

  // generate soil
  generateSoil(glm::vec3(0, -1, 0), soilPositions, POT_RADIUS);
}

// recursively generates a voxel-based bonsai tree */
void generateBonsai(glm::vec3 pos, std::vector<glm::vec3> &branchPositions,
                    std::vector<glm::vec3> &leafPositions, int growth,
                    int branchTier, int xdir, int zdir) {

  if (branchTier == 0) { // base case
    generateLeaves(pos, leafPositions, LEAF_HEIGHT, LEAF_RADIUS);
    return;
  } else if (growth == 0) { // split into smaller branches
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

      // prevents self-collisions
      if (checkTouching(branchPositions, npos) > 2)
        return;
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

// generate branchs new dir for a branch
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

// recursively generates bonsai leaves
void generateLeaves(glm::vec3 pos, std::vector<glm::vec3> &leafPositions,
                    int height, int radius) {
  if (!height)
    return;
  else {
    for (int x = -radius; x <= radius; x++) {
      for (int z = -radius; z <= radius; z++) {
        if (x * x + z * z <= radius * radius)
          if ((x != 0 || z != 0) && rand() % (abs(x) + abs(z)) == 0)
            leafPositions.push_back(pos + glm::vec3(x, 1, z));
      }
    }
  }
  generateLeaves(pos + glm::vec3(0, 1, 0), leafPositions, height - 1,
                 radius - 2);
}

// recursively generates a spherical pot
void generatePot(glm::vec3 pos, std::vector<glm::vec3> &potPositions,
                 int depth) {
  int y = pos.y, radius = -0.5 * ((y - 1) * (y + 6));
  std::cout << pos.y << " " << radius << std::endl;
  if (depth == MAX_POT_DEPTH)
    return;
  else {
    for (int x = -radius; x <= radius; x++) {
      for (int z = -radius; z <= radius; z++) {
        if (x * x + z * z <= radius * radius)
          potPositions.push_back(pos + glm::vec3(x, 0, z));
      }
    }
    generatePot(pos + glm::vec3(0, -1, 0), potPositions, depth + 1);
  }
}

// generate soil
void generateSoil(glm::vec3 pos, std::vector<glm::vec3> &soilPositions,
                  int radius) {
  for (int x = -radius; x <= radius; x++) {
    for (int z = -radius; z <= radius; z++) {
      if (x * x + z * z <= radius * radius)
        soilPositions.push_back(pos + glm::vec3(x, 0, z));
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