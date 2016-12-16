#ifndef TEST_GATES_H
#define TEST_GATES_H
#include "helper_functions.h"
#include "he.h"

class TEST_GATES {
	public:
		TEST_GATES();
		Errors test();
	private:
		HE he;
		key_params params;
		void set_params();
		long nslots;
		vector< vector<long> > v_in, v_out;
		vector<mkt> k_constant;
		vector<mkt> k;
		unsigned IO; //2 in, 1 out
		void initialize();
		void make_copies();
		bool test_AND();
		bool test_XOR();
		bool test_NOT();
		bool test_NAND();
		bool test_OR();
		bool test_NOR();
		bool test_XNOR();
};
#endif