//
// Created by arrouan on 28/09/16.
//

#include "GridCell.h"
#include "Organism.h"

void GridCell::diffuse_protein() {
  for (int i = x_ - Common::Diffusion_Neighbors_Size; i <= x_ + Common::Diffusion_Neighbors_Size; i++) {
    for (int j = y_  - Common::Diffusion_Neighbors_Size; j <= y_ + Common::Diffusion_Neighbors_Size; j++) {
      if (i >= 0 || i < world_->width_)
        if (j >= 0 || j < world_->height_) {
          for (auto prot : protein_list_map_) {
            if (prot.second->concentration_up() > 0) {
              if ( world_->grid_cell_[i*world_->width_+j]->protein_list_map_.
                  find(prot.first) == world_->grid_cell_[i*world_->width_+j]->
                  protein_list_map_.end() ) {
                Protein* prot_n = new Protein(prot.second);
                prot_n->set_concentration(prot.second->concentration_up()*Common::Diffusion_Quantity);
                prot.second->concentration_up( -1* prot.second->concentration_up()*Common::Diffusion_Quantity);
                world_->grid_cell_[i*world_->width_+j]->protein_list_map_[prot.first] = prot_n;
              } else {
                world_->grid_cell_[i*world_->width_+j]->protein_list_map_[prot.first]->
                    concentration_up( prot.second->concentration_up()*Common::Diffusion_Quantity);
                prot.second->concentration_up(-1 * prot.second->concentration_up()*Common::Diffusion_Quantity);
              }
            }
          }
        }
    }
  }
}

void GridCell::degrade_protein() {
  for (auto prot : protein_list_map_) {
    prot.second->concentration_up( -1 *
        prot.second->concentration_up() * Common::Protein_Degradation_Rate );
  }
}

void GridCell::push_new_protein(std::vector<Protein*> protein_list) {
  for (auto prot : protein_list) {
    if ( protein_list_map_.find(prot->get_value()) == protein_list_map_.end() ) {
      protein_list_map_[prot->get_value()] =  prot;
    } else {
      protein_list_map_[prot->get_value()]->concentration_up( prot->concentration_up());
    }
  }
}





