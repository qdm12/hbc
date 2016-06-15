#ifndef TEST_CIRC_ARITHM_H
#define TEST_CIRC_ARITHM_H
#include "helper_functions.h"
#include "he.h"
#include <bitset>
#include <stdlib.h> //rand

class TEST_CIRC_ARITHM{
	public:
		TEST_CIRC_ARITHM(unsigned bits, bool dbg);
		Errors test();
	private:
		bool debug;
		Timing t;
		const string className();
		void t_start();
		void t_end(string name);
		HE he;
		Conversion conv;
		key_params params;
		long nslots;
		unsigned bits; //number of bits
		unsigned N_numbers;
		vector< vector < long > > inputs;
		vector< vector < vector<long> > >  v_in;
		vector< vector < mkt > > k_constant;
		vector< vector < mkt > > k;
		void set_params();
		bool test_MULTIPLY();
		bool test_DIVIDE();
		bool test_AVERAGES();
		bool test_FAVERAGES();
};
#endif