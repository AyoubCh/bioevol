//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_RNA_H
#define PDC_EVOL_MODEL_RNA_H


#include <vector>
#include "BP.h"

class RNA {
 public:
    /**
     * Default constructor
     */
    RNA(float binding_pattern,
        float concentration_base) {
      binding_pattern_ = binding_pattern;
      concentration_base_ = concentration_base;
      current_concentration_ = concentration_base;
    }

    /*
     * Cloning constructor
     */
    RNA(RNA* rna) {

      for (auto rna_l : rna->bp_list_) {
        //bp_list_.push_back(new BP(rna_l));
         bp_list_.push_back(rna_l->clone());
      }

      binding_pattern_ = rna->binding_pattern_;
      concentration_base_ = rna->concentration_base_;
      current_concentration_ = rna->current_concentration_;
    }

    ~RNA() {
      for (auto bp : bp_list_) {
        //delete bp;
        bp-> del();
      }
    }

    std::vector<BP*> bp_list_;

    float binding_pattern_;
    float concentration_base_;
    float current_concentration_;

};


#endif //PDC_EVOL_MODEL_RNA_H
