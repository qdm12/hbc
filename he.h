#ifndef HE_H
#define HE_H
#include "FHE.h"
#include "EncryptedArray.h"
#include <boost/unordered_map.hpp>
#include "helper_functions.h" //generate_string, mkt, key_params


class HE{
	private:
		bool debug;
		const string className();
		//COMMON
		EncryptedArray* ea;
		vector<mkt> map_keys;
		boost::unordered_map<mkt, Ctxt> map_ctxts;
		
		//SERVER SIDE
		mkt k_ones;
		mkt k_zeros;
	
		//CLIENT SIDE
		FHEcontext* context;
		FHESecKey* secretKey;
		const FHEPubKey* publicKey;

		//COMMON
		mkt generate_map_key();
		mkt storeCtxt(Ctxt* ctxt);
		
		//CLIENT SIDE
		bool ctxt_exists(mkt k);
		
		//CLIENT SIDE or COMMON
		long getSlots();
		
	public:
		HE();
		void debug_on(bool on);
	
		//CLIENT SIDE
		long keyGen(key_params params);
		
		//CLIENT SIDE
		mkt encrypt(vector<long> ptxt_vect);
		vector<long> decrypt(mkt k);
		vector< vector<long> > decryptNbits(vector<mkt> k);
		
		
		//CLIENT
		mkt setOnes(long n);
		//SERVER SIDE
		void set01(mkt k_ones);
		
		//SERVER SIDE - All the following...
		mkt copy(mkt k);
		void replace(mkt k, mkt k_replacement);
		void erase(mkt k);
		
		/* HElib WRAPPER */
		/* Arithmetic operations in the ciphertexts map (overrides previous data) */
		void add(mkt k1, mkt k2);
		void sub(mkt k1, mkt k2);
		void mul(mkt k1, mkt k2);
		void mul(mkt k1, mkt k2, mkt k3);
		void neg(mkt k1);
		bool eq(mkt k1, mkt k2);
		
		/*LOGIC GATES */
		void AND(mkt k1, mkt k2);
		void AND(mkt k1, mkt k2, mkt k3);
		void XOR(mkt k1, mkt k2);
		void NOT(mkt k1);
		void NAND(mkt k1, mkt k2);
		void OR(mkt k1, mkt k2);
		void NOR(mkt k1, mkt k2);
		void XNOR(mkt k1, mkt k2);
		
		/* COMBINATIONAL CIRCUITS */
		void HFADDER(mkt k1, mkt k2);
		void FLADDER(mkt k1, mkt k2, mkt k_Cin);
		void HFSUBER(mkt k1, mkt k2);
		void FLSUBER(mkt k1, mkt k2, mkt k_Bin);
		void HFEQUAL(mkt k1, mkt k2);
		void FLEQUAL(mkt k1, mkt k2, mkt k_eq);
		void SMALLER(mkt k1, mkt k2);
		void HFCMP(mkt k1, mkt k2);
		void FLCMP(mkt k1, mkt k2, mkt k_eq, mkt k_gre);
		void MUX(mkt k1, mkt k2, mkt ks);
		
		
		
		/* N BIT CIRCUITS (2 * (N + 1) ciphertexts) */	
		void RCADDER(vector<mkt> &k1_bit, vector<mkt> k2_bit);
		void RBSUBER(vector<mkt> &k1_bit, vector<mkt> k2_bit);
		void RCMP(vector<mkt> &k1_bit, vector<mkt> &k2_bit);
		void NMUX(vector<mkt> k1, vector<mkt> k2, mkt ks);
		void REQ(vector<mkt> &k1_bit, vector<mkt> k2_bit);
		void SHIFTR(vector<mkt> k1_bit, const unsigned shift);
		void SHIFTL(vector<mkt> &k1_bit, const unsigned shift);
		
		unsigned PAD_BITS(vector<mkt> &k1_bit, vector<mkt> &k2_bit);
		
		void MULTIPLY(vector<mkt> &k1_bit, vector<mkt> k2_bit);
		void DIVIDE(vector<mkt> &x, vector<mkt> &y);
		void AVERAGES(vector< vector<mkt> > &numbers, vector<mkt> &N);
		void FAVERAGES(vector< vector<mkt> > &numbers);
};

#endif