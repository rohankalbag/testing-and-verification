#include <stdlib.h>
#include <stdio.h>
#include <bdduser.h>

int main (int argc, char* argv[])
{
	// Create the universe..
	bdd_manager bddm = bdd_init();	

	// make 3 variables x0,x1,x2 (in that order)
	bdd x0 = bdd_new_var_last(bddm);
	bdd x1 = bdd_new_var_last(bddm);
	bdd x2 = bdd_new_var_last(bddm);

	// compute y = (x0.x1)+x2
	bdd a  = bdd_and (bddm,x0,x1);	
	bdd y  = bdd_or (bddm,x2,a);	

	// compute z = (x0 + x2).(x1 + x2)
	bdd b = bdd_or (bddm, x0,x2);
	bdd c = bdd_or (bddm, x1,x2);
	bdd z = bdd_and (bddm, b, c);

	

	printf("----------------------------------------------------\n");

	// print y
	bdd_print_bdd(bddm,y,NULL, NULL,NULL, stdout);

	// are z and y the same?
	if (z == y)
	{
		printf("Equal\n");
	}	
	else
	{
		printf("Not Equal\n");
		// print z
		bdd_print_bdd(bddm,z,NULL, NULL,NULL, stdout);
	}


	printf("----------------------------------------------------\n");

	// compute w = (x0 + x2) + (x1 + x2)
	bdd w = bdd_or (bddm, b,c);
	bdd_print_bdd(bddm,w,NULL, NULL,NULL, stdout);

	// are w and y the same? of course not.
	if (w == y)
	{
		printf("Equal\n");
	}	
	else
	{
		// you should get this.
		printf("Not Equal\n");
	}

	printf("----------------------------------------------------\n");

	// Existential quantification example.
	bdd x4 = bdd_new_var_last(bddm);
	bdd p = bdd_and(bddm, x4, z); // (x0 + x2).(x1 + x2).(x4)  
    bdd_print_bdd(bddm,p,NULL, NULL,NULL, stdout);

    printf("----------------------------------------------------\n");

	// first create a new association
	bdd Q[2];
	Q[0] = x0; Q[1] = 0;
	int assoc = bdd_new_assoc(bddm,Q,0);

	// set the association for further
	// operations
	bdd_assoc(bddm,assoc);

	// call bdd_exists
	//  Note: this uses the association
	//    that was most recently set.
	bdd pq = bdd_exists(bddm,p);

	// print the bdd.
	bdd_print_bdd(bddm,pq,NULL, NULL,NULL, stdout);

	return(0);
}
