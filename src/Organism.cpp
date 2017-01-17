//
// Created by arrouan on 28/09/16.
//

#include <omp.h>

#include "Organism.h"
#include "DNA.h"
#include "Common.h"

#include <omp.h>


void Organism::init_organism() {
  translate_RNA();
  translate_protein();
  translate_pump();
  translate_move();
}



void Organism::translate_RNA_back() {

  RNA* current_rna = nullptr;
  //for (auto it = dna_->bp_list_.begin(); it != dna_->bp_list_.end(); it++)

#pragma omp parallele for private(current_rna) schedule(runtime)
//#pragma omp parallele for
//#pragma omp for schedule(runtime)

  for( int i = 0; i < dna_->bp_list_.size(); i++)
  {
    BP** it = &(dna_->bp_list_[i]);
    //RNA* current_rna = nullptr;
    if ((*it)->type_ == (int)BP::BP_Type::START_RNA)
    {
      current_rna = new RNA((*it)->binding_pattern_, (*it)->concentration_);
    }
    else if ((*it)->type_ == (int)BP::BP_Type::END_RNA)
    {
      if (current_rna != nullptr)
      {
        rna_list_.push_back(current_rna);
        current_rna = nullptr;
      }
    }
    else if (current_rna != nullptr)
    {
      //current_rna->bp_list_.push_back(new BP((*it)));
      current_rna->bp_list_.push_back((*it)->clone());
    }
  }
}

void Organism::translate_RNA() {

  RNA* current_rna = nullptr;
  
  //int packet_size = dna_->bp_list_.size()/omp_get_max_threads();

  for( int i = 0; i < dna_->bp_list_.size(); i++)
  {
    BP** it = &(dna_->bp_list_[i]);
    if ((*it)->type_ == (int)BP::BP_Type::START_RNA)
    {
		current_rna = new RNA((*it)->binding_pattern_, (*it)->concentration_);
    }
    else if (current_rna != nullptr)
    {
        if ((*it)->type_ == (int)BP::BP_Type::END_RNA)
        {
            rna_list_.push_back(current_rna);
            current_rna = nullptr;
        }
        else
        {
          //current_rna->bp_list_.push_back(new BP((*it)));
            current_rna->bp_list_.push_back((*it)->clone());
        }
    }
  }
}

void Organism::translate_protein() {

  float binding_pattern = -1;
  int rna_id = 0;
  for ( auto it = rna_list_.begin(); it != rna_list_.end(); it++ ) {
    for (auto it_j = (*it)->bp_list_.begin(); it_j < (*it)->bp_list_.end(); it_j++) {
      if ((*it_j)->type_ == (int) BP::BP_Type::START_PROTEIN) {
        binding_pattern = (*it_j)->binding_pattern_;
      } else if ((*it_j)->type_ == (int) BP::BP_Type::END_PROTEIN) {
        binding_pattern = -1;
      } else if (((*it_j)->type_ ==
          (int) BP::BP_Type::PROTEIN_BLOCK) && (binding_pattern != -1)) {
        bool current_float = false; // true == next value if op arith else float
        bool first_value = true; // current_value is initialized or not
        float current_value = -1;
        int current_arith_op = -1;

        for (auto it_k = (*it_j)->get_protein_block_()->bp_prot_list_.begin();
             it_k < (*it_j)->get_protein_block_()->bp_prot_list_.end();
             it_k++) {
          if ((*it_k)->type_ ==
                (int) BP_Protein::BP_Protein_Type::ARITHMETIC_OPERATOR) {
            if (current_float) {
              current_arith_op = (*it_k)->op_;
              current_float = false;
            }
          } else if ((*it_k)->type_ ==
                (int) BP_Protein::BP_Protein_Type::FLOAT_NUMBER) {
            if ((!current_float) && first_value) {
              current_value =
                  (*it_k)->number_;
              current_float = true;
              first_value = false;
            } else if ((!current_float) && (!first_value)) {
              float value = (*it_k)->number_;

              current_float = true;
              if (current_arith_op == (int) BP_Protein::Arithmetic_Operator_Type::ADD) {
                current_value+=value;
              } else if (current_arith_op == (int) BP_Protein::Arithmetic_Operator_Type::MODULO) {
                current_value=std::fmod(current_value,value);
              } else if (current_arith_op == (int) BP_Protein::Arithmetic_Operator_Type::MULTIPLY) {
                current_value*=value;
              } else if (current_arith_op == (int) BP_Protein::Arithmetic_Operator_Type::POWER) {
                current_value=std::pow(current_value,value);
              }
            }
          }
        }

        int type = -1;
        if (current_value < 0.8) {
          type = (int) Protein::Protein_Type::FITNESS;
        } else if (current_value >= 0.8 && current_value < 0.9) {
          type = (int) Protein::Protein_Type::TF;
        } else if (current_value >= 0.9 && current_value < 0.95) {
          type = (int) Protein::Protein_Type::POISON;
        } else if (current_value >= 0.95 && current_value < 1.0) {
          type = (int) Protein::Protein_Type::ANTIPOISON;
        }

        Protein* prot = new Protein(type,binding_pattern,current_value);
        prot->set_concentration( (*it)->concentration_base_);



        if ( protein_list_map_.find(current_value) == protein_list_map_.end() ) {
          protein_list_map_[current_value] = prot;


          if (type == (int) Protein::Protein_Type::FITNESS) {
            protein_fitness_list_.push_back(prot);
          } else if (type == (int) Protein::Protein_Type::TF) {
            protein_TF_list_.push_back(prot);
          } else if (type == (int) Protein::Protein_Type::POISON) {
            protein_poison_list_.push_back(prot);
          } else if (type == (int) Protein::Protein_Type::ANTIPOISON) {
            protein_antipoison_list_.push_back(prot);
          }

          rna_produce_protein_[rna_id][current_value] = prot;
        } else {
          protein_list_map_[current_value]->concentration_up((*it)->concentration_base_);
          delete prot;
          rna_produce_protein_[rna_id][current_value] = protein_list_map_[current_value];
        }

      }
    }
    rna_id++;
  }
}

void Organism::translate_pump() {
  bool within_pump = false;

  for ( auto it = rna_list_.begin(); it != rna_list_.end(); it++ ) {
    for (auto it_j = (*it)->bp_list_.begin(); it_j < (*it)->bp_list_.end(); it_j++) {
      if ((*it_j)->type_ ==
          (int) BP::BP_Type::START_PUMP) {
        within_pump = true;
      } else if ((*it_j)->type_ ==
                 (int) BP::BP_Type::END_PUMP) {
        within_pump = false;
      } else if (((*it_j)->type_ ==
                  (int) BP::BP_Type::PUMP_BLOCK) && (within_pump)) {
        for (auto it_k=(*it_j)->pump_block_->bp_pump_list_.begin(); it_k < (*it_j)->pump_block_->
            bp_pump_list_.end(); it_k++) {
          Pump* pump = new Pump((*it_k)->in_out_,(*it_k)->start_range_,
                                (*it_k)->end_range_,(*it_k)->speed_);
          pump_list_.push_back(pump);
        }
      }
    }
  }
}

void Organism::translate_move() {
	
	
	//int packet_size = Common::Number_Evolution_Step/omp_get_max_threads();
	
  //#pragma omp parallel for 
  //for ( auto it = rna_list_.begin(); it != rna_list_.end(); it++ ) 
  //{
  for( int i = 0; i < rna_list_.size(); i++)
  {
    RNA** it = &(rna_list_[i]);
	bool within_move = false;
    for (auto it_j = (*it)->bp_list_.begin(); it_j < (*it)->bp_list_.end(); it_j++) 
    {
      if ((*it_j)->type_ == (int) BP::BP_Type::START_MOVE) 
      {
        within_move = true;
      } 
      else if ((*it_j)->type_ == (int) BP::BP_Type::END_MOVE) 
      {
        within_move = false;
      } 
      else if (((*it_j)->type_ == (int) BP::BP_Type::MOVE_BLOCK) && (within_move)) 
      {
		//#pragma omp parallel for shared(it_j)
        for( int j=0; j < (*it_j)->move_block_-> bp_move_list_.size();j++)
        {
		  BP_Move* it_k = (*it_j)->move_block_-> bp_move_list_[j];
          Move* move = new Move( it_k->distance_, it_k ->retry_);
          
          //#pragma omp critical 
          {
			move_list_.push_back(move);
			//printf("-> %d \n",move->distance_);
		   }
        }
      }
    }
  }

}

void Organism::build_regulation_network() {
  int rna_id = 0;
  for ( auto it = rna_list_.begin(); it != rna_list_.end(); it++ ) {
    for ( auto it_j = protein_fitness_list_.begin(); it_j != protein_fitness_list_.end(); it_j++ ) {
      int index_i = (*it)->binding_pattern_*Common::BINDING_MATRIX_SIZE;
      int index_j = (*it_j)->binding_pattern_*Common::BINDING_MATRIX_SIZE;
      if (Common::matrix_binding_[index_i*Common::BINDING_MATRIX_SIZE+index_j] != 0) {
        rna_influence_[rna_id][(*it_j)->value_] = Common::matrix_binding_[index_i*Common::BINDING_MATRIX_SIZE+index_j];
      }
    }
    for ( auto it_j = protein_TF_list_.begin(); it_j != protein_TF_list_.end(); it_j++ ) {
      int index_i = rna_list_[rna_id]->binding_pattern_*Common::BINDING_MATRIX_SIZE;
      int index_j =  (*it_j)->binding_pattern_*Common::BINDING_MATRIX_SIZE;
      if (Common::matrix_binding_[index_i*Common::BINDING_MATRIX_SIZE+index_j] != 0) {
        rna_influence_[rna_id][(*it_j)->value_] = Common::matrix_binding_[index_i*Common::BINDING_MATRIX_SIZE+index_j];
      }
    }
    rna_id++;
  }
}

void Organism::compute_next_step() {
  // Activate Pump
  activate_pump();

  // Compute protein concentration for X steps
  compute_protein_concentration();
}



void Organism::activate_pump() {

    //#pragma omp critical(activate_pump)
    {
        for (auto it = pump_list_.begin(); it != pump_list_.end(); it++) {
        if ((*it)->in_out_) {
          for (auto prot : protein_list_map_) {
            if ((*it)->start_range_ >= prot.second->value_ &&
                (*it)->end_range_ <= prot.second->value_) {
              float remove = prot.second->concentration_up()*((*it)->speed_/100);
              prot.second->concentration_up(-1*remove);

              #pragma omp critical(activate_pump1)
              {
                  if ( gridcell_->protein_list_map_.find(prot.second->value_)
                       == gridcell_->protein_list_map_.end() ) {
                    Protein* prot_n = new Protein(prot.second->type_,
                                                prot.second->binding_pattern_,
                                                  prot.second->value_);
                    prot_n->set_concentration(remove);
                    gridcell_->protein_list_map_[prot.second->value_] = prot_n;
                  } else {
                    gridcell_->protein_list_map_[prot.second->value_]
                        ->concentration_up( remove);
                  }
              }
            }
          }
        } else {
            //#pragma omp critical(activate_pump2)
            {
              for (auto prot : gridcell_->protein_list_map_) {
                if ((*it)->start_range_ >= prot.first &&
                    (*it)->end_range_ <= prot.first) {
                  float remove =
                      prot.second->concentration_up()*((*it)->speed_/100);
                  prot.second->concentration_up(-remove);
                  if ( protein_list_map_.find(prot.first)
                       == protein_list_map_.end() ) {
                    Protein* prot_n = new Protein(prot.second->type_,
                                                  prot.second->binding_pattern_,
                                                  prot.second->value_);
                    prot_n->set_concentration(remove);
                    protein_list_map_[prot_n->value_] = prot_n;
                  } else {
                    protein_list_map_[prot.first]
                        ->concentration_up( remove );
                  }
                }
              }
            }
        }
      }
    }
 }



void Organism::compute_protein_concentration() {
  //static float min = 1000;
  //static float max = 0;
  int rna_id = 0;
  for (auto it = rna_list_.begin(); it != rna_list_.end(); it++) {
    float delta_pos = 0, delta_neg = 0;
    for (auto prot : rna_influence_[rna_id]) {
      if (prot.second > 0)
        delta_pos += prot.second * protein_list_map_[prot.first]->concentration_up();
      else
        delta_neg -= prot.second * protein_list_map_[prot.first]->concentration_up();
    }

    float delta_pos_pow_n = pow(delta_pos,Common::hill_shape_n);
    float delta_neg_pow_n = pow(delta_neg,Common::hill_shape_n);

    /*if(delta_pos_pow_n > max) {
        max = delta_pos_pow_n;
        std::cout << "now max is " <<max<< std::endl;
    }

    if(delta_pos_pow_n < min) {
        min = delta_pos_pow_n;
        std::cout << "now min is "<< min << std::endl;
    }*/

     rna_list_[rna_id]->current_concentration_ = rna_list_[rna_id]->concentration_base_
                               * (Common::hill_shape
                                  / (delta_neg_pow_n + Common::hill_shape))
                               * (1 + ((1 / rna_list_[rna_id]->concentration_base_) - 1)
                                      * (delta_pos_pow_n /
                                         (delta_pos_pow_n +
                                             Common::hill_shape)));
    rna_id++;
  }

  std::unordered_map<float,float> delta_concentration;
  for (auto rna : rna_produce_protein_) {
    for (auto prot : rna_produce_protein_[rna.first]) {
      if (delta_concentration.find(prot.first) == delta_concentration.end()) {
        delta_concentration[prot.first] = rna_list_[rna.first]->current_concentration_;
      } else {
        delta_concentration[prot.first] += rna_list_[rna.first]->current_concentration_;
      }
    }
  }

  for (auto delta : delta_concentration) {
    delta.second -= Common::Protein_Degradation_Rate * protein_list_map_[delta.first]->concentration_up();
    delta.second *= 1/(Common::Protein_Degradation_Step);

    protein_list_map_[delta.first]->concentration_up(delta.second);
  }
}

bool Organism::dying_or_not() {
  // Compute if dying or not
  double concentration_sum = 0;
  for (auto prot : protein_fitness_list_) {
    concentration_sum+=prot->concentration_up();
  }
  for (auto prot : protein_TF_list_) {
    concentration_sum+=prot->concentration_up();
  }

  if (concentration_sum > 10.0 && concentration_sum <= 0.0) {
    return true;
  }

  double poison=0,antipoison=0;

  for (auto prot : protein_poison_list_) {
    poison+=prot->concentration_up();
  }

  for (auto prot : protein_antipoison_list_) {
    antipoison+=prot->concentration_up();
  }

  if (poison-antipoison>0.1) {
    return true;
  }

  std::binomial_distribution<int> dis_death(1024,Common::Random_Death);
  int death_number = dis_death(gridcell_->float_gen_);


  bool death = (bool) death_number % 2;

  return death;
}

void Organism::try_to_move() {
  for (auto it = move_list_.begin(); it != move_list_.end(); it++) {
    if ((*it)->distance_ > 0) {
      bool move = false;
      int retry = 0;
      std::uniform_int_distribution<uint32_t> dis_distance(0,(*it)->distance_);
      while (retry < (*it)->retry_ && !move) {
        int x_offset=dis_distance(gridcell_->float_gen_);
        int y_offset=dis_distance(gridcell_->float_gen_);
        int new_x,new_y;
        if (gridcell_->x_+x_offset >= gridcell_->world_->width_) {
          new_x = gridcell_->world_->width_-1;
        } else {
          new_x = gridcell_->x_+x_offset;
        }

        if (gridcell_->y_+y_offset >= gridcell_->world_->height_) {
          new_y = gridcell_->world_->height_-1;
        } else {
          new_y = gridcell_->y_+y_offset;
        }

        if (gridcell_->world_->grid_cell_[new_x*gridcell_->world_->width_+new_y]->organism_ != nullptr) {
          move = true;
          move_success_++;
          gridcell_->world_->grid_cell_[new_x*gridcell_->world_->width_+new_y]->organism_ = this;
          gridcell_ = gridcell_->world_->grid_cell_[new_x*gridcell_->world_->width_+new_y];
          gridcell_->organism_ = nullptr;
        }
      }
    }
  }
}

void Organism::compute_fitness() {
  life_duration_++;

  for (int i = 0; i < Common::Metabolic_Error_Precision; i++)
    metabolic_error[i] = 0.0;

  for (auto prot : protein_fitness_list_) 
  { //.begin(); it != protein_fitness_list_.end(); it++) {
    int index = prot->value_*Common::Metabolic_Error_Precision;

    float concentration = prot->concentration_up();

    for (int j = index - Common::Metabolic_Error_Protein_Spray;
        j <= index + Common::Metabolic_Error_Protein_Spray; j++) {
      if (j < Common::Metabolic_Error_Precision && j >= 0) {
        if (j < index) {
          metabolic_error[j] +=
              (index - j) * Common::Metabolic_Error_Protein_Slope * concentration;
        } else if (j > index) {
          metabolic_error[j] +=
              (j - index) * Common::Metabolic_Error_Protein_Slope * concentration;
        } else {
          metabolic_error[j] += concentration;
        }
      }
    }
  }

  sum_metabolic_error = 0;
  for (int i = 0; i < Common::Metabolic_Error_Precision; i++) {
    sum_metabolic_error+=std::abs(gridcell_->environment_target[i]-metabolic_error[i]);
  }

  sum_metabolic_error=sum_metabolic_error/Common::Metabolic_Error_Precision;

  fitness_ = std::exp(-Common::Fitness_Selection_Pressure*sum_metabolic_error);
}

void Organism::mutate() {
  old = this;

  std::binomial_distribution<int> dis_switch(dna_->bp_list_.size(),Common::Mutation_Rate);
  std::binomial_distribution<int> dis_insertion(dna_->bp_list_.size(),Common::Mutation_Rate);
  std::binomial_distribution<int> dis_deletion(dna_->bp_list_.size(),Common::Mutation_Rate);
  std::binomial_distribution<int> dis_duplication(dna_->bp_list_.size(),Common::Mutation_Rate);
  std::binomial_distribution<int> dis_modification(dna_->bp_list_.size(),Common::Mutation_Rate);

  int nb_switch = dis_switch(gridcell_->float_gen_);
  int nb_insertion = dis_insertion(gridcell_->float_gen_);
  int nb_deletion = dis_deletion(gridcell_->float_gen_);
  int nb_duplication = dis_duplication(gridcell_->float_gen_);
  int nb_modification = dis_modification(gridcell_->float_gen_);

  std::uniform_int_distribution<uint32_t> dis_position(0,dna_->bp_list_.size());

  for (int i = 0; i < nb_deletion; i++) {
    int deletion_pos = dis_position(gridcell_->float_gen_);
    while (deletion_pos >= dna_->bp_list_.size()) {
      deletion_pos = dis_position(gridcell_->float_gen_);
    }

    dna_->bp_list_.erase(dna_->bp_list_.begin() + deletion_pos);
  }

  for (int i = 0; i < nb_switch; i++) {
    int switch_pos_1 = dis_position(gridcell_->float_gen_);
    while (switch_pos_1 >= dna_->bp_list_.size()) {
      switch_pos_1 = dis_position(gridcell_->float_gen_);
    }
    int switch_pos_2 = dis_position(gridcell_->float_gen_);
    while (switch_pos_2 >= dna_->bp_list_.size()) {
      switch_pos_2 = dis_position(gridcell_->float_gen_);
    }

    BP* tmp = dna_->bp_list_[switch_pos_1];
    dna_->bp_list_[switch_pos_1] = dna_->bp_list_[switch_pos_2];
    dna_->bp_list_[switch_pos_2] = tmp;
  }

  for (int i = 0; i < nb_duplication; i++) {
    int duplication_pos = dis_position(gridcell_->float_gen_);
    while (duplication_pos >= dna_->bp_list_.size()) {
      duplication_pos = dis_position(gridcell_->float_gen_);
    }

    int where_to_duplicate = dis_position(gridcell_->float_gen_);
    while (where_to_duplicate >= dna_->bp_list_.size()) {
      where_to_duplicate = dis_position(gridcell_->float_gen_);
    }

    dna_->bp_list_.insert(dna_->bp_list_.begin()+where_to_duplicate,
                          //new BP(dna_->bp_list_[duplication_pos]));
                            (dna_->bp_list_[duplication_pos])->clone());
  }


  for (int i = 0; i < nb_insertion; i++) {
    int insertion_pos = dis_position(gridcell_->float_gen_);
    while (insertion_pos >= dna_->bp_list_.size()) {
      insertion_pos = dis_position(gridcell_->float_gen_);
    }

    dna_->insert_a_BP(insertion_pos,gridcell_);
  }

  for (int i = 0; i < nb_modification; i++) {
    int modification_pos = dis_position(gridcell_->float_gen_);
    while (modification_pos >= dna_->bp_list_.size()) {
      modification_pos = dis_position(gridcell_->float_gen_);
    }

    dna_->modify_bp(modification_pos,gridcell_);
  }
}

Organism* Organism::divide() {
  Organism* new_org = new Organism(this);
  return new_org;
}




Organism::Organism(Organism* organism) {
  dna_ = new DNA(organism->dna_);

  for(auto prot : organism->protein_list_map_) {
    Protein* new_prot = new Protein(prot.second);
    new_prot->set_concentration(new_prot->concentration_up()/2);
    prot.second->set_concentration( prot.second->concentration_up()/2);
  }
}

Organism::~Organism() {
  for (auto rna : rna_list_)
    delete rna;

  

  rna_influence_.clear();
  rna_produce_protein_.clear();

  for (auto prot : protein_list_map_) {
    delete prot.second;
  }

  protein_fitness_list_.clear();
  protein_TF_list_.clear();
  protein_poison_list_.clear();
  protein_antipoison_list_.clear();

  protein_list_map_.clear();

  for (auto pump : pump_list_) {
    delete pump;
  }
  pump_list_.clear();

  for (auto move : move_list_) {
    delete move;
  }
  move_list_.clear();
  
  //#pragma omp critical
  delete dna_;
}
