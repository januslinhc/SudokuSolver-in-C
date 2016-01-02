#!/usr/bin/env bash
cd SudokuSolverinC
gcc -o main main.c SudokuSolver.c SudokuSolver.h
mv main ../