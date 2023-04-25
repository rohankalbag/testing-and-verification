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
		x_has_odd_high_bits = bdd_xor(bddm, x[i], x_has_odd_high_bits);
	} 

	bdd y_has_odd_high_bits = bdd_xor(bddm, y[0], y[1]);
	
	for(int i=2; i<4; i++){
		y_has_odd_high_bits = bdd_xor(bddm, y[i], y_has_odd_high_bits);
	} 

	// function P(x, y) for subset A

	bdd has_odd_high_bits = bdd_and(bddm, x_has_odd_high_bits, y_has_odd_high_bits);

	// function equivalence check c, s <-> F(x, y)

	bdd equivalence = bdd_xnor(bddm, cout, c[4 - 1]);
	
	for(int i=0; i<4; i++){
		bdd curr_eq = bdd_xnor(bddm, s[i], sum[i]);
		equivalence = bdd_and(bddm, equivalence, curr_eq); 
	}

	bdd condition = bdd_and(bddm, equivalence, has_odd_high_bits);

	// To get image apply existence quantification 
	// Q(c, s) = ∃ x,y P(x, y).(c, s <-> F(x))

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

	printf("bdd of image, where var.8 - cout\n");
	printf("var.9 - sum[0], var.10 - sum[1], var.11 - sum[2], var.12 - sum[3]\n");
	visualize_bdd(bddm, image);

	bdd_free_assoc(bddm, assoc);

	// approach for question 2

	// bdd for B(s, c)

	bdd output_odd_high_bits = bdd_xor(bddm, sum[0], cout); 
	
	for(int i=1; i<4; i++){
		output_odd_high_bits = bdd_xor(bddm, sum[i], output_odd_high_bits);
	}
	 
	// to find the preimage of set B
	
	condition = bdd_and(bddm, equivalence, output_odd_high_bits); 
	
	// To get pre-image apply existence quantification 
	// S(x, y) = ∃s,c B(s, c).(s, c <-> F(x, y))
	
	for(int i=0; i<4;i++){
		quantified_vars[i] = sum[i];
	}

	quantified_vars[4] = cout;

	quantified_vars[5] = 0;

	assoc = bdd_new_assoc(bddm, quantified_vars, 0);
	bdd_assoc(bddm, assoc);
	
	// bdd for the pre-image of the set

	bdd preimage = bdd_exists(bddm, condition);

	printf("bdd of pre-image, where var.0 - x[0]\n");
	printf("var.1 - x[1], var.2 - x[2], var.3 - x[3], var.4 - y[0]\n");
	printf("var.5 - y[1], var.6 - y[2], var.7 - y[3]\n");
	visualize_bdd(bddm, preimage);

	bdd_free_assoc(bddm, assoc);

	// approach for question 3

	bdd even_four_bit_num = bdd_and(bddm, bdd_not(bddm, sum[0]), bdd_or(bddm, sum[2], sum[3])); // R(s)

	bdd x_is_prime = bdd_or(bddm, bdd_and(bddm, x[0], bdd_xor(bddm, x[1], x[2])), bdd_and(bddm, bdd_not(bddm, x[3]), bdd_or(bddm, bdd_and(bddm, x[0], x[2]), bdd_and(bddm, x[1], bdd_not(bddm, x[2])))));
	bdd y_is_prime = bdd_or(bddm, bdd_and(bddm, y[0], bdd_xor(bddm, y[1], y[2])), bdd_and(bddm, bdd_not(bddm, y[3]), bdd_or(bddm, bdd_and(bddm, y[0], y[2]), bdd_and(bddm, y[1], bdd_not(bddm, y[2])))));
	bdd x_is_prime_and_y_is_prime = bdd_and(bddm, x_is_prime, y_is_prime); // M(x, y)

	// s <-> F(x, y)
	equivalence = bdd_xnor(bddm, s[0], sum[0]);

	for(int i=1; i<4; i++){
		equivalence = bdd_and(bddm, equivalence, bdd_xnor(bddm, s[i], sum[i]));
	}

	for(int i=0; i<4;i++){
		quantified_vars[i] = x[i];
	}

	for(int i=4; i<8; i++){
		quantified_vars[i] = y[i - 4];
	}

	quantified_vars[8] = 0;

	assoc = bdd_new_assoc(bddm, quantified_vars, 0);
	bdd_assoc(bddm, assoc);

	// M(x,y).(s <-> F(x, y))

	condition = bdd_and(bddm, equivalence, x_is_prime_and_y_is_prime);
	
	// ∃x,y M(x,y).(s <-> F(x, y))
	bdd exists = bdd_exists(bddm, condition);

	// R(s) -> ∃x,y M(x,y).(s <-> F(x, y))
	bdd implication = bdd_or(bddm, exists, bdd_not(bddm, even_four_bit_num));

	bdd_free_assoc(bddm, assoc);

	// to prove for all outputs
	for(int i=0;i<4;i++){
		quantified_vars[i] = sum[i];
	}

	quantified_vars[4] = 0;

	assoc = bdd_new_assoc(bddm, quantified_vars, 0);
	bdd_assoc(bddm, assoc);

	// ∀s [ R(s) -> ∃x,y M(x,y).(s <-> F(x, y)) ]
	bdd final = bdd_forall(bddm, implication);

	if(final == bdd_one(bddm)){
		printf("The claim is correct, Hence Proved!\n");
	}
	else{
		printf("The claim is incorrect, Hence Disproved!\n");
	}

	bdd_free_assoc(bddm, assoc);
	bdd_quit(bddm);
	return(0);
}