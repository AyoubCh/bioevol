//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_PROTEIN_BLOCK_H
#define PDC_EVOL_MODEL_BP_PROTEIN_BLOCK_H


#include <vector>
#include <cstdio>
#include <cstdlib>
#include "BP_Protein.h"

class BP_Protein_Block {
 public:
    BP_Protein_Block(BP_Protein_Block* bp_prot) {
      for (auto prot : bp_prot->bp_prot_list_) {
        bp_prot_list_.push_back(new BP_Protein(prot));
      }
    };

    BP_Protein_Block() {

    };

    ~BP_Protein_Block() {
      for (auto prot : bp_prot_list_) {
        if (prot == nullptr) { printf("NO:");exit(-1);}
        delete prot;
      }
    }

    std::vector<BP_Protein*> bp_prot_list_;
};


#endif //PDC_EVOL_MODEL_BP_PROTEIN_BLOCK_H
