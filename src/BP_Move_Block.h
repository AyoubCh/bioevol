//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_MOVE_BLOCK_H
#define PDC_EVOL_MODEL_BP_MOVE_BLOCK_H


#include <vector>
#include <cstdio>
#include <cstdlib>
#include "BP_Move.h"

class BP_Move_Block {
 public:
    BP_Move_Block(BP_Move_Block* bp_move) {
      for (auto move : bp_move->bp_move_list_) {
        bp_move_list_.push_back(new BP_Move(move));
      }
    };

    BP_Move_Block() {
    };

    ~BP_Move_Block() {
      for (auto move : bp_move_list_) {
        delete move;
      }
    }


    std::vector<BP_Move*> bp_move_list_;

};


#endif //PDC_EVOL_MODEL_BP_MOVE_BLOCK_H
