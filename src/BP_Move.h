//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_MOVE_H
#define PDC_EVOL_MODEL_BP_MOVE_H


class BP_Move {
 public:
    BP_Move(int distance, int retry) {
      distance_ = distance;
      retry_ = distance;
    };

    BP_Move(BP_Move* bp_pump) {
      retry_ = bp_pump->retry_;
      distance_ = bp_pump->distance_;
    };

    int distance_;
    int retry_;
};


#endif //PDC_EVOL_MODEL_BP_MOVE_H
