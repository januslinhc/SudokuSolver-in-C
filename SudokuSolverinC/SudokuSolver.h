//
//  SudokuSolver.h
//  SudokuSolverinC
//
//  Created by Janus on 1/1/2016.
//  Copyright © 2016年 Janus. All rights reserved.
//

#ifndef SudokuSolver_h
#define SudokuSolver_h
#include <stdio.h>

void readFile(const char * path);
void print();
void freeAll();
void intial_cvalue_tabel();
void get_cvalue(int x, int y);

#endif /* SudokuSolver_h */
