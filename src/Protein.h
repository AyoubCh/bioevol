//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_PROTEIN_H
#define PDC_EVOL_MODEL_PROTEIN_H


class Protein {
 public:
    enum class Protein_Type {
        FITNESS = 0,
        TF = 1,
        POISON = 2,
        ANTIPOISON = 3
    };

    Protein(int type, float binding_pattern, float value) {
      type_ = type;
      binding_pattern_ = binding_pattern;
      value_ = value;
      concentration_ = 0;
    }

    Protein(Protein* prot) {
      type_ = prot->type_;
      binding_pattern_ = prot->binding_pattern_;
      value_ = prot->value_;
      concentration_ = prot->concentration_;
    }



    float concentration_up(float edit = 0){
        float value;
        #pragma omp critical(concentration)
        {
            concentration_ += edit;
            value = concentration_;
        }
        return value;
    }

    void set_concentration(float edit){
        #pragma omp critical(concentration)
        {
            concentration_ = edit;
        }
    }

    int type_ = -1;
    float binding_pattern_ = -1;
    float value_ = -1;

    float get_value(){ return value_;}

private :
     float concentration_ = -1;
};


#endif //PDC_EVOL_MODEL_PROTEIN_H
