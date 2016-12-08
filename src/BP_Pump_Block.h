//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_PUMP_BLOCK_H
#define PDC_EVOL_MODEL_BP_PUMP_BLOCK_H


#include <vector>
#include "BP_Pump.h"

class BP_Pump_Block {
 public:
    BP_Pump_Block(BP_Pump_Block* bp_prot) {
      for (auto pump : bp_prot->bp_pump_list_) {
        bp_pump_list_.push_back(new BP_Pump(pump));
      }
    }

    void clone(BP_Pump_Block* bp_prot) {
        for (auto pump : bp_prot->bp_pump_list_) {
          bp_pump_list_.push_back(new BP_Pump(pump));
        }
    }

    BP_Pump_Block() {
    }

    ~BP_Pump_Block() {
      for (auto pump : bp_pump_list_) {
        delete pump;
      }
    }

    std::vector<BP_Pump*> bp_pump_list_;
    long referenced_by = 0;
};


#endif //PDC_EVOL_MODEL_BP_PUMP_BLOCK_H
