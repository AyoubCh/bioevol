//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_BP_PUMP_H
#define PDC_EVOL_MODEL_BP_PUMP_H


class BP_Pump {
 public:
    BP_Pump(bool in_out, float start_range, float end_range, int speed) {
      in_out_ = in_out;
      start_range_ = start_range;
      end_range_ = end_range;
      speed_ = speed;
    }

    BP_Pump(BP_Pump* bp_pump) {
      in_out_ = bp_pump->in_out_;
      start_range_ = bp_pump->start_range_;
      end_range_ = bp_pump->end_range_;
      speed_ = bp_pump->speed_;
    }

    bool in_out_;
    float start_range_;
    float end_range_;
    int speed_;
};


#endif //PDC_EVOL_MODEL_BP_PUMP_H
