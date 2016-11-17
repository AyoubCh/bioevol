//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_GRIDCELL_H
#define PDC_EVOL_MODEL_GRIDCELL_H

#include <random>
#include <unordered_map>
#include "World.h"
#include "Protein.h"
#include "Common.h"

class Organism;
class GridCell {
 public:
    GridCell(World* world, int x, int y, uint32_t seed) {
      world_ = world;
      x_=x; y_=y;
      float_gen_.seed(seed);

    };

    World* world_;
    std::unordered_map<float,Protein*> protein_list_map_;
    Organism* organism_ = nullptr;
    float environment_target[Common::Metabolic_Error_Precision];

    int x_;
    int y_;

    std::mt19937 float_gen_;

    void diffuse_protein();
    void degrade_protein();
    void push_new_protein(std::vector<Protein*> protein_list);
};


#endif //PDC_EVOL_MODEL_GRIDCELL_H
