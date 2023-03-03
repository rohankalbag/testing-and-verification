#include <stdlib.h>
#include <stdio.h>
#include <bdduser.h>

int main (int argc, char* argv[])
{
	// Create the universe..
	bdd_manager bddm = bdd_init();	

	// make 4 variables 
	bdd x1 = bdd_new_var_last(bddm);
	bdd x2 = bdd_new_var_last(bddm);
	bdd x3 = bdd_new_var_last(bddm);
	bdd x4 = bdd_new_var_last(bddm);

	bdd f1 = bdd_xor(bddm,x1,x2);
	bdd f2 = bdd_xor(bddm,f1,x3);
	bdd f = bdd_xor(bddm, f2, x4);

	printf("----------------------------------------------------\n");
	bdd_print_bdd(bddm,f,NULL, NULL,NULL, stdout); //prints bdd
	printf("----------------------------------------------------\n");

	// perform existential quantification
	bdd Q[2];
	Q[0] = x1; Q[1] = 0;
	int assoc = bdd_new_assoc(bddm, Q, 0);
	bdd_assoc(bddm, assoc);
	bdd_print_bdd(bddm,f,NULL, NULL,NULL, stdout); //prints bdd
	printf("----------------------------------------------------\n");
	bdd pq = bdd_exists(bddm, f);
	bdd qq = bdd_forall(bddm, f);
	bdd_print_bdd(bddm, pq, NULL, NULL,NULL, stdout);
	printf("----------------------------------------------------\n");
	bdd_print_bdd(bddm, qq, NULL, NULL,NULL, stdout);
	printf("----------------------------------------------------\n");
	return(0);
}

