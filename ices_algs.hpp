///////////////////////////////////////////////////////////////////////////////
// ices_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
// Solve the iceberg avoiding problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_exhaustive(const grid& setting) {
    
  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t steps = setting.rows() + setting.columns() - 2;
  assert(steps < 64);
  unsigned int count_paths = 0;

	for (int bits = 0; bits <= (pow(2, steps) - 1);bits++) {
		path candidate(setting);
		int invalid = 0;
		for(size_t k = 0; k <= (steps - 1); k++) {
			auto bit = (bits>>k)&1;
			if (bit == 1) {
				if (candidate.is_step_valid(STEP_DIRECTION_RIGHT)){
					candidate.add_step(STEP_DIRECTION_RIGHT);
				}
				else 
					invalid++;
			}
			else {
				if (candidate.is_step_valid(STEP_DIRECTION_DOWN)) {
					candidate.add_step(STEP_DIRECTION_DOWN);
				}
				else 
					invalid++;
			}
		}
		if(invalid == 0)
			count_paths++;
	}
  return count_paths;
}

// Solve the iceberg avoiding problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  const int DIM=100;
  std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));

  A[0][0] = 1;
  auto r = setting.rows(), c = setting.columns();
	for (size_t i = 0; i < r; i++){
		for (size_t j = 0; j < c; j++){
			if(setting.get(i,j) == CELL_ICEBERG){
				A[i][j] = 0;
			}
			int from_above = 0, from_left = 0;
			bool a_true = false, l_true = false;
			
			if( i > 0 && (setting.get(i-1,j) != CELL_ICEBERG)) {
				from_above = A[i-1][j];
				a_true = true;
			}
			if( j > 0 && (setting.get(i,j-1) != CELL_ICEBERG)) {
				from_left = A[i][j-1];
				l_true = true;
			}
			if (a_true && l_true)
				A[i][j] = from_left + from_above;
			else if(!a_true && l_true)
				A[i][j] = from_left;
			else if(a_true && !l_true)
				A[i][j] = from_above;
			
		}

	}
  return A[r-1][c-1];
}
}
