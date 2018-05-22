
#include <limits.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
//#include <sys/malloc.h>

// pthread
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "evaluate.h"



/**
 * return the numer of solutions.
 * solustions are stored in the lowest bits of each solutions[i].
 */
static inline value_t gauss(linearpart_t *working_mat, const int poly_num, const int unknown_num, value_t *solutions)
{
    
    
    int pos[poly_num + 1];
    int rank = 0;
    
    
    
    for (int pi = 0; pi < poly_num; pi++) {
        
        if (working_mat[pi] == 0) {
            continue;
        }
        
        pos[pi] = largestpos(working_mat[pi], unknown_num + 1);
        rank++;
        
        if (pos[pi] == 0) {
            return 0;
        }
        
        
        //print_status(working_mat, poly_num, unknown_num + 1);
        //PR_ENTER
        
        for (int j = pi + 1; j < poly_num; j++) {
            
            if (working_mat[j] & ((linearpart_t)1 << pos[pi])) {
                working_mat[j] ^= (working_mat[pi]);
            }
        }
        
    }
    

  
    
    
    
    // back
    for (int pi = 0; pi < poly_num; pi++) {
        
        if (working_mat[pi] == 0) {
            continue;
        }
        
        for (int j = 0; j < pi; j++) {
            if (working_mat[j] & ((linearpart_t)1 << pos[pi])) {
                working_mat[j] ^= working_mat[pi];
            }
        }
    }
    
    
    
    
    if (rank == unknown_num) {
        
        // only one solution.
        solutions[0] = 0;
        
        for (int pi = 0; pi < poly_num; pi++) {
            
            if (working_mat[pi] == 0) {
                continue;
            }
            
            if (working_mat[pi] & (linearpart_t)1) {
                solutions[0] ^= ((value_t)1 << (pos[pi]));
            }
        }
        
        return 1;
        
    } else {
        
        // now troubles come
        solutions[0] = 0;
        value_t sol_num = 1;
        // bool appear[unknown_num];
        bool appear[unknown_num+1];

        for (int i = 0; i < unknown_num+1; i++) {
            appear[i] = false;
        }
        
        
        for (int pi = 0; pi < poly_num; pi++) {
            
            if (working_mat[pi] == 0) {
                continue;
            }
            
            appear[pos[pi]] = true;
            
            if (working_mat[pi] & (linearpart_t)1) {
                solutions[0] ^= ((value_t)1 << (pos[pi]));
            }
        }
        
        
        
        // duplicate solutions.
        for (int i = 0; i < unknown_num+1; i++) {
            
            if (appear[i] == false) {
                
                for (int j = 0; j < sol_num; j++) {
                    solutions[j + sol_num] = (solutions[j]) ^ ((value_t)1 << i);
                }
                for (int pi = 0; pi < poly_num; pi++) {
		   if (working_mat[pi] == 0) {
			continue;
		   }

		   for (int j = 0; j < sol_num * ((working_mat[pi] & (((linearpart_t) 1) << i)) != 0); j++) {
    		        solutions[j + sol_num] ^= ((value_t) 1 << pos[pi]);
    		   }
                }
                sol_num *= 2;
            }
        }
        
        return sol_num;
        
    }
    
    
}








int main(int argc, const char * argv[]) {
    
    

    

    
    

    
    // generate random matrices.
    const int para_num = PARAM_NUM;
    const int enum_num = 24;
    //const int set_num = para_num - enum_num;
    const value_t set_val = ((value_t)23489300054) << enum_num; 
    const int unknown_num = VAR_NUM;
    const int poly_num = LINEAR_NUM;
    
    const int ori_num = NONLINEAR_NUM;
    
    
    
    
    
    
    // linear part.
    
    // linear part can be unsigned int, if the unknowns are smaller than 31
    linearpart_t linear_mat[para_num][poly_num];
    linearpart_t working_mat[poly_num]; // initialized as the const part of linear matrix. also used as the results of linear part.
    linearpart_t working_mat_copy[poly_num];
    
    squarepart_t square_mat[para_num][poly_num];
    squarepart_t const_mat[poly_num]; // used to compute the const part from square polys.
    
    
    oripoly_t polys[ori_num][para_num + unknown_num + 1][2];
    oripoly_t cstpoly[2];
    
  
	//read matrices files

	FILE *in1 = fopen("./linear_mat.txt", "r+");
	FILE *in2 = fopen("./square_mat.txt", "r+");
	FILE *in3 = fopen("./polys_mat.txt", "r+");
	FILE *in4 = fopen("./working_mat.txt", "r+");


	char c1, c2, c3, c4;
	for (int i = 0; i < para_num; i++) {
		for (int j = 0; j < poly_num; j++) {
			linear_mat[i][j] = 0;
			square_mat[i][j] = 0;

			for (int k = 0; k < unknown_num + 1; k++) {
				fscanf(in1, "%c", &c1);
				while (c1 != '0' && c1 != '1') {
					fscanf(in1, "%c", &c1);
				}
				if (c1 == '1') {
					linear_mat[i][j] ^= ((linearpart_t) 1 << k);
				}
			}
			for (int k = 0; k < para_num; k++) {
				fscanf(in2, "%c", &c2);
				while (c2 != '0' && c2 != '1') {
					fscanf(in2, "%c", &c2);
				}
				if (c2 == '1') {
					square_mat[i][j] ^= ((squarepart_t) 1 << k);
				}
			}
		}
	}


	for (int i = 0; i < ori_num; i++) {
		for (int j = 0; j < para_num + unknown_num + 1; j++) {
			polys[i][j][0] = 0;
			polys[i][j][1] = 0;

			for (int k = 0; k < para_num + unknown_num  + 1; k++) {
				fscanf(in3, "%c", &c3);
				while (c3 != '0' && c3 != '1') {
					fscanf(in3, "%c", &c3);
				}
				if (k < para_num && c3 == '1') {
					polys[i][j][0] ^= ((oripoly_t) 1 << k);
				} else if (k >= para_num && c3 == '1') {
					polys[i][j][1] ^= ((oripoly_t) 1 << (k - para_num));
				}
			}
		}
	}


	for (int i = 0; i < poly_num; i++) {
		working_mat[i] = 0;
		for (int j = 0; j < unknown_num + 1; j++) {
			fscanf(in4, "%c", &c4);
			while (c4 != '0' && c4 != '1') {
				fscanf(in4, "%c", &c4);
			}
			if (c4 == '1') {
				working_mat[i] ^= ((linearpart_t) 1 << j);
			}
		}
	}

	fclose(in1);
	fclose(in2);
	fclose(in3);
	fclose(in4);
  

    
    
    
    
    
    
    
    
    // now start.
    

    
    
    value_t val = set_val, w;    // the initial value of parameters.
    int pos = -1;
    value_t solutions[10];
    value_t sol_num;
    
    int sol_total = 0;
    
    
    
    
    
    ///////// deal with the case with all parameters 0. ////////////
    

    
    // linear part.
   
    
    
    for(int pos = enum_num; pos < para_num;pos++){
        
        
        if (val &((value_t)1<< pos )) {
           
            for (int pi = 0; pi < poly_num; pi++) {
                working_mat[pi] ^= linear_mat[pos][pi];
            }
            
            
            
            for (int pi = 0; pi < poly_num; pi++) {
                const_mat[pi] ^= square_mat[pos][pi];
                
            }

        }
        
        
        
    }
    for (int i = 0; i< poly_num; i++) {
        working_mat_copy[i] = working_mat[i];
    }
    
    
    
    for (int pi = 0; pi < poly_num; pi++) {
        
        
        w = const_mat[pi] & val;
        
        w = (w) ^ (w >> 32);
        w = (w) ^ (w >> 16);
        w = (w) ^ (w >> 8);
        w = (w) ^ (w >> 4);
        w = (w) ^ (w >> 2);
        w = (w) ^ (w >> 1);
        
        
        if (w & (value_t)1) {
            
            working_mat[pi] ^= (linearpart_t)1;
        }
        
    }

    


    // gauss
    sol_num = gauss(working_mat, poly_num, unknown_num, solutions);
    
    
   
    sol_total += sol_num;
    

    
    // verify on ori polys.
    for (int s = 0; s < sol_num; s++) {

	int op;
	for (op = 0; op < ori_num; op++) {

		cstpoly[0] = polys[op][para_num + unknown_num][0];
		cstpoly[1] = polys[op][para_num + unknown_num][1];

	 	// for parameters.
		for (int pa = 0; pa < para_num; pa++) {
			if (val & ((value_t) 1 << pa)) {

				cstpoly[0] ^= polys[op][pa][0];
				cstpoly[1] ^= polys[op][pa][1];
			}
		}

		for (int un = 0; un < unknown_num; un++) {
			if (solutions[s] & ((value_t) 1 << (un + 1))) {
				cstpoly[0] ^= polys[op][un + para_num][0];
				cstpoly[1] ^= polys[op][un + para_num][1];
			}
		}

		// evaluate
		cstpoly[0] = cstpoly[0] & val;
		cstpoly[1] = cstpoly[1] & (solutions[s] | 1);

		value_t w = cstpoly[0] ^ cstpoly[1];

		w = (w) ^ (w >> 32);
		w = (w) ^ (w >> 16);
		w = (w) ^ (w >> 8);
		w = (w) ^ (w >> 4);
		w = (w) ^ (w >> 2);
		w = (w) ^ (w >> 1);
		printf("val:%lu,sol:%lu,op:%d\n",val,solutions[s],op);

		if (w & (value_t) 1) {
			break;
		}
	}



	if (op == ori_num) {
		printf(" we have done !!! %ld,   %ld\n", val, solutions[s]);
		exit(1);
	}

    }

    
    
    //////// end case all 0 ////////
    
    
    
    
    
    
    
    
    
    
    
    

    
    
    ///////// deal with the case with not all parameters 0. ////////////
    

    
    // main loop.
    for (value_t count = 1; count < (1 << enum_num); count++) {
       
        // generate the next gray code
        pos = largestpos(count ^ (count - 1), enum_num);
        val = val ^ ((value_t)1 << pos);
  
        
        
        
        
        
        // linear part
        for (int pi = 0; pi < poly_num; pi++) {
            working_mat_copy[pi] ^= linear_mat[pos][pi];
           
        }
        
        
        
        // square part
        for (int pi = 0; pi < poly_num; pi++) {
            const_mat[pi] ^= square_mat[pos][pi];
           
        }


        for (int pi = 0; pi < poly_num; pi++) {
            working_mat[pi] = working_mat_copy[pi];
        }
        
        
       
        
        
        for (int pi = 0; pi < poly_num; pi++) {
       
            
            w = const_mat[pi] & val;
 
            w = (w) ^ (w >> 32);
            w = (w) ^ (w >> 16);
            w = (w) ^ (w >> 8);
            w = (w) ^ (w >> 4);
            w = (w) ^ (w >> 2);
            w = (w) ^ (w >> 1);

            
            if (w & (value_t)1) {
               
                working_mat[pi] ^= (linearpart_t)1;
            }
            
        }
       

        
        // gauss
        sol_num = gauss(working_mat, poly_num, unknown_num, solutions);
        sol_total += sol_num;

        // verify on ori polys.
        for (int s = 0; s < sol_num; s++) {




		int op;

		for (op = 0; op < ori_num; op++) {

			cstpoly[0] = polys[op][para_num + unknown_num][0];
			cstpoly[1] = polys[op][para_num + unknown_num][1];

			// for parameters.
			for (int pa = 0; pa < para_num; pa++) {

				if (val & ((value_t) 1 << pa)) {

					cstpoly[0] ^= polys[op][pa][0];
					cstpoly[1] ^= polys[op][pa][1];
				}

			}

			// for unknowns.
			for (int un = 0; un < unknown_num; un++) {
				if (solutions[s] & ((value_t) 1 << (un + 1))) {
					cstpoly[0] ^= polys[op][un + para_num][0];
					cstpoly[1] ^= polys[op][un + para_num][1];
				}
			}

			// evaluate
			cstpoly[0] = cstpoly[0] & val;
			cstpoly[1] = cstpoly[1] & (solutions[s] | 1);

			value_t w = cstpoly[0] ^ cstpoly[1];

			w = (w) ^ (w >> 32);
			w = (w) ^ (w >> 16);
			w = (w) ^ (w >> 8);
			w = (w) ^ (w >> 4);
			w = (w) ^ (w >> 2);
			w = (w) ^ (w >> 1);

			if (w & (value_t) 1) {
				break;
			}
		}

		printf("val:%lu,sol:%lu,op:%d\n",val,solutions[s],op);
		if (op == ori_num) {
			printf(" we have done !!! %ld,   %ld\n", val, solutions[s]);
			exit(1);
		}
	}


       
    }
    
    

    
    
   
    
    
    
    
    /// the end
    
    
     
     
   
    

    
}















































