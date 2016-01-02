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
#include <math.h>

char **sudoku_board;
int sudoku_board_y_size = 0;
int sudoku_board_x_size = 0;
int sudoku_zone_width = 0;
int sudoku_zone_height = 0;
int **cvalue_table;

void readFile(const char * path){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    //printf("try to open file\n");
    fp = fopen(path, "rt");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    sudoku_board = (char**)calloc(1, sizeof(char));
    
    //printf("reading from opened file\n");
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
    //printf("closing file\n");
    fclose(fp);
    //printf("closed file\n");
    if (line)
        free(line);
    if(sizeof(sudoku_board)%2 !=0){
        printf("Input size error!");
        exit(1);
    }
    
    for (int y = 0; y < sudoku_board_y_size; y++) {
        for (int x = 0; x < sudoku_board_x_size; x++) {
            if (sudoku_board[y][x] == NULL){
                printf("Input size error!");
                exit(1);
            }
            //printf("%d-%d: %c \n", y, x, sudoku_board[y][x]);
        }
    }
    int temp = sqrt(sudoku_board_x_size);
    if (temp * temp == sudoku_board_x_size) {
        sudoku_zone_width = sqrt(sudoku_board_x_size);
        sudoku_zone_height = sqrt(sudoku_board_x_size);
    }else{
        sudoku_zone_width = sudoku_board_x_size / 2;
        sudoku_zone_height = 2;
    }
    print();
    
    printf("This is a %d x %d Sudoku Puzzle! 🎲 \n", sudoku_board_x_size, sudoku_board_y_size);
}

void print(){
    for (int y = 0; y < sudoku_board_y_size; y++) {
        if (y % (sudoku_zone_height)== 0) {
            for (int a = 0; a < (sudoku_board_x_size * 3 + 3); a++) {
                printf("-");
            }
            printf("\n");
        }
        for (int x = 0; x < sudoku_board_x_size; x++) {
            if (x % (sudoku_zone_width) == 0) {
                printf("|");
            }
            
            printf(" %c ", sudoku_board[y][x]);
        }
        printf("|\n");
    }
    for (int a = 0; a < (sudoku_board_x_size * 3 + 3); a++) {
        printf("-");
    }
    printf("\n");
}

void intial_cvalue_tabel(){
    cvalue_table = malloc(sizeof(int) * sudoku_board_y_size);
    int* tempblock;
    tempblock = malloc(sudoku_board_y_size * sudoku_board_x_size * sizeof(int));
    for (int i = 0; i < sudoku_board_y_size; i++) {
        cvalue_table[i] = tempblock + (i * sudoku_board_x_size);
    }
    //for (int a = 0; a < sudoku_board_y_size; a++) {
      //  cvalue_table[a] = malloc(sizeof(int) * sudoku_board_x_size);
    //}
    
    for (int y = 0; y < sudoku_board_y_size; y++) {
        for (int x = 0; x < sudoku_board_x_size; x++) {
            cvalue_table[y][x] = get_cvalue (x, y);
        }
    }
    
    printf("This is value table: \n");
    print_cvalue_table();
}

void print_cvalue_table(){
    for (int y = 0; y < sudoku_board_y_size; y++) {
        if (y % (sudoku_zone_height)== 0) {
            for (int a = 0; a < (sudoku_board_x_size * 3 + 3); a++) {
                printf("-");
            }
            printf("\n");
        }
        for (int x = 0; x < sudoku_board_x_size; x++) {
            if (x % (sudoku_zone_width) == 0) {
                printf("|");
            }
            
            printf(" %d ", cvalue_table[y][x]);
        }
        printf("|\n");
    }
    for (int a = 0; a < (sudoku_board_x_size * 3 + 3); a++) {
        printf("-");
    }
    printf("\n");
}

int get_cvalue(int x, int y){
    int cvalue = 0;
    int zone_x_start_at = 0;
    int zone_x_end_at = 0;
    int zone_y_start_at = 0;
    int zone_y_end_at = 0;
    
    if (isdigit(sudoku_board[y][x])==1) {
        return -1;
    }
    
    for (int xx = sudoku_zone_width; xx <= sudoku_board_x_size; xx += sudoku_zone_width) {
        if (x < xx) {
            zone_x_end_at = xx - 1;
            zone_x_start_at = (xx - sudoku_zone_width);
            break;
        }
    }
    
    for (int yy = sudoku_zone_height; yy <= sudoku_board_y_size; yy += sudoku_zone_height) {
        if (y < yy) {
            zone_y_end_at = yy - 1;
            zone_y_start_at = (yy - sudoku_zone_height);
            break;
        }
    }
    
    for (int pointery = zone_y_start_at; pointery <= zone_y_end_at; pointery++) {
        for (int pointerx = zone_x_start_at; pointerx <= zone_x_end_at; pointerx++) {
            if (isdigit(sudoku_board[pointery][pointerx]) == 1) {
                //printf("%d, %d \n", pointerx, pointery);
                cvalue++;
            }
        }
    }
    
    for (int pointerx = 0; pointerx < zone_x_start_at; pointerx++) {
        if (isdigit(sudoku_board[y][pointerx]) == 1) {
            //printf("%d, %d \n", pointerx, y);
            cvalue++;
        }
    }
    
    for (int pointerx = zone_x_end_at + 1; pointerx < sudoku_board_x_size; pointerx++) {
        if (isdigit(sudoku_board[y][pointerx]) == 1) {
            //printf("%d, %d \n", pointerx, y);
            cvalue++;
        }
    }
    
    for (int pointery = 0; pointery < zone_y_start_at; pointery++) {
        if (isdigit(sudoku_board[pointery][x]) == 1) {
            //printf("%d, %d \n", x, pointery);
            cvalue++;
        }
    }
    
    for (int pointery = zone_y_end_at + 1; pointery < sudoku_board_y_size; pointery++) {
        if (isdigit(sudoku_board[pointery][x]) == 1) {
            //printf("%d, %d \n", x, pointery);
            cvalue++;
        }
    }
    //printf("cvalue = %d\n%c\n", cvalue, (char**)sudoku_board[1][0]);
    return cvalue;
}

void freeAll(){
    free(sudoku_board);
}