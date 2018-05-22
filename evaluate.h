//
//  evaluate.h
//  F4
//
//  Created by Bear on 17/7/18.
//  Copyright (c) 2017年 Bear. All rights reserved.
//

#ifndef F4_evaluate_h
#define F4_evaluate_h

#define PARAM_NUM 46
#define VAR_NUM 34
#define LINEAR_NUM 35
#define NONLINEAR_NUM 45


typedef long value_t; // to save values of variables.

//typedef unsigned long constpart_t; // the part with no parameters.
typedef unsigned long linearpart_t; // to save 32 unknowns and 1 contants.
typedef unsigned long squarepart_t;

typedef unsigned long oripoly_t;





/// functions

static inline void binary_print(value_t val, int len) {
    
    for (int i = 0; i < len; i++) {
        if (val & ((value_t)1 << i)) {
            printf("1");
        } else {
            printf("0");
        }
        
        if ((i + 1) % 5 == 0) {
            printf(" ");
        }
    }
    
}


static inline int largestpos(value_t val, int len) {
    

    
    for (int i = len - 1; i >= 0; i--) {
        if (val & ((value_t)1 << i)) {
            return i;
        }
    }
    
    return -1;
}



static inline void print_status(linearpart_t *status, int poly_num, int word_len) {
    
    for (int i = 0; i < poly_num; i++) {
        binary_print(status[i], word_len);
        
        if (i < poly_num - 1) {
            printf("\n");
        } else {
            printf("\n");
        }
    }
    
}





#endif
