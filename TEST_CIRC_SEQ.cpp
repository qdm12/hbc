#include "TEST_CIRC_SEQ.h"

TEST_CIRC_SEQ::TEST_CIRC_SEQ(unsigned number_bits, bool dbg){
	bits = number_bits;
	debug = dbg;
	set_params();
	initialize();
}
const string TEST_CIRC_SEQ::className(){
	return "TEST_CIRC_SEQ";
}
void TEST_CIRC_SEQ::t_start(){
	if (debug){
		t.start();
	}
}
void TEST_CIRC_SEQ::t_end(string name){
	if (debug){
		double duration = t.end("silent");
		cout << className() << ": " << name << " - " << nslots << " operations done in " << duration << "s" << endl;
		cout << className() << ": " << name << " - Time for each operation: " << 1000*duration/nslots << "ms" << endl;
	}
}

Errors TEST_CIRC_SEQ::test(){
	Errors e("TEST_CIRC_SEQ");
	e.add("RIPPLE CARRY ADDER sequential circuit", test_RCADDER());
	e.add("RIPPLE BORROW SUBTRACTOR sequential circuit", test_RBSUBER());
	e.add("RIPPLE COMPARATOR sequential circuit", test_RCMP());
	e.add("RIPPLE EQUALITY COMPARATOR sequential circuit", test_REQ());
	e.add("SHIFT RIGHT sequential circuit", test_SHIFTR());
	e.add("SHIFT LEFT sequential circuit", test_SHIFTL());
	e.add("MUX sequential circuit", test_NMUX());
	return e;
}

void TEST_CIRC_SEQ::set_params(){
	params.p = 2;
	params.r = 1;
	params.d = 0; // field p^d
	params.k = 128;
	params.slb = 800;
	params.L = 0;
	if(params.L == 0){ //L not set 
	//for the ripple carry adder so for circuits with complexity up to 3n+1
	//not valid for ripple comparator !
		switch(bits){
			case 1:
				params.L = 5;
				break;
			case 2:
				params.L = 7;
				break;
			case 3:
				params.L = 9;
				break;
			case 4:
				params.L = 12;
				break;
			case 5:
				params.L = 13;
				break;
			case 6:
				params.L = 15;
				break;
			case 7:
				params.L = 17;
				break;
			case 8:
				params.L = 19;
				break;
			case 9:
				params.L = 21;
				break;
			case 10:
				params.L = 23;
				break;
			case 11:
				params.L = 27;
				break;
			case 12:
				params.L = 29;
				break;
			case 13:
				params.L = 31;
				break;
			case 14:
				params.L = 34;
				break;
			case 15:
				params.L = 35;
				break;
			case 16:
				params.L = 37;
				break;
			if(params.L == 0){ // bits not in 1 .. 16
				params.L = 44; //should work with everything
			}
		}
	}
	
	
	
	if ((params.L > 42)&&(params.slb > 600)){
		params.L *= 1.2;
	}
	params.c = 3;
	params.w = 64;
	
	params.m = FindM(params.k,params.L,params.c,params.p,params.d,params.slb,0);
}
void TEST_CIRC_SEQ::initialize(){
	N_numbers = 2;
	he.debug_on(debug);
	cout << className() << ": Number of bits n was set to " << bits << endl; 
	nslots = he.keyGen(params);
	mkt k_ones = he.setOnes(nslots);
	he.set01(k_ones);
	inputs.resize(N_numbers, vector < long > (nslots,0));
	v_in.resize(N_numbers,vector< vector<long> >(bits,vector<long>(nslots,0)));
	k_constant.resize(N_numbers, vector < mkt>(bits));
	
	//inputs to N bit circuits
	for(unsigned i = 0; i < nslots; i++){
		inputs[0][i] = rand() % (unsigned)pow(2,bits);
		inputs[1][i] = rand() % (unsigned)pow(2,bits);
	}
	
	//Converts inputs to bits into v_in for parallel ciphertexts
	for(unsigned n = 0; n < N_numbers; n++){
		for(unsigned j = 0; j < nslots; j++){
			bitset<64> bin(inputs[n][j]); //max is 2^64 so max nbits = 64
			for(unsigned b = 0; b < bits; b++){
				v_in[n][b][j] = bin[b]; //first ctxt (b = 0) is LSB
			}
		}
	}
	
	//Encrypts all the vectors into ciphertexts
	if(debug){
		cout << "TEST_CIRC_SEQ: Encrypting input vectors (" << N_numbers * bits << " vectors)" << endl;
	}
	for(unsigned n = 0; n < N_numbers; n++){
		for (unsigned b = 0; b < bits; b++){
			k_constant[n][b] = he.encrypt(v_in[n][b]);
		}
	}
}
void TEST_CIRC_SEQ::make_copies(){
	for(unsigned n = 0; n < k.size(); n++){
		for (unsigned b = 0; b < k[n].size(); b++){
			he.erase(k[n][b]);
		}
	}
	k = vector< vector<mkt> >(N_numbers, vector<mkt>(bits));
	for(unsigned n = 0; n < N_numbers; n++){
		for (unsigned b = 0; b < bits; b++){
			k[n][b] = he.copy(k_constant[n][b]);
		}
	}
}
bool TEST_CIRC_SEQ::test_RCADDER(){
	//NO 2's complement, MSB is used for "overflow" (last carry out)
	make_copies();
	t_start();
	he.RCADDER(k[0],k[1]);
	t_end(__FUNCTION__);
	vector<long> results(nslots);
	results = conv.matrix2LongVec(he.decryptNbits(k[0]));
	for (unsigned i = 0; i < nslots; i++){ //bit level
		if (results[i] != (inputs[0][i] + inputs[1][i])){
			return true;
		}
	}
	return false;
}

bool TEST_CIRC_SEQ::test_RBSUBER(){
	make_copies();
	string bin_str;
	t_start();
	he.RBSUBER(k[0],k[1]);
	t_end(__FUNCTION__);
	vector<long> results(nslots);
	results = conv.matrix2SignedLongVec(he.decryptNbits(k[0]));
	for (unsigned i = 0; i < nslots; i++){ //bit level
		if (results[i] != (inputs[0][i] - inputs[1][i])){
			return true;
		}
	}
	return false;
}
bool TEST_CIRC_SEQ::test_RCMP(){
	make_copies();
	t_start();
	he.RCMP(k[0],k[1]);
	t_end(__FUNCTION__);
	
	vector< vector<long> > v_out1, v_out2;
	v_out1 = he.decryptNbits(k[0]);
	v_out2 = he.decryptNbits(k[1]);

	for (unsigned i = 0; i < nslots; i++){
		if (v_out1[0][i] != (inputs[0][i] == inputs[1][i])){
			return true;
		}
		if (v_out2[0][i] != (inputs[0][i] > inputs[1][i])){
			return true;
		}
		if ((!v_out1[0][i] && !v_out2[0][i]) != (inputs[0][i] < inputs[1][i])){
			return true;
		}
	}
	return false;
}
bool TEST_CIRC_SEQ::test_REQ(){
	make_copies();
	t_start();
	he.REQ(k[0],k[1]);
	t_end(__FUNCTION__);
	vector<long> results(nslots);
	results = conv.matrix2LongVec(he.decryptNbits(k[0]));
	for (unsigned i = 0; i < nslots; i++){
		if (results[i] != (inputs[0][i] == inputs[1][i])){
			return true;
		}
	}
	return false;
}
bool TEST_CIRC_SEQ::test_SHIFTR(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	//complexity negligible
	make_copies();
	vector<long> results(nslots);
	unsigned nbits = k[0].size();
	for (unsigned shift = 0; shift <= nbits; shift++){
		make_copies();
		he.SHIFTR(k[0],shift);
		results = conv.matrix2LongVec(he.decryptNbits(k[0]));
		for (unsigned i = 0; i < nslots; i++){
			if ((inputs[0][i] >> shift) != results[i]){
				return true;
			}
		}
	}
	return false;
}
bool TEST_CIRC_SEQ::test_SHIFTL(){
	if(debug){
		cout << className() << ": Running " << __FUNCTION__ << "..." << endl;
	}
	//complexity negligible
	make_copies();
	vector<long> results(nslots);
	for (unsigned shift = 0; shift <= bits; shift++){
		make_copies();
		he.SHIFTL(k[0], shift);
		results = conv.matrix2LongVec(he.decryptNbits(k[0]));
		for (unsigned i = 0; i < nslots; i++){
			if (results[i] != (inputs[0][i] << shift)){	
				return true;
			}
		}
	}
	return false;
}
bool TEST_CIRC_SEQ::test_NMUX(){
	vector<long> v_zeros(nslots, 0), v_ones(nslots, 1);
	mkt k_zeros = he.encrypt(v_zeros);
	mkt k_ones = he.encrypt(v_ones);
	vector<long> results(nslots);
	
	make_copies();
	t_start();
	he.NMUX(k[0], k[1], k_zeros);
	t_end(__FUNCTION__);
	results = conv.matrix2LongVec(he.decryptNbits(k[0]));
	for (unsigned i = 0; i < nslots; i++){
		if (results[i] != inputs[1][i]){	
			return true;
		}
	}
	make_copies();
	he.NMUX(k[0], k[1], k_ones);
	results = conv.matrix2LongVec(he.decryptNbits(k[0]));
	for (unsigned i = 0; i < nslots; i++){
		if (results[i] != inputs[0][i]){	
			return true;
		}
	}
	return false;
}

