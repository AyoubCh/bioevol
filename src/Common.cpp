//
// Created by arrouan on 28/09/16.
//

//#include <stdint-gcc.h>
#include <stdint.h>
#include <random>
//#include <QTZlib/zlib.h>
#include <zlib.h>
#include "Common.h"

float Common::matrix_binding_[BINDING_MATRIX_SIZE*BINDING_MATRIX_SIZE];

void Common::init_binding_matrix(uint32_t seed) {

  std::mt19937 float_gen_;
  float_gen_.seed(seed);
  std::uniform_real_distribution<float> dis_number(-1, 1);
  std::uniform_int_distribution<int8_t> dis_percent(0,100);

  for (int i = 0; i < BINDING_MATRIX_SIZE; i++) {
    for (int j = 0; j < BINDING_MATRIX_SIZE; j++) {
      if (dis_percent(float_gen_) > BINDING_MATRIX_ZERO_PERCENT)
        matrix_binding_[i*BINDING_MATRIX_SIZE+j]=dis_number(float_gen_);
      else
        matrix_binding_[i*BINDING_MATRIX_SIZE+j]=0;
    }
  }

}

void Common::save_binding_matrix() {
  char* lookup_table_file_name = new char[100];

  sprintf( lookup_table_file_name, "binding_matrix.zip" );

  gzFile lookup_table_file = gzopen( lookup_table_file_name, "r" );

  if ( lookup_table_file == Z_NULL )
  {
    printf( "ERROR : Could not read lookup table file %s\n", lookup_table_file_name );
    exit( EXIT_FAILURE );
  }

  float value;
  for (int i = 0; i < BINDING_MATRIX_SIZE; i++) {
    for (int j = 0; j < BINDING_MATRIX_SIZE; j++) {
      gzread(lookup_table_file, &value, sizeof(float));
      matrix_binding_[i*BINDING_MATRIX_SIZE+j] = value;
    }
  }

  gzclose( lookup_table_file );

  delete[] lookup_table_file_name;
}

void Common::load_binding_matrix() {
  char* lookup_table_file_name = new char[100];

  sprintf( lookup_table_file_name, "binding_matrix.zip" );

  gzFile lookup_table_file = gzopen( lookup_table_file_name, "w" );

  if ( lookup_table_file == Z_NULL )
  {
    printf( "ERROR : Could not write lookup table file %s\n", lookup_table_file_name );
    exit( EXIT_FAILURE );
  }

  double value;
  for (int i = 0; i < BINDING_MATRIX_SIZE; i++) {
    for (int j = 0; j < BINDING_MATRIX_SIZE; j++) {
      value = matrix_binding_[i*BINDING_MATRIX_SIZE+j];

      gzwrite(lookup_table_file, &value, sizeof(float));
    }
  }

  gzclose( lookup_table_file );

  delete[] lookup_table_file_name;
}
