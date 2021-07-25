/* Bonsai Class:
 * Contains the algorithm for generating a bonsai using cubes
 * -- Hao X. July 2021
 */

#ifndef BONSAI_H
#define BONSAI_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// constants -------------------------------------------------------------------
// branch parameters
const unsigned int Y_GROWTH = 8;
const unsigned int MAX_XZ_GROWTH = 3;
const unsigned int BRANCH_COOLDOWN = 2;
const unsigned int BRANCHES_TIERS = 4;

// pot parameters
const int MAX_POT_DEPTH = 4;
const int POT_RADIUS = 3;

// leaf parameters
int LEAF_HEIGHT = 3;
int LEAF_RADIUS = 6;

class Bonsai {
public:
  // attributes ----------------------------------------------------------------
  std::vector<glm::vec3> BranchPositions;
  std::vector<glm::vec3> LeafPositions;
  std::vector<glm::vec3> PotPositions;
  std::vector<glm::vec3> SoilPositions;

  // constructor ---------------------------------------------------------------
  Bonsai() {
    // generate bonsai tree with xz axis directions (-1, 0 or 1)
    int xdir = rand() % 3 - 1, zdir = rand() % 3 - 1;
    generateTree(glm::vec3(0, 0, 0), Y_GROWTH, BRANCHES_TIERS, xdir, zdir);

    // generate pot
    generatePot(glm::vec3(0, -1, 0), 0);

    // generate soil
    generateSoil(glm::vec3(0, -1, 0), POT_RADIUS);
  }

private:
  // recursively generates a voxel-based bonsai tree ---------------------------
  // - generates branch cubes based on chance and a degenerating growth rate
  // - using rand() to generate numbers is sufficient as % the result
  // - to maintain a realistic branch structure, ea. branch is segmented into
  //   tiers wherein the lower the tier, the more likely it is to branch
  void generateTree(glm::vec3 pos, int growth, int tier, int xdir, int zdir) {

    // base case: on smallest branch -> now generate foliage
    if (tier == 0) {
      generateLeaves(pos, LEAF_HEIGHT, LEAF_RADIUS);
      return;

      // recursive case: branch tier finished growing, move to lower tier
    } else if (growth == 0) {
      generateTree(pos, pow(2, (tier - 1)), tier - 1, xdir, zdir);

      // recursive case: continue generating current tier
    } else {
      glm::vec3 npos = pos;

      // randomly generate horizontal movement, 1 axis at a time
      for (unsigned int i = 0; i < MAX_XZ_GROWTH; i++) {
        if (rand() % 2 == 0)
          npos += glm::vec3(xdir * (rand() % 2), 0, 0);
        else
          npos += glm::vec3(0, 0, zdir * (rand() % 2));
        BranchPositions.push_back(npos);
      }

      // lastly add upward movement
      npos += glm::vec3(0, 1, 0);
      BranchPositions.push_back(npos);

      // (random) chance to make a new branch depending on tier
      if (growth % BRANCH_COOLDOWN == 0 && rand() % tier == 0) {
        generateBranch(npos, growth, tier, xdir, zdir);
      }

      // continue making branch
      generateTree(npos, growth - 1, tier, xdir, zdir);
    }
  }

  // creates a new branch with a new direction and tier-proportionate growth
  void generateBranch(glm::vec3 pos, int growth, int tier, int xdir, int zdir) {
    int nxdir = chooseNewDirection(xdir), nzdir = chooseNewDirection(zdir);
    if (nxdir || nzdir)
      generateTree(pos, pow(2, (tier - 1)), tier - 1, nxdir, nzdir);
  }

  // recursively generates bonsai leaves
  void generateLeaves(glm::vec3 pos, int height, int radius) {
    if (!height)
      return;
    else {
      // create circular cross-section on xz plane
      for (int x = -radius; x <= radius; x++) {
        for (int z = -radius; z <= radius; z++) {
          if (x * x + z * z <= radius * radius)
            // the further away from the centre the less likely a leaf spawns
            if ((x != 0 || z != 0) && rand() % (abs(x) + abs(z)) == 0)
              LeafPositions.push_back(pos + glm::vec3(x, 1, z));
        }
      }
    }
    generateLeaves(pos + glm::vec3(0, 1, 0), height - 1, radius - 2);
  }

  // recursively generates a circular pot with xy curvature defined by quadratic
  void generatePot(glm::vec3 pos, int depth) {
    int y = pos.y, radius = -0.5 * ((y - 1) * (y + 6)); // calc radius using y
    if (depth == MAX_POT_DEPTH)
      return;
    else {
      // create circular cross-section on xz plane
      for (int x = -radius; x <= radius; x++) {
        for (int z = -radius; z <= radius; z++) {
          if (x * x + z * z <= radius * radius)
            PotPositions.push_back(pos + glm::vec3(x, 0, z));
        }
      }
      generatePot(pos + glm::vec3(0, -1, 0), depth + 1);
    }
  }

  // generate circular soil patch with centre position 'pos'
  void generateSoil(glm::vec3 pos, int radius) {
    for (int x = -radius; x <= radius; x++) {
      for (int z = -radius; z <= radius; z++) {
        if (x * x + z * z <= radius * radius)
          SoilPositions.push_back(pos + glm::vec3(x, 0, z));
      }
    }
  }

  // randomly choose a direction different to the previous for a an axis
  int chooseNewDirection(int dir) {
    // guard against 0 x/z direction to so branch doesn't degenerate to an
    // upward stick if we were to only flip the x/z direction
    int randint = rand() % 2;
    if (dir == 0)
      return (randint == 0) ? -1 : 1;
    else
      return dir * -1 * randint; // chance to flip direction
  }
};
#endif
