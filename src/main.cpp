#include "helper_functions.h"
#include "TEST_GATES.h"
#include "TEST_CIRC_COMB.h"
#include "TEST_CIRC_SEQ.h"
#include "TEST_CIRC_ARITHM.h"


int main(int argc, char **argv)
{
	cout << "======================================\n";
	bool verbose = false;
	Timing t_all("Overall");
	t_all.start();
	Errors e("test");
	
	TEST_GATES tg;
	e = tg.test();
	e.display();
	
	TEST_CIRC_COMB t_combinational(verbose);
	e = t_combinational.test();
	e.display();

	TEST_CIRC_SEQ t_sequential(4, verbose);
	e = t_sequential.test();
	e.display();
	
	//max 3 bits because of average circuit (can be disabled thought)
	TEST_CIRC_ARITHM t_arithmetic(3, verbose);
	e = t_arithmetic.test();
	e.display();
	
    t_all.end();
	cout << "======================================\n";
	return 0;
}