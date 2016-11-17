//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_PROTEIN_H
#define PDC_EVOL_MODEL_BP_PROTEIN_H


class BP_Protein {
 public:
    enum class BP_Protein_Type {
        ARITHMETIC_OPERATOR = 0,
        FLOAT_NUMBER = 1
    };

    enum class Arithmetic_Operator_Type {
        ADD = 0,
        MULTIPLY = 1,
        MODULO = 2,
        POWER = 3
    };

    BP_Protein(int type, int op) {
      type_ = type;
      op_ = op;
    };

    BP_Protein(int type, float number) {
      type_  = type;
      number_ = number;
    }

    BP_Protein(BP_Protein* bp_protein) {
      type_ = bp_protein->type_;
      op_ = bp_protein->op_;
      number_ = bp_protein->number_;
    }

    int type_;
    int op_ = -1;
    float number_ = -1;
};


#endif //PDC_EVOL_MODEL_BP_PROTEIN_H
