//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_H
#define PDC_EVOL_MODEL_BP_H


#include "BP_Protein_Block.h"
#include "BP_Pump_Block.h"
#include "BP_Move_Block.h"

class BP {
 public:
    enum class BP_Type {
        START_RNA = 0,
        END_RNA = 1,
        START_PROTEIN = 2,
        END_PROTEIN = 3,
        PROTEIN_BLOCK = 4,
        START_PUMP = 5,
        END_PUMP = 6,
        PUMP_BLOCK = 7,
        START_MOVE = 8,
        END_MOVE = 9,
        MOVE_BLOCK = 10
    };

    /**
     * Generic constructor (i.e. BP without args)
     */
    BP(int type) { type_ = type; }

    /**
     * Constructor for BP Protein Block
     */
    BP(int type, BP_Protein_Block* protein_block) {
      type_ = type;
      protein_block_ = protein_block;
    }

    /**
     * Constructor for BP Pump Block
     */
    BP(int type, BP_Pump_Block* pump_block) {
      type_ = type;
      pump_block_ = pump_block;
    }

    /**
     * Constructor for BP Move Block
     */
    BP(int type, BP_Move_Block* move_block) {
      type_ = type;
      move_block_ = move_block;
    }

    /**
     * Constructor for START_RNA BP
     */
    BP(int type, float binding_pattern, float concentration) {
      type_ = type;
      binding_pattern_ = binding_pattern;
      concentration_ = concentration;
    }

    /**
     * Constructor for START_PROTEIN BP
     */
    BP(int type, float binding_pattern) {
      type_ = type;
      binding_pattern_ = binding_pattern;
    }


    BP(BP* bp) {
      type_ = bp->type_;
      binding_pattern_ = bp->binding_pattern_;
      concentration_ = bp->concentration_;

      if (bp->protein_block_ != nullptr) {
        //protein_block_ = new BP_Protein_Block(bp->get_protein_block_());
        protein_block_ = bp->get_protein_block_();
      } else if (bp->pump_block_ != nullptr) {
        //pump_block_ = new BP_Pump_Block(bp->pump_block_);
         pump_block_ = bp->pump_block_;
      } else if (bp->move_block_ != nullptr) {
        //move_block_ = new BP_Move_Block(bp->move_block_);
        move_block_ = bp->move_block_;
      }
    }

    ~BP() {
      /*delete protein_block_;
      delete pump_block_;
      delete move_block_;*/
    }

    int type_;

    float binding_pattern_;
    float concentration_;

    BP_Pump_Block* pump_block_ = nullptr;
    BP_Move_Block* move_block_ = nullptr;

    BP_Protein_Block* get_protein_block_() const {return protein_block_;}

private :
    BP_Protein_Block* protein_block_ = nullptr;
};


#endif //PDC_EVOL_MODEL_BP_H
