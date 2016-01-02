//
//  main.c
//  SudokuSolverinC
//
//  Created by Janus on 1/1/2016.
//  Copyright © 2016年 Janus. All rights reserved.
//

#include <stdio.h>
#include "SudokuSolver.h"

static void show_usgae(const char * name){
    printf("Usage: %s <SudokuTextFilePath> \n", name);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc < 2) {
        show_usgae(argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1],"r");
    if( !fp ) {
        printf("File does not exist! \n");
        return 1;
    }
    readFile(argv[1]);
    printf("🤖 Terminal \n");
    freeAll();
    return 0;
}
