//
// Created by arrouan on 28/09/16.
//

#include "DNA.h"
#include "Common.h"

DNA::DNA(GridCell* grid_cell) {
// Create a random individual
  int bp_type;
  for (int ibp = 0 ; ibp < Common::DNA_base_length; ibp++) {
    std::uniform_int_distribution<uint32_t> dis(0,11);

    bp_type = dis(grid_cell->float_gen_);

    if (bp_type == (int) BP::BP_Type::PROTEIN_BLOCK) {
      BP_Protein_Block* bp_protein_block = new BP_Protein_Block();

      std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Protein_base_length+1);
      std::uniform_int_distribution<uint32_t> dis_block_type(0,2);
      std::uniform_int_distribution<uint32_t> dis_arith(0,4);
      std::uniform_real_distribution<float> dis_number(0, 1);

      int block_size = dis_block(grid_cell->float_gen_);
      int bp_prot_type;
      int bp_prot_arith;
      float bp_prot_number;


      for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
        bp_prot_type = dis_block_type(grid_cell->float_gen_);

        if (bp_prot_type == (int) BP_Protein::BP_Protein_Type::ARITHMETIC_OPERATOR) {
          bp_prot_arith = dis_arith(grid_cell->float_gen_);
          BP_Protein* bp_prot = new BP_Protein(bp_prot_type,bp_prot_arith);
          bp_protein_block->bp_prot_list_.push_back(bp_prot);
        } else {
          // Number
          bp_prot_number=dis_number(grid_cell->float_gen_);
          BP_Protein* bp_prot = new BP_Protein(bp_prot_type,bp_prot_number);
          bp_protein_block->bp_prot_list_.push_back(bp_prot);
        }
      }

      BP* bp_c = new BP((int)BP::BP_Type::PROTEIN_BLOCK,bp_protein_block);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::PUMP_BLOCK) {
      BP_Pump_Block* bp_pump_block = new BP_Pump_Block();

      std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Pump_base_length+1);
      std::uniform_int_distribution<uint32_t> dis_in_out(0,2);
      std::uniform_int_distribution<uint32_t> dis_speed(0,Common::Pump_Max_Speed);
      std::uniform_real_distribution<float> dis_number(0, 1);

      int block_size = dis_block(grid_cell->float_gen_);
      bool in_out;
      float start_range;
      float end_range;
      int speed;

      for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
        in_out = dis_in_out(grid_cell->float_gen_);
        start_range=dis_number(grid_cell->float_gen_);
        end_range=dis_number(grid_cell->float_gen_);
        speed=dis_speed(grid_cell->float_gen_);

        BP_Pump* pump = new BP_Pump(in_out,start_range,end_range,speed);
        bp_pump_block->bp_pump_list_.push_back(pump);
      }

      BP* bp_c = new BP((int)BP::BP_Type::PUMP_BLOCK,bp_pump_block);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::MOVE_BLOCK) {
      BP_Move_Block* bp_move_block = new BP_Move_Block();

      std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Move_base_length+1);
      std::uniform_int_distribution<uint32_t> dis_retry(0,Common::Max_Move_Retry+1);
      std::uniform_int_distribution<uint32_t> dis_distance(0,Common::Max_Move_Distance+1);

      int block_size = dis_block(grid_cell->float_gen_);
      int retry;
      int distance;

      for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
        retry = dis_retry(grid_cell->float_gen_);
        distance = dis_distance(grid_cell->float_gen_);

        BP_Move* move = new BP_Move(retry,distance);
        bp_move_block->bp_move_list_.push_back(move);
      }

      BP* bp_c = new BP((int)BP::BP_Type::MOVE_BLOCK,bp_move_block);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::START_RNA) {
      std::uniform_real_distribution<float> dis_binding(0, 1);
      std::uniform_real_distribution<float> dis_concentration(0, 1);

      float binding_pattern = dis_binding(grid_cell->float_gen_);
      float concentration = dis_concentration(grid_cell->float_gen_);

      BP* bp_c = new BP((int)BP::BP_Type::START_RNA,binding_pattern,concentration);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::START_PROTEIN) {
      std::uniform_real_distribution<float> dis_binding(0, 1);

      float binding_pattern = dis_binding(grid_cell->float_gen_);

      BP* bp_c = new BP((int)BP::BP_Type::START_PROTEIN,binding_pattern);

      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::END_RNA) {
      BP* bp_c = new BP((int)BP::BP_Type::END_RNA);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::END_PROTEIN) {
      BP* bp_c = new BP((int)BP::BP_Type::END_PROTEIN);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::START_PUMP) {
      BP* bp_c = new BP((int)BP::BP_Type::START_PUMP);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::END_PUMP) {
      BP* bp_c = new BP((int)BP::BP_Type::END_PUMP);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::START_MOVE) {
      BP* bp_c = new BP((int)BP::BP_Type::START_MOVE);
      bp_list_.push_back(bp_c);
    } else if (bp_type == (int) BP::BP_Type::END_MOVE) {
      BP* bp_c = new BP((int)BP::BP_Type::END_MOVE);
      bp_list_.push_back(bp_c);
    }
  }
}

void DNA::insert_a_BP(int pos, GridCell* grid_cell) {
  std::uniform_int_distribution<uint32_t> dis(0,11);

  int bp_type = dis(grid_cell->float_gen_);

  if (bp_type == (int) BP::BP_Type::PROTEIN_BLOCK) {
    BP_Protein_Block* bp_protein_block = new BP_Protein_Block();

    std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Protein_base_length+1);
    std::uniform_int_distribution<uint32_t> dis_block_type(0,2);
    std::uniform_int_distribution<uint32_t> dis_arith(0,4);
    std::uniform_real_distribution<float> dis_number(0, 1);

    int block_size = dis_block(grid_cell->float_gen_);
    int bp_prot_type;
    int bp_prot_arith;
    float bp_prot_number;


    for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
      bp_prot_type = dis_block_type(grid_cell->float_gen_);

      if (bp_prot_type == (int) BP_Protein::BP_Protein_Type::ARITHMETIC_OPERATOR) {
        bp_prot_arith = dis_arith(grid_cell->float_gen_);
        BP_Protein* bp_prot = new BP_Protein(bp_prot_type,bp_prot_arith);
        bp_protein_block->bp_prot_list_.push_back(bp_prot);
      } else {
        // Number
        bp_prot_number=dis_number(grid_cell->float_gen_);
        BP_Protein* bp_prot = new BP_Protein(bp_prot_type,bp_prot_number);
        bp_protein_block->bp_prot_list_.push_back(bp_prot);
      }
    }

    BP* bp_c = new BP((int)BP::BP_Type::PROTEIN_BLOCK,bp_protein_block);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::PUMP_BLOCK) {
    BP_Pump_Block* bp_pump_block = new BP_Pump_Block();

    std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Pump_base_length+1);
    std::uniform_int_distribution<uint32_t> dis_in_out(0,2);
    std::uniform_int_distribution<uint32_t> dis_speed(0,Common::Pump_Max_Speed);
    std::uniform_real_distribution<float> dis_number(0, 1);

    int block_size = dis_block(grid_cell->float_gen_);
    bool in_out;
    float start_range;
    float end_range;
    int speed;

    for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
      in_out = dis_in_out(grid_cell->float_gen_);
      start_range=dis_number(grid_cell->float_gen_);
      end_range=dis_number(grid_cell->float_gen_);
      speed=dis_speed(grid_cell->float_gen_);

      BP_Pump* pump = new BP_Pump(in_out,start_range,end_range,speed);
      bp_pump_block->bp_pump_list_.push_back(pump);
    }

    BP* bp_c = new BP((int)BP::BP_Type::PUMP_BLOCK,bp_pump_block);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::MOVE_BLOCK) {
    BP_Move_Block* bp_move_block = new BP_Move_Block();

    std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Move_base_length+1);
    std::uniform_int_distribution<uint32_t> dis_retry(0,Common::Max_Move_Retry+1);
    std::uniform_int_distribution<uint32_t> dis_distance(0,Common::Max_Move_Distance+1);

    int block_size = dis_block(grid_cell->float_gen_);
    int retry;
    int distance;

    for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
      retry = dis_retry(grid_cell->float_gen_);
      distance = dis_distance(grid_cell->float_gen_);

      BP_Move* move = new BP_Move(retry,distance);
      bp_move_block->bp_move_list_.push_back(move);
    }

    BP* bp_c = new BP((int)BP::BP_Type::MOVE_BLOCK,bp_move_block);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_RNA) {
    std::uniform_real_distribution<float> dis_binding(0, 1);
    std::uniform_real_distribution<float> dis_concentration(0, 1);

    float binding_pattern = dis_binding(grid_cell->float_gen_);
    float concentration = dis_concentration(grid_cell->float_gen_);

    BP* bp_c = new BP((int)BP::BP_Type::START_RNA,binding_pattern,concentration);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_PROTEIN) {
    std::uniform_real_distribution<float> dis_binding(0, 1);

    float binding_pattern = dis_binding(grid_cell->float_gen_);

    BP* bp_c = new BP((int)BP::BP_Type::START_PROTEIN,binding_pattern);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_RNA) {
    BP* bp_c = new BP((int)BP::BP_Type::END_RNA);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_PROTEIN) {
    BP* bp_c = new BP((int)BP::BP_Type::END_PROTEIN);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_PUMP) {
    BP* bp_c = new BP((int)BP::BP_Type::START_PUMP);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_PUMP) {
    BP* bp_c = new BP((int)BP::BP_Type::END_PUMP);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_MOVE) {
    BP* bp_c = new BP((int)BP::BP_Type::START_MOVE);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_MOVE) {
    BP* bp_c = new BP((int)BP::BP_Type::END_MOVE);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  }
}

void DNA::modify_bp(int pos, GridCell* grid_cell) {
  std::uniform_int_distribution<uint32_t> dis(0,11);

  int bp_type = ( bp_list_[pos]->type_ + dis(grid_cell->float_gen_) ) % 10;

  if (bp_type == bp_list_[pos]->type_ )
    return;
  else
    bp_list_.erase(bp_list_.begin()+pos);

  if (bp_type == (int) BP::BP_Type::PROTEIN_BLOCK) {
    BP_Protein_Block* bp_protein_block = new BP_Protein_Block();

    std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Protein_base_length+1);
    std::uniform_int_distribution<uint32_t> dis_block_type(0,2);
    std::uniform_int_distribution<uint32_t> dis_arith(0,4);
    std::uniform_real_distribution<float> dis_number(0, 1);

    int block_size = dis_block(grid_cell->float_gen_);
    int bp_prot_type;
    int bp_prot_arith;
    float bp_prot_number;


    for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
      bp_prot_type = dis_block_type(grid_cell->float_gen_);

      if (bp_prot_type == (int) BP_Protein::BP_Protein_Type::ARITHMETIC_OPERATOR) {
        bp_prot_arith = dis_arith(grid_cell->float_gen_);
        BP_Protein* bp_prot = new BP_Protein(bp_prot_type,bp_prot_arith);
        bp_protein_block->bp_prot_list_.push_back(bp_prot);
      } else {
        // Number
        bp_prot_number=dis_number(grid_cell->float_gen_);
        BP_Protein* bp_prot = new BP_Protein(bp_prot_type,bp_prot_number);
        bp_protein_block->bp_prot_list_.push_back(bp_prot);
      }
    }

    BP* bp_c = new BP((int)BP::BP_Type::PROTEIN_BLOCK,bp_protein_block);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::PUMP_BLOCK) {
    BP_Pump_Block* bp_pump_block = new BP_Pump_Block();

    std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Pump_base_length+1);
    std::uniform_int_distribution<uint32_t> dis_in_out(0,2);
    std::uniform_int_distribution<uint32_t> dis_speed(0,Common::Pump_Max_Speed);
    std::uniform_real_distribution<float> dis_number(0, 1);

    int block_size = dis_block(grid_cell->float_gen_);
    bool in_out;
    float start_range;
    float end_range;
    int speed;

    for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
      in_out = dis_in_out(grid_cell->float_gen_);
      start_range=dis_number(grid_cell->float_gen_);
      end_range=dis_number(grid_cell->float_gen_);
      speed=dis_speed(grid_cell->float_gen_);

      BP_Pump* pump = new BP_Pump(in_out,start_range,end_range,speed);
      bp_pump_block->bp_pump_list_.push_back(pump);
    }

    BP* bp_c = new BP((int)BP::BP_Type::PUMP_BLOCK,bp_pump_block);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::MOVE_BLOCK) {
    BP_Move_Block* bp_move_block = new BP_Move_Block();

    std::uniform_int_distribution<uint32_t> dis_block(1,Common::BP_Move_base_length+1);
    std::uniform_int_distribution<uint32_t> dis_retry(0,Common::Max_Move_Retry+1);
    std::uniform_int_distribution<uint32_t> dis_distance(0,Common::Max_Move_Distance+1);

    int block_size = dis_block(grid_cell->float_gen_);
    int retry;
    int distance;

    for (int ibp_block=0; ibp_block < block_size; ibp_block++) {
      retry = dis_retry(grid_cell->float_gen_);
      distance = dis_distance(grid_cell->float_gen_);

      BP_Move* move = new BP_Move(retry,distance);
      bp_move_block->bp_move_list_.push_back(move);
    }

    BP* bp_c = new BP((int)BP::BP_Type::MOVE_BLOCK,bp_move_block);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);

  } else if (bp_type == (int) BP::BP_Type::START_RNA) {
    std::uniform_real_distribution<float> dis_binding(0, 1);
    std::uniform_real_distribution<float> dis_concentration(0, 1);

    float binding_pattern = dis_binding(grid_cell->float_gen_);
    float concentration = dis_concentration(grid_cell->float_gen_);

    BP* bp_c = new BP((int)BP::BP_Type::START_RNA,binding_pattern,concentration);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_PROTEIN) {
    std::uniform_real_distribution<float> dis_binding(0, 1);

    float binding_pattern = dis_binding(grid_cell->float_gen_);

    BP* bp_c = new BP((int)BP::BP_Type::START_PROTEIN,binding_pattern);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_RNA) {
    BP* bp_c = new BP((int)BP::BP_Type::END_RNA);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_PROTEIN) {
    BP* bp_c = new BP((int)BP::BP_Type::END_PROTEIN);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_PUMP) {
    BP* bp_c = new BP((int)BP::BP_Type::START_PUMP);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_PUMP) {
    BP* bp_c = new BP((int)BP::BP_Type::END_PUMP);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::START_MOVE) {
    BP* bp_c = new BP((int)BP::BP_Type::START_MOVE);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  } else if (bp_type == (int) BP::BP_Type::END_MOVE) {
    BP* bp_c = new BP((int)BP::BP_Type::END_MOVE);
    bp_list_.insert(bp_list_.begin()+pos,bp_c);
  }
}


