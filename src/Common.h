//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_COMMON_H
#define PDC_EVOL_MODEL_COMMON_H


#include "BP_Protein.h"

class Common {
 public:
    const static int DNA_base_length = 500;

    const static int BP_Protein_base_length = 50;
    const static int BP_Pump_base_length = 50;
    const static int BP_Move_base_length = 50;

    const static int Max_Move_Retry = 8;
    const static int Max_Move_Distance = 8;

    const static int BINDING_MATRIX_SIZE = 1000;
    const static int BINDING_MATRIX_ZERO_PERCENT = 75;

    static float matrix_binding_[BINDING_MATRIX_SIZE*BINDING_MATRIX_SIZE];

    static void init_binding_matrix(uint32_t seed);
    static void save_binding_matrix();
    static void load_binding_matrix();

    const static uint32_t Pump_Max_Speed = 20;

    constexpr static float hill_shape = 4.0;
    constexpr static float hill_shape_n = 0.0625;
    constexpr static float Protein_Degradation_Rate = 0.01;
    const static int Protein_Degradation_Step = 10;
    constexpr static float Random_Death  = 1e-5;

    const static int Duplicate_Neighbors_Offset = 1;

    const static int Metabolic_Error_Precision = 500;
    const static int Metabolic_Error_Protein_Spray = 5;
    constexpr static float Metabolic_Error_Protein_Slope = 0.1;

    constexpr static float Fitness_Selection_Pressure = 10.0;
    constexpr static double Mutation_Rate = 1e-3;

    const static int Diffusion_Neighbors_Size = 1;
    constexpr static float Diffusion_Quantity = 0.1;

    /** Graphical Constants **/
    const static int CELL_SPACE = 3;
    const static int CELL_SCALE = 15;
    const static int SPAN = 1;
    const static size_t GRADIENT_SIZE = 20;
    constexpr static double GRADIENT_SCALE = 30;

    const static int Number_Degradation_Step = 5;//10;
    //const static int Number_Evolution_Step = 1000000;
    //const static int Number_Evolution_Step = 100000+1;
    const static int Number_Evolution_Step = 10000+1;
	const static int Time_flush = 10;
};


#endif //PDC_EVOL_MODEL_COMMON_H
