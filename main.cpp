
#include <iostream>
#include "src/World.h"
#include "src/Common.h"
#include <string.h>

using namespace std;


int main(int args, char** argv) {

  
  Common::init_binding_matrix(897685687);
  printf("Init binding matrix\n");

  World* world = new World(32, 32, 897986875);
  printf("Create World\n");

  world->init_environment();
  printf("Initialize environment\n");

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


  //int d;
  //scanf("%d",&d);
  return 0;
}
