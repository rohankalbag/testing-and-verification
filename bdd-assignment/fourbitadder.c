#include <stdlib.h>
#include <stdio.h>
#include <bdduser.h>

// number of bits in adder


void visualize_bdd(bdd_manager bddm, bdd x){
	printf("----------------------------------------------------\n");
	bdd_print_bdd(bddm,x,NULL, NULL,NULL, stdout); //prints bdd
	printf("----------------------------------------------------\n");
}


int main (int argc, char* argv[])
{
	// create the universe
	
	bdd_manager bddm = bdd_init();	

	// make inputs of adder

	bdd x[4];
	for(int i=0;i<4;i++){
		x[i] = bdd_new_var_last(bddm);
	}

	bdd y[4];
	for(int i=0;i<4;i++){
		y[i] = bdd_new_var_last(bddm);
	}

	// sum and carry of adder

	bdd s[4];
	bdd c[4];

	// first bit of adder 

	s[0] = bdd_xor(bddm, x[0], y[0]);
	c[0] = bdd_and(bddm, x[0], y[0]);

	// subsequent rippling of adder bits

	for(int i=1; i<4; i++){
		s[i] = bdd_xor(bddm, x[i], y[i]);
		bdd temp = bdd_and(bddm, s[i], c[i-1]);
		s[i] = bdd_xor(bddm, s[i], c[i-1]);
		c[i] = bdd_and(bddm, x[i], y[i]);
		c[i] = bdd_or(bddm, c[i], temp);
	}

	bdd cout = bdd_new_var_last(bddm);
	bdd sum[4];

	for(int i=0; i<4; i++){
		sum[i] = bdd_new_var_last(bddm);
	}

	// approach to question 1

	bdd x_has_odd_high_bits = bdd_xor(bddm, x[0], x[1]);
	
	for(int i=2; i<4; i++){
		x_has_odd_high_bits = bdd_or(bddm, x[i], x_has_odd_high_bits);
	} 

	bdd y_has_odd_high_bits = bdd_xor(bddm, y[0], y[1]);
	
	for(int i=2; i<4; i++){
		y_has_odd_high_bits = bdd_or(bddm, y[i], y_has_odd_high_bits);
	} 

	// function P(x) for subset A

	bdd has_odd_high_bits = bdd_and(bddm, x_has_odd_high_bits, y_has_odd_high_bits);

	// function equivalence check y <-> F(x)

	bdd equivalence = bdd_xnor(bddm, cout, c[4 - 1]);
	
	for(int i=0; i<4; i++){
		bdd curr_eq = bdd_xnor(bddm, s[i], sum[i]);
		equivalence = bdd_and(bddm, equivalence, curr_eq); 
	}

	bdd condition = bdd_and(bddm, equivalence, has_odd_high_bits);

	// To get image apply existence quantification 
	// Q(y) = ∃x P(x).(y <-> F(x))

	bdd quantified_vars[9];
	
	for(int i=0; i<4;i++){
		quantified_vars[i] = x[i];
	}

	for(int i=4; i<8; i++){
		quantified_vars[i] = y[i - 4];
	}

	quantified_vars[8] = 0;

	int assoc = bdd_new_assoc(bddm, quantified_vars, 0);
	bdd_assoc(bddm, assoc);
	
	// bdd for the image of the set

	bdd image = bdd_exists(bddm, condition);

	printf("bdd of image\n");
	visualize_bdd(bddm, image);

	bdd_free_assoc(bddm, assoc);

	// approach for question 2

	// bdd for B(y)

	bdd output_odd_high_bits = bdd_xor(bddm, sum[0], cout); 
	
	for(int i=1; i<4; i++){
		output_odd_high_bits = bdd_xor(bddm, sum[i], output_odd_high_bits);
	}
	
	// to find the preimage of set B
	
	condition = bdd_and(bddm, equivalence, output_odd_high_bits); 
	
	// To get pre-image apply existence quantification 
	// S(x) = ∃y B(y).(y <-> F(x))
	
	for(int i=0; i<4;i++){
		quantified_vars[i] = sum[i];
	}

	quantified_vars[4] = cout;

	quantified_vars[5] = 0;

	assoc = bdd_new_assoc(bddm, quantified_vars, 0);
	bdd_assoc(bddm, assoc);
	
	// bdd for the pre-image of the set

	bdd preimage = bdd_exists(bddm, condition);

	printf("bdd of pre-image\n");
	visualize_bdd(bddm, preimage);

	bdd_free_assoc(bddm, assoc);
	
	bdd_quit(bddm);
	return(0);
}