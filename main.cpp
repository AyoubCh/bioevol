#include <iostream>
#include "src/World.h"
#include "src/Common.h"
#include <string.h>

using namespace std;

int main(int args, char** argv) {
  printf("Init binding matrix\n");
  Common::init_binding_matrix(897685687);

  printf("Create World\n");
  World* world = new World(32, 32, 897986875);

  printf("Initialize environment\n");
  world->init_environment();

  bool test = false;
  if (args == 2 && !strcmp("-t", argv[1])) test = true;
  if (test) {
    world->test_mutate();
  } else {
    printf("Initialize random population\n");
    world->random_population();

    printf("Run evolution\n");
    world->run_evolution();
  }
}
