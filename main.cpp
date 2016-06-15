#include "helper_functions.h"
#include "TEST_GATES.h"
#include "TEST_CIRC_COMB.h"
#include "TEST_CIRC_SEQ.h"
#include "TEST_CIRC_ARITHM.h"


int main(int argc, char **argv)
{
	cout << "======================================\n";
	Timing t_all("Overall");
	t_all.start();
	Errors e("test");
	
	
	TEST_GATES tg;
	e = tg.test();
	e.display();
	
	TEST_CIRC_COMB t_combinational(false);
	e = t_combinational.test();
	e.display();

	TEST_CIRC_SEQ t_sequential(8, true);
	e = t_sequential.test();
	e.display();
	
	//max 3 bits because of average circuit (can be disabled thought)
	TEST_CIRC_ARITHM t_arithmetic(3, true);
	e = t_arithmetic.test();
	e.display();
	
    t_all.end();
	cout << "======================================\n";
	return 0;
}