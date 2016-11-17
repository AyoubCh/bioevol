//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_MOVE_H
#define PDC_EVOL_MODEL_MOVE_H


class Move {
 public:
    Move(int distance, int retry) {
      distance_ = distance;
      retry_ = retry;
    }

    Move(Move* move) {
      distance_ = move->distance_;
      retry_ = move->retry_;
    }

    int distance_ = -1;
    int retry_ = -1;

};


#endif //PDC_EVOL_MODEL_MOVE_H
