#include "TEST_CIRC_COMB.h"

TEST_CIRC_COMB::TEST_CIRC_COMB(bool dbg){
	debug = dbg;
	set_params();
	initialize();
}
const string TEST_CIRC_COMB::className(){
	return "TEST_CIRC_COMB";
}
Errors TEST_CIRC_COMB::test(){
	Errors e("TEST_CIRC_COMB");
	e.add("HALF ADDER combinational circuit", test_HFADDER());
	e.add("FULL ADDER 1 bit circuit", test_FLADDER());
	e.add("HALF SUBTRACTOR combinational circuit", test_HFSUBER());
	e.add("FULL SUBTRACTOR combinational circuit", test_FLSUBER());
	e.add("HFEQUAL combinational circuit", test_HFEQUAL());
	e.add("FLEQUAL combinational circuit", test_FLEQUAL());
	e.add("SMALLER combinational circuit", test_SMALLER());
	e.add("HFCMP combinational circuit", test_HFCMP());
	e.add("FLCMP combinational circuit", test_FLCMP());
	e.add("MUX combinational circuit", test_MUX());
	return e;
}

void TEST_CIRC_COMB::set_params(){
	params.p = 2;
	params.r = 1;
	params.d = 0;
	params.k = 128;
	params.L = 15;
	params.c = 3;
	params.w = 64;
	params.slb = 0;
	params.m = FindM(params.k,params.L,params.c,params.p,params.d,params.slb,0);
}
void TEST_CIRC_COMB::initialize(){
	bits = 3; //2 to 3 inputs for all combinational circuits
	he.debug_on(debug);
	nslots = he.keyGen(params);
	mkt k_ones = he.setOnes(nslots);
	he.set01(k_ones);
	k_constant.resize(bits);
	v_in.resize(bits, vector<long> (nslots,0));
	for(int i = 0; i < nslots; i++){
		bitset<64> bin(i % (unsigned)pow(2,bits)); //max is 2^64
		for(int b = 0; b < bits; b++){
			v_in[b][i] = bin[b]; //fill up matrix of bits
		}
	}
	for (int i = 0; i < bits; i++){
		k_constant[i] = he.encrypt(v_in[i]);
	}
}
void TEST_CIRC_COMB::make_copies(){
	for (unsigned b = 0; b < k.size(); b++){
		he.erase(k[b]);
	}
	k = vector< mkt >(bits);
	for (unsigned b = 0; b < bits; b++){
		k[b] = he.copy(k_constant[b]);
	}
}
bool TEST_CIRC_COMB::test_HFADDER(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.HFADDER(k[0],k[1]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != (v_in[0][i] ^ v_in[1][i])) &&
		   (v_out[1][i] != (v_in[0][i] & v_in[1][i]))){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_FLADDER(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.FLADDER(k[0],k[1],k[2]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != ((v_in[0][i] ^ v_in[1][i]) ^ v_in[2][i])) &&
		   (v_out[1][i] != ((v_in[0][i] & v_in[1][i]) & v_in[2][i]))){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_HFSUBER(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.HFSUBER(k[0],k[1]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != (v_in[0][i] ^ v_in[1][i])) &&
		   (v_out[1][i] != (!v_in[0][i] & v_in[1][i]))){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_FLSUBER(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.FLSUBER(k[0],k[1],k[2]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != ((v_in[0][i] ^ v_in[1][i]) ^ v_in[2][i])) &&
		   (v_out[1][i] != (!(!v_in[0][i] & v_in[1][i]) & v_in[2][i]))){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_HFEQUAL(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.HFEQUAL(k[0],k[1]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if(v_out[0][i] != (v_in[0][i] == v_in[1][i])){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_FLEQUAL(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.FLEQUAL(k[0],k[1],k[2]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if(v_out[0][i] != ((v_in[0][i] == v_in[1][i]) && v_in[2][i])){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_SMALLER(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.SMALLER(k[0],k[1]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if(v_out[0][i] != (v_in[0][i] < v_in[1][i])){
			return true; //error
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_HFCMP(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.HFCMP(k[0],k[1]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != (v_in[0][i] == v_in[1][i])) ||
		   (v_out[1][i] != (v_in[0][i] > v_in[1][i]))){
			return true;
		}
	}
	return false;
}
bool TEST_CIRC_COMB::test_FLCMP(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	vector<long> temp(nslots,0);
	mkt k_zeros = he.encrypt(temp);
	temp = vector<long>(nslots,1);
	mkt k_ones = he.encrypt(temp);
	make_copies();
	he.FLCMP(k[0],k[1], k_zeros, k_zeros);
	//always smaller, so outputs should be NOT EQ and NOT GRE (0, 0)
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != 0) ||
		   (v_out[1][i] != 0)){
			return true;
		}
	}
	make_copies();
	he.FLCMP(k[0],k[1], k_zeros, k_ones);
	//always k1 > k2, so outputs should be NOT EQ and GRE (0, 1)
	v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != 0) ||
		   (v_out[1][i] != 1)){
			return true;
		}
	}
	make_copies();
	he.FLCMP(k[0],k[1], k_ones, k_zeros);
	//bits were equal up to this point, so outputs depends on k[0] and k[1]
	v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if((v_out[0][i] != (v_in[0][i] == v_in[1][i])) ||
		   (v_out[1][i] != (v_in[0][i] > v_in[1][i]))){
			return true;
		}
	}
	// he.FLCMP(k[0],k[1], k_01[1], k_01[1]); should never happen
	return false;
}
bool TEST_CIRC_COMB::test_MUX(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	make_copies();
	he.MUX(k[0],k[1],k[2]);
	vector< vector<long> > v_out = he.decryptNbits(k);
	for (int i = 0; i < nslots; i++){
		if (v_in[2][i] && (v_out[0][i] != v_in[0][i])){
			return true;
		}
		if (!v_in[2][i] && (v_out[0][i] != v_in[1][i])){
			return true;
		}
	}
	return false;
}
