//
//  SudokuSolver.c
//  SudokuSolverinC
//
//  Created by Janus on 1/1/2016.
//  Copyright © 2016年 Janus. All rights reserved.
//

#include "SudokuSolver.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char **sudoku_board;
int sudoku_board_y_size = 0;
int sudoku_board_x_size = 0;

void readFile(const char * path){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    fp = fopen(path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    sudoku_board = (char**)calloc(1, sizeof(char));
    
    while ((read = getline(&line, &len, fp)) != -1) {
        char* token;
        char* string;
        char* tofree;
        
        string = strdup(line);
        
        if (string != NULL) {
            
            tofree = string;
            sudoku_board_y_size++;
            sudoku_board = realloc(sudoku_board, sudoku_board_y_size+1 * sizeof(char));
            sudoku_board[sudoku_board_y_size-1] = malloc(sizeof(char) * 1);
            sudoku_board_x_size = 0;
            int count = 0;
            while ((token = strsep(&string, "\t")) != NULL)
            {
                sudoku_board_x_size++;
                sudoku_board[sudoku_board_y_size-1] = realloc(sudoku_board[sudoku_board_y_size-1], sudoku_board_x_size * sizeof(char));
                sudoku_board[sudoku_board_y_size-1][sudoku_board_x_size-1] = *token;
                //printf("%d: %c\n", count, sudoku_board[sudoku_board_y_size-1][sudoku_board_x_size-1]);
                count++;
            }
        }
    }
    fclose(fp);
    if (line)
        free(line);
    if(sizeof(sudoku_board)%2 !=0){
        printf("Input size error!");
        exit(1);
    }

    for (int y = 0; y < sudoku_board_y_size; y++) {
        for (int x = 0; x < sudoku_board_x_size; x++) {
            printf("%d-%d: %c \n", y, x, sudoku_board[y][x]);
        }
    }
    
    printf("This is a %d x %d Sudoku Puzzle! \n", sudoku_board_x_size, sudoku_board_y_size);
    free(sudoku_board);
    exit(EXIT_SUCCESS);
}