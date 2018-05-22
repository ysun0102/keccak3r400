# keccak3r400
by Yao Sun(sunyao@iie.ac.cn) and Ting Li(liting@iie.ac.cn). 

This code is used in 'Preimage Attacks on the Round-reduced Keccak with Cross-linear Structures'(https://tosc.iacr.org/index.php/ToSC/article/view/802). 
### Introduction
We did a practical preimage attack on the 3-round instance Keccak [r = 240, c =160, nr = 3] in the Keccak Challenge(https://keccak.team/crunchy_contest.html). 
We formulated 80 quadratic polynomials in 80unknowns from the preimage problem by guessing the values of 31 linear polynomials.
We guessed the values of another 15 linear polynomials to get 50 linear equationstotally by using the cross-linear structures.
Then we obtained 81 linearequations in 80 unknowns. 
Even if there exists a solution to this linear system, it maynot be the true solution of the formulated system.
At last, we verified the solutionby the original 80 polynomials. 
Totally, the attack costs 5 days with 8 GPU cards,and we finally found a true solution as well as a preimage to this Keccak instance.
Here we give the code on CPU since it is easier to deploy and it is tested on Ubuntu 16.04.
### Usage
```
1. cd $KECCAK3R400_ROOT
2. make
3. ./keccak400
```
Please note that, this code is the demo and it may not find the right solution.
### Data format
To speed up the traversing process, we rewrite the original polynomials as binary string in several parts. To illustrate clearly, we give 
the original polynomials in file'original polys.txt'. In this file, the values of x0-x46 are guessed and the other variables with subscript 
greater than 100 are unknowns. In the 80 polynomials, if we guessed the 46 values, 35 polynomials are linear and these linear polynomials 
are stored in linear_mat.txt,working_mat.txt and square_mat.txt. The 45 quadratic polynomials are stored in polys_mat.txt.

For example:
```
x1*(x2+x3+x4+x200+x201+x202)+x2*(x3+x4+x202+1)+x3+x4+x200+x201+1.

(x200+x201+x202),(x202+1),1 and 1 are stored in linear_mat.txt.

(x200+x201+1) are stored in working_mat.txt.

(x2+x3+x4),(x3+x4),0 and 0 are stored in square_mat.txt.

(x2+x3+x4+x200+x201+x202),(x3+x4+x202+1),1,1,(x200+x201+1) are stored in polys_mat.txt.
```
### Citation

If this code is helpful, please consider citing:
```
@articleToSC802{fse2018,
	author = {Ting Li and Yao Sun and Maodong Liao and Dingkang Wang},
	title = {Preimage Attacks on the Round-reduced Keccak with Cross-linear Structures},
	journal = {IACR Transactions on Symmetric Cryptology},
	volume = {2017},
	number = {4},
	year = {2017},
	keywords = {Cryptanalysis; SHA-3; Keccak; Preimage attacks},
	issn = {2519-173X},
	pages = {39--57},
	doi = {10.13154/tosc.v2017.i4.39-57},
	url = {https://tosc.iacr.org/index.php/ToSC/article/view/802}
}
```
