#ifndef TEST_CIRC_SEQ_H
#define TEST_CIRC_SEQ_H
#include "helper_functions.h"
#include "he.h"
#include <bitset>
#include <stdlib.h> //rand

class TEST_CIRC_SEQ {
	public:
		TEST_CIRC_SEQ(unsigned bits, bool dbg);
		Errors test();
	private:
		const string className();
		bool debug;
		unsigned bits; //number of bits
		HE he;
		Timing t;
		Conversion conv;
		unsigned N_numbers;
		long nslots;
		key_params params;
		vector< vector < long > > inputs;
		vector< vector < vector<long> > >  v_in;
		vector< vector < mkt > > k_constant, k;
		void t_start();
		void t_end(string name);
		void initialize();
		void set_params();
		void make_copies();
		bool test_RCADDER();
		bool test_RBSUBER();
		bool test_RCMP();
		bool test_REQ();
		bool test_SHIFTR();
		bool test_SHIFTL();
		bool test_NMUX();
};
#endif