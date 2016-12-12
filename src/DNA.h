//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_DNA_H
#define PDC_EVOL_MODEL_DNA_H


#include "GridCell.h"
#include "BP.h"

class DNA {
 public:
    DNA(DNA* dna) {
      for (auto bp : dna->bp_list_) {
        //bp_list_.push_back(new BP(bp));
        bp_list_.push_back(bp->clone());
      }
    } // Clone

    DNA(GridCell* grid_cell); // Random one
    
    ~DNA() {
         for (auto bp : bp_list_)
         {
           //delete bp;
             bp-> del();
         }
         bp_list_.clear();
    }

    std::vector<BP*> bp_list_;

    void insert_a_BP(int pos, GridCell* grid_cell);
    void modify_bp(int pos, GridCell* grid_cell);

};


#endif //PDC_EVOL_MODEL_DNA_H
