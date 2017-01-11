//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_H
#define PDC_EVOL_MODEL_BP_H


#include "BP_Protein_Block.h"
#include "BP_Pump_Block.h"
#include "BP_Move_Block.h"
//#include <mutex>
#include <cassert>

class BP {
 public:
    enum  BP_Type {
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
    
    enum class TypeOP{
		UP,DOWN,READ
	};

    /**
     * Generic constructor (i.e. BP without args)
     */
    BP(int type) { type_ = type;}

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

    BP* clone(){
		editReferenced(TypeOP::UP);
        return this;
    }


    void del(){
        if( editReferenced(TypeOP::READ) == 0)
            return delete this;
        else {
			editReferenced(TypeOP::DOWN);
		}
        assert(editReferenced(TypeOP::READ)>=0);
    }

	long editReferenced(TypeOP op){
		long value;
        #pragma omp critical
		{
			if(op==TypeOP::UP){
				referenced ++;
			} else if(op==TypeOP::DOWN) {
				referenced --;
			} else {
				value = referenced;
			}
		}
		return value;
	}

	
    ~BP() {
      assert(referenced==0);
      delete protein_block_;
      delete pump_block_;
      delete move_block_;
    }

    int type_;

    float binding_pattern_;
    float concentration_;

    BP_Pump_Block* pump_block_ = nullptr;
    BP_Move_Block* move_block_ = nullptr;

    BP_Protein_Block* get_protein_block_() const {return protein_block_;}

    long referenced =  0;


private :
    BP_Protein_Block* protein_block_ = nullptr;


    //static std::mutex g_pages_mutex;
};


#endif //PDC_EVOL_MODEL_BP_H
