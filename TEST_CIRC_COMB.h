#ifndef TEST_CIRC_COMB_H
#define TEST_CIRC_COMB_H
#include "helper_functions.h"
#include "he.h"
#include <bitset>

class TEST_CIRC_COMB {
	public:
		TEST_CIRC_COMB(bool dbg);
		Errors test();
	private:
		const string className();
		Timing t;
		bool debug;
		unsigned bits;
		long nslots;
		HE he;
		key_params params;
		vector< vector<long> > v_in;
		vector<mkt> k_constant, k;
		void initialize();
		void set_params();
		void make_copies();
		bool test_HFADDER();
		bool test_FLADDER();
		bool test_HFSUBER();
		bool test_FLSUBER();
		bool test_HFEQUAL();
		bool test_FLEQUAL();
		bool test_SMALLER();
		bool test_HFCMP();
		bool test_FLCMP();
		bool test_MUX();
};
#endif