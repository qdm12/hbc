#include "TEST_CIRC_ARITHM.h"

TEST_CIRC_ARITHM::TEST_CIRC_ARITHM(unsigned number_bits, bool dbg){
	debug = dbg;
	bits = number_bits;
	if (bits < 2){
		cout << className() << ": Not enough bits (" << bits << ")";
		cout << " for some unit tests (average and euclidean division).";
		cout << " Those will be disabled." << endl;
	}
	set_params();
	he.debug_on(debug);
}
const string TEST_CIRC_ARITHM::className(){
	return "TEST_CIRC_ARITHM";
}
void TEST_CIRC_ARITHM::t_start(){
	if (debug){
		t.start();
	}
}
void TEST_CIRC_ARITHM::t_end(string name){
	if (debug){
		double duration = t.end("silent");
		cout << className() << ": " << name << " - " << nslots << " operations done in " << duration << "s" << endl;
		cout << className() << ": " << name << " - Time for each operation: " << 1000*duration/nslots << "ms" << endl;
	}
}

Errors TEST_CIRC_ARITHM::test(){
	cout << className() << ": Number of bits n was set to " << bits << endl;
    Errors e("TEST_CIRC_ARITHM");
	e.add("MULTIPLY arithmetic circuit", test_MULTIPLY());
	e.add("FAST AVERAGES arithmetic circuit", test_FAVERAGES());
	if (bits > 1){
		e.add("DIVIDE arithmetic circuit", test_DIVIDE());
		e.add("AVERAGES arithmetic circuit", test_AVERAGES());
	}
	return e;
}

void TEST_CIRC_ARITHM::set_params(){
	params.p = 2;
	params.r = 1;
	params.d = 0;
	params.k = 128;
	params.c = 3;
	params.w = 64;
	params.slb = 800;
}


bool TEST_CIRC_ARITHM::test_MULTIPLY(){
	params.L = 0; //overwrite of the level parameter
	if(params.L == 0){ //L not set
		params.L = 60; //super deep circuit for more bits (takes a while)

		switch(bits){
			case 2:
				params.L = 8; //for 1024 slots
				break;
			case 3:
				params.L = 13; //for 1024 slots
				break;
			case 4:
				params.L = 17; //for 1024 slots
				break;
			case 5:
				params.L = 22; //for 1024 slots
				break;
			case 6: 
				params.L = 28; //for 1800 slots - 229.265
				break;
			case 7: //for 1800 slots
				params.L = 32; //33 - 349.625s
				break;
			case 8: //for 1800 slots
				params.L = 37;
				break;
			case 9:
				params.L = 41;
				break;
				
		}
	}
	params.m = FindM(params.k,params.L,params.c,params.p,params.d,params.slb,0);
	nslots = he.keyGen(params);
	mkt k_ones = he.setOnes(nslots);
	he.set01(k_ones);
	inputs = vector< vector<long> >(2, vector<long>(nslots,0));
	v_in = vector< vector < vector<long> > >(2, vector < vector < long > > (bits, vector < long > (nslots,0)));
	for(unsigned n = 0; n < k.size(); n++){
		for (unsigned b = 0; b < k[n].size(); b++){
			he.erase(k[n][b]);
		}
	}
	k = vector< vector<mkt> >(2, vector <mkt>(bits));

	for(unsigned i = 0; i < nslots; i++){
		inputs[0][i] = rand() % (unsigned)pow(2,bits);
		inputs[1][i] = rand() % (unsigned)pow(2,bits);
	}
	
	//Converts inputs to bits into v_in for parallel ciphertexts
	for(unsigned n = 0; n < 2; n++){
		for(unsigned j = 0; j < nslots; j++){
			bitset<64> bin(inputs[n][j]); //max is 2^64 so max nbits = 64
			for(unsigned b = 0; b < bits; b++){
				v_in[n][b][j] = bin[b]; //first ctxt (b = 0) is LSB
			}
		}
	}
	
	//Encrypts all the vectors into ciphertexts
	if(debug){
		cout << className() << ": " << __FUNCTION__ << ": Encrypting input vectors (" << 2 * bits << " vectors)" << endl;
	}
	for(unsigned n = 0; n < 2; n++){
		for (unsigned b = 0; b < bits; b++){
			k[n][b] = he.encrypt(v_in[n][b]);
		}
	}
	
	if(debug){
		cout << className() << ": " << __FUNCTION__ << ": " << bits << " bits, with L=" << params.L << endl;
	}
	
	
	vector<long> results(nslots);
	t_start();
	he.MULTIPLY(k[0], k[1]);
	t_end(__FUNCTION__);
	results = conv.matrix2LongVec(he.decryptNbits(k[0]));
	for (unsigned i = 0; i < nslots; i++){
		if (results[i] != (inputs[0][i] * inputs[1][i])){	
			return true;
		}
	}
	return false;
}
bool TEST_CIRC_ARITHM::test_DIVIDE(){
	params.L = 0; //overwrite of the level parameter
	if(params.L == 0){ //L not set
		//only valid for 1024 plaintext slots
		switch(bits){
			case 2:
				params.L = 12;
				break;
			case 3:
				params.L = 21;
				break;
			case 4:
				params.L = 50;
				break;
			case 5:
				params.L = 55;
				break;
			case 6: //This requires a level of more than 65
				//usually needs too much ram
				//bootstrapping should thus be used here
				params.L = -1;
				cout << "6 bits and more not supported." << endl;
				return true;
				break;
		}
	}
	params.m = FindM(params.k,params.L,params.c,params.p,params.d,params.slb,0);
	nslots = he.keyGen(params);
	mkt k_ones = he.setOnes(nslots);
	he.set01(k_ones);
	inputs = vector< vector<long> >(2, vector<long>(nslots,0));
	v_in = vector< vector < vector<long> > >(2, vector < vector < long > > (bits, vector < long > (nslots,0)));
	for(unsigned n = 0; n < k.size(); n++){
		for (unsigned b = 0; b < k[n].size(); b++){
			he.erase(k[n][b]);
		}
	}
	k = vector< vector<mkt> >(2, vector <mkt>(bits));

	for(unsigned i = 0; i < nslots; i++){
		inputs[0][i] = rand() % (unsigned)pow(2,bits);
		while(inputs[0][i] == 0){ //for division
			inputs[0][i] = rand() % (unsigned)pow(2,bits);
		}
		inputs[1][i] = rand() % (unsigned)pow(2,bits);
		while((inputs[1][i] == 0) || (inputs[1][i] > inputs[0][i])){ //for division
			inputs[1][i] = rand() % (unsigned)pow(2,bits);
		}
	}
	
	//Converts inputs to bits into v_in for parallel ciphertexts
	for(unsigned n = 0; n < 2; n++){
		for(unsigned j = 0; j < nslots; j++){
			bitset<64> bin(inputs[n][j]); //max is 2^64 so max nbits = 64
			for(unsigned b = 0; b < bits; b++){
				v_in[n][b][j] = bin[b]; //first ctxt (b = 0) is LSB
			}
		}
	}
	//Encrypts all the vectors into ciphertexts
	if(debug){
		cout << className() << ": " << __FUNCTION__ << ": Encrypting input vectors (" << 2 * bits << " vectors)" << endl;
	}
	for(unsigned n = 0; n < 2; n++){
		for (unsigned b = 0; b < bits; b++){
			k[n][b] = he.encrypt(v_in[n][b]);
		}
	}
	
	if(debug){
		cout << className() << ": " << __FUNCTION__ << ": " << bits << " bits, with L=" << params.L << endl;
	}
	
	
	
	vector<long> q(nslots), r(nslots);
	t_start();
	he.DIVIDE(k[0], k[1]);
	t_end(__FUNCTION__);
	q = conv.matrix2LongVec(he.decryptNbits(k[0]));
	r = conv.matrix2LongVec(he.decryptNbits(k[1]));
	ldiv_t expected;
	for (unsigned i = 0; i < nslots; i++){
		expected = div(inputs[0][i], inputs[1][i]);
		if ((expected.rem != r[i]) || (expected.quot != q[i])){
			return true;
		}
	}
	return false;
}
bool TEST_CIRC_ARITHM::test_AVERAGES(){
	long N_NUMBERS = 2;
	params.L = 0; //overwrite of the level parameter
	if(params.L == 0){ //L not set
		//only valid for 1024 plaintext slots, 2 numbers
		switch(bits){
			case 2:
				params.L = 26;
				break;
			case 3:
				params.L = 45;
				break;
			case 4:
				params.L = 65; //more than 65
				break;
			case 5:
				params.L = 100; //bootstrapping should be used
				break;
			case 6:
				params.L = 100; //bootstrapping should be used
				break;
		}
	}
	
	
	params.m = FindM(params.k,params.L,params.c,params.p,params.d,params.slb,0);
	nslots = he.keyGen(params);
	mkt k_ones = he.setOnes(nslots);
	he.set01(k_ones);
	v_in = vector< vector < vector<long> > >(N_NUMBERS, vector < vector < long > > (bits, vector < long > (nslots,0)));
	for(unsigned n = 0; n < k.size(); n++){
		for (unsigned b = 0; b < k[n].size(); b++){
			he.erase(k[n][b]);
		}
	}
	k = vector< vector<mkt> >(N_NUMBERS, vector <mkt>(bits));
	inputs = vector< vector<long> >(N_NUMBERS, vector<long>(nslots,0));

	
	//Generation of inputs
	for(unsigned n = 0; n < N_NUMBERS; n++){
		for(unsigned i = 0; i < nslots; i++){
			inputs[n][i] = rand() % (unsigned)pow(2,bits);
			while(inputs[n][i] == 0){
				inputs[n][i] = rand() % (unsigned)pow(2,bits);
				//to be sure A>B for euclidean division XXX
			}
		}
	}
	vector<long> vector_divisor(nslots, N_NUMBERS);
	
	//Converts inputs to bits into v_in for parallel ciphertexts
	for(unsigned n = 0; n < N_NUMBERS; n++){
		for(unsigned j = 0; j < nslots; j++){
			bitset<64> bin(inputs[n][j]); //max is 2^64 so max nbits = 64
			for(unsigned b = 0; b < bits; b++){
				v_in[n][b][j] = bin[b];
			}
		}
	}
	
	vector< vector<long> > matrix_divisor = conv.longVec2Matrix(vector_divisor);
	unsigned bits_divisor = matrix_divisor.size();
	
	//Encrypts the input matrices into ciphertexts
	if(debug){
		cout << className() << ": " << __FUNCTION__ << ": Encrypting input vectors (" << N_NUMBERS * bits + bits_divisor<< " vectors)" << endl;
		cout << className() << ": " << __FUNCTION__ << ": " << bits << " bits, with L=" << params.L << endl;
	}
	for(unsigned n = 0; n < N_NUMBERS; n++){
		for (unsigned b = 0; b < bits; b++){
			k[n][b] = he.encrypt(v_in[n][b]);
		}
	}
	vector<mkt> k_divisor(bits_divisor);
	for(unsigned b = 0; b < bits_divisor; b++){
		k_divisor[b] = he.encrypt(matrix_divisor[b]);
	}
	
	vector<long> q(nslots), r(nslots);
	t_start();
	he.AVERAGES(k, k_divisor);
	t_end(__FUNCTION__);
	q = conv.matrix2LongVec(he.decryptNbits(k[0]));
	r = conv.matrix2LongVec(he.decryptNbits(k_divisor));
	ldiv_t expected;
	unsigned expected_sum;
	for (unsigned i = 0; i < nslots; i++){
		expected_sum = 0;
		for (unsigned n = 0; n < N_NUMBERS; n++){
			expected_sum += inputs[n][i];
		}
		expected = div(expected_sum, N_NUMBERS);
		if ((expected.rem != r[i]) || (expected.quot != q[i])){
			return true;
		}
	}
	return false;
}


bool TEST_CIRC_ARITHM::test_FAVERAGES(){
	long N_NUMBERS = 4;
	params.L = 0; //overwrite of the level parameter
	if(params.L == 0){ //L not set
		params.L = 45; //for bigger numbers for instance (not tested)
		//only valid for 1024 plaintext slots, 4 numbers
		switch(bits){
			case 2:
				params.L = 11;
				break;
			case 3:
				params.L = 13;
				break;
			case 4:
				params.L = 15;
				break;
			case 5:
				params.L = 17;
				break;
			case 6:
				params.L = 19;
				break;
			case 7:
				params.L = 21;
				break;
			case 8:
				params.L = 26;
				break;
			case 9:
				params.L = 28;
				break;
				
		}
	}
	
	
	params.m = FindM(params.k,params.L,params.c,params.p,params.d,params.slb,0);
	nslots = he.keyGen(params);
	mkt k_ones = he.setOnes(nslots);
	he.set01(k_ones);
	v_in = vector< vector < vector<long> > >(N_NUMBERS, vector < vector < long > > (bits, vector < long > (nslots,0)));
	for(unsigned n = 0; n < k.size(); n++){
		for (unsigned b = 0; b < k[n].size(); b++){
			he.erase(k[n][b]);
		}
	}
	k = vector< vector<mkt> >(N_NUMBERS, vector <mkt>(bits));
	inputs = vector< vector<long> >(N_NUMBERS, vector<long>(nslots,0));

	
	//Generation of inputs
	for(unsigned n = 0; n < N_NUMBERS; n++){
		for(unsigned i = 0; i < nslots; i++){
			inputs[n][i] = rand() % (unsigned)pow(2,bits);
			while(inputs[n][i] == 0){
				inputs[n][i] = rand() % (unsigned)pow(2,bits);
				//to be sure A>B for euclidean division XXX
			}
		}
	}
	
	//Converts inputs to bits into v_in for parallel ciphertexts
	for(unsigned n = 0; n < N_NUMBERS; n++){
		for(unsigned j = 0; j < nslots; j++){
			bitset<64> bin(inputs[n][j]); //max is 2^64 so max nbits = 64
			for(unsigned b = 0; b < bits; b++){
				v_in[n][b][j] = bin[b];
			}
		}
	}
	
	//Encrypts the input matrices into ciphertexts
	if(debug){
		cout << className() << ": " << __FUNCTION__ << ": Encrypting input vectors (" << N_NUMBERS * bits << " vectors)" << endl;
		cout << className() << ": " << __FUNCTION__ << ": " << bits << " bits, with L=" << params.L << endl;
	}
	for(unsigned n = 0; n < N_NUMBERS; n++){
		for (unsigned b = 0; b < bits; b++){
			k[n][b] = he.encrypt(v_in[n][b]);
		}
	}
	
	vector<long> q(nslots);
	t_start();
	he.FAVERAGES(k);
	t_end(__FUNCTION__);
	q = conv.matrix2LongVec(he.decryptNbits(k[0]));
	ldiv_t expected;
	unsigned expected_sum;
	for (unsigned i = 0; i < nslots; i++){
		expected_sum = 0;
		for (unsigned n = 0; n < N_NUMBERS; n++){
			expected_sum += inputs[n][i];
		}
		expected = div(expected_sum, N_NUMBERS);
		if (expected.quot != q[i]){
			return true;
		}
	}
	return false;
}
