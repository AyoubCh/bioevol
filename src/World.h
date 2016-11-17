//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_WORLD_H
#define PDC_EVOL_MODEL_WORLD_H


//#include "GridCell.h"

#include <cstdint>
#include <random>
#include <iostream>
#include <fstream>

class GridCell;

class World {
 public:
    World(int width, int height, uint32_t seed);
    ~World() { statfile_best_.close(); statfile_mean_.close(); }
    void random_population();
    void test_mutate();

    int width_;
    int height_;

    uint32_t global_seed_;
    std::mt19937 global_gen_;


    GridCell** grid_cell_;

    int time_;

    float min_fitness_,max_fitness_;
    int death_;
    int new_mutant_;
    std::ofstream statfile_best_;
    std::ofstream statfile_mean_;

    void run_evolution();

    void evolution_step();

    void init_environment();

    void stats();
};


#endif //PDC_EVOL_MODEL_WORLD_H
