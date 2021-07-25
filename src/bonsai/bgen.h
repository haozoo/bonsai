/* Bonsai Class:
 * -- Hao X. July 2021
 */

#ifndef BONSAI_H
#define BONSAI_H

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

class BonsaiTree {
public:
  // attributes ----------------------------------------------------------------
  std::vector<glm::vec3> BranchPositions;
  std::vector<glm::vec3> LeafPositions;
  std::vector<glm::vec3> PotPositions;
  std::vector<glm::vec3> SoilPositions;

  // constructor ---------------------------------------------------------------
  BonsaiTree() {
    // call bonsai generation with random start direction
    int xdir = rand() % 3 - 1, zdir = rand() % 3 - 1;
    generateBonsai(glm::vec3(0, 0, 0), BONSAI_GROWTH, BONSAI_BRANCHES_TIERS,
                   xdir, zdir);

    // call pot generation
    generatePot(glm::vec3(0, -1, 0), 0);

    // generate soil
    generateSoil(glm::vec3(0, -1, 0), POT_RADIUS);
  }

private:
  // recursively generates a voxel-based bonsai tree */
  void generateBonsai(glm::vec3 pos, int growth, int branchTier, int xdir,
                      int zdir) {

    if (branchTier == 0) { // base case
      generateLeaves(pos, LEAF_HEIGHT, LEAF_RADIUS);
      return;
    } else if (growth == 0) { // split into smaller branches
      generateBonsai(pos, pow(2, (branchTier - 1)), branchTier - 1, xdir, zdir);
    } else {
      glm::vec3 npos = pos;

      // randomly calculate horizontal movement
      for (unsigned int i = 0; i < MAX_XZ_GROWTH; i++) {
        if (rand() % 2 == 0)
          npos += glm::vec3(xdir * (rand() % 2), 0, 0);
        else
          npos += glm::vec3(0, 0, zdir * (rand() % 2));

        // prevents self-collisions
        if (checkTouching(BranchPositions, npos) > 2)
          return;
        BranchPositions.push_back(npos);
      }
      npos += glm::vec3(0, 1, 0);
      BranchPositions.push_back(npos);

      // (random) potentially makes a branch growth iterations
      if (growth % BRANCH_COOLDOWN == 0 && rand() % branchTier == 0) {
        generateBranch(npos, growth, branchTier, xdir, zdir);
      }

      /* recursive case */
      generateBonsai(npos, growth - 1, branchTier, xdir, zdir);
    }
  }

  // generate branchs new dir for a branch
  void generateBranch(glm::vec3 pos, int growth, int branchTier, int xdir,
                      int zdir) {
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
      generateBonsai(pos, pow(2, (branchTier - 1)), branchTier - 1, nxdir,
                     nzdir);
  }

  // recursively generates bonsai leaves
  void generateLeaves(glm::vec3 pos, int height, int radius) {
    if (!height)
      return;
    else {
      for (int x = -radius; x <= radius; x++) {
        for (int z = -radius; z <= radius; z++) {
          if (x * x + z * z <= radius * radius)
            if ((x != 0 || z != 0) && rand() % (abs(x) + abs(z)) == 0)
              LeafPositions.push_back(pos + glm::vec3(x, 1, z));
        }
      }
    }
    generateLeaves(pos + glm::vec3(0, 1, 0), height - 1, radius - 2);
  }

  // recursively generates a spherical pot
  void generatePot(glm::vec3 pos, int depth) {
    int y = pos.y, radius = -0.5 * ((y - 1) * (y + 6));
    std::cout << pos.y << " " << radius << std::endl;
    if (depth == MAX_POT_DEPTH)
      return;
    else {
      for (int x = -radius; x <= radius; x++) {
        for (int z = -radius; z <= radius; z++) {
          if (x * x + z * z <= radius * radius)
            PotPositions.push_back(pos + glm::vec3(x, 0, z));
        }
      }
      generatePot(pos + glm::vec3(0, -1, 0), depth + 1);
    }
  }

  // generate soil
  void generateSoil(glm::vec3 pos, int radius) {
    for (int x = -radius; x <= radius; x++) {
      for (int z = -radius; z <= radius; z++) {
        if (x * x + z * z <= radius * radius)
          SoilPositions.push_back(pos + glm::vec3(x, 0, z));
      }
    }
  }

  // utility -------------------------------------------------------------------
  // checks to see if a voxel exists at the given position
  int checkCollision(std::vector<glm::vec3> cube_positions, glm::vec3 pos) {
    for (unsigned int i = 0; i < cube_positions.size(); i++) {
      if (cube_positions[i] == pos)
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
};
#endif
