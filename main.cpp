
#include <iostream>
#include "src/World.h"
#include "src/Common.h"
#include <string.h>
	
using namespace std;

clock_t LastProfilingClock=clock();

inline float profiling (const char *s, clock_t *whichClock=NULL)
{
    if (whichClock==NULL)
        whichClock=&LastProfilingClock;

    clock_t newClock=clock();
    float res = (float) (newClock-*whichClock) / (float) CLOCKS_PER_SEC;
    if (s!=NULL)
        std::cerr << "Time: " << s << ": " << res << std::endl;
    *whichClock = newClock;
    return res;
}

int main(int args, char** argv) {

  
  bool test = false;
  bool cuda = false;
  for (int i = 1; i < args; ++i) {
    if (!strcmp("-t", argv[i])) test = true;
    else if (!strcmp("-c", argv[i])) cuda = true;
  }
  clock_t initMatTime;
  if (test) initMatTime = clock();
  if (cuda) Common::init_binding_matrix_gpu(897685687);
  else Common::init_binding_matrix(897685687);
  if (test) profiling("Init binding matrix", &initMatTime);
  else printf("Init binding matrix\n");

  World* world = new World(32, 32, 897986875);
  printf("Create World\n");

  world->init_environment();
  printf("Initialize environment\n");

  if (test) {
    world->test_mutate();
    profiling("Test total");
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
