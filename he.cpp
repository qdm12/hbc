#include "he.h"

HE::HE(){}
const string HE::className(){
	return "HE";
}


//COMMON
mkt HE::storeCtxt(Ctxt* ctxt) {
/*	Takes a pointer to a ciphertext object and stores this one with a 
	unique and random key generated in the unordered map map_ctxts. 
	The key generated is then returned. */
	mkt k = generate_map_key();
	map_ctxts.insert(make_pair(k, *ctxt));
	return k;
}
mkt HE::generate_map_key(){
	mkt k = generate_string(16);
	while(find(map_keys.begin(), map_keys.end(), k) != map_keys.end()){
		k = generate_string(16);
	}
	map_keys.push_back(k);
	return k;
}

//CLIENT SIDE or COMMON
bool HE::ctxt_exists(mkt k){
	if (find(map_keys.begin(), map_keys.end(), k) == map_keys.end()){
		if (map_ctxts.find(k) == map_ctxts.end()){
			return false;
		}
	}
	return true;
}

//CLIENT SIDE or COMMON
long HE::getSlots(){
	return ea->size();
}


void HE::debug_on(bool on){
	debug = on;
}

//CLIENT SIDE
long HE::keyGen(key_params params) {
	if(debug){
		string left_margin = "";
		for (unsigned i = 0; i < className().length() + 2; i++){
			left_margin += " ";
		}
		cout << className() << ": *** Native plaintext space"<<endl;
		cout << left_margin << "    M="<<params.m<<", P="<<params.p;
		cout <<", R="<<params.r<<", D="<<params.d<<endl;
		cout << left_margin << "*** Security parameter, in bits [k]: ";
		cout <<params.k<<endl;
		cout << left_margin << "*** Levels / Circuit depth [L]: ";
		cout <<params.L<<endl;
		cout << left_margin << "*** Columns in KSM [c]: "<<params.c<<endl;
		cout << left_margin << "*** Hamming weight [w]: "<<params.w<<endl;
		cout << className() << ": Generating keypair..." << endl;
	}
	ZZX G;
	//initialize context
	context = new FHEcontext(params.m, params.p, params.r);
	//Adding primes to the modulus chain
	buildModChain(*context, params.L, params.c);
	//Construct a secret key structure
	secretKey = new FHESecKey(*context);
	// an "upcast": FHESecKey is a subclass of FHEPubKey
	publicKey = secretKey;
	if(params.d == 0) { //always the case for binary
		G = context->alMod.getFactorsOverZZ()[0];
	} else {
		G = makeIrredPoly(params.p, params.d);
	}
	//Generates a secret key with Hamming weight w
	secretKey->GenSecKey(params.w);
	addSome1DMatrices(*secretKey);
	//Construct an Encrypted array object ea that is associated with the 
	//given context and polynomial G   
	ea = new EncryptedArray(*context, G); 
	//get the number of plaintext slots
	long n = getSlots();
	if(debug){
		cout << className() << ": Keypair Generated." << endl;
		cout << className() << ": Plaintext slots = " << n << endl;
	}
	return n;
}

//CLIENT SIDE
mkt HE::encrypt(vector<long> ptxt_vect) {
/* 	Encrypts a plaintext vector of long values. Stores the ciphertext in 
	the unordered map. Returns the map key variable associated with 
	the new ciphertext */
	Ctxt ctxt(*publicKey, 0);
	ea->encrypt(ctxt, *publicKey, ptxt_vect);    
	return storeCtxt(&ctxt); 
}

//CLIENT
mkt HE::setOnes(long n){
	if(debug){
		cout << className() << ": Setting Ones ciphertext" << endl;
	}
	vector<long> ones(n,1);
	return encrypt(ones);
}
//SERVER
void HE::set01(mkt k){
	if(debug){
		cout << className() << ": Deducting Zeros ciphertext" << endl;
	}
	k_ones = k;
	k_zeros = copy(k_ones);
	sub(k_zeros, k_ones);
}

//CLIENT SIDE
vector<long> HE::decrypt(mkt k) {
/* 	Decrypts a packed ciphertext and stores the results in a vector of 
	long values. If the map key or the ciphertext does not exist, 
	the resulting vector is filled with 500 '9's, digit clearly 
	distinguishable from 0s and 1s in the binary system. */
	vector<long> ptxt_vect;
	if (ctxt_exists(k)){
		ea->decrypt(map_ctxts.at(k), *secretKey, ptxt_vect);
	} else {
		for (unsigned i = 0; i < 500; i++){
			ptxt_vect.push_back(9); //Could raise an exception instead
		}
	}
	return ptxt_vect;
}
vector< vector<long> > HE::decryptNbits(vector<mkt> k){
	unsigned nbits = k.size();
	vector< vector<long> > v_out(nbits);
	for (unsigned i = 0; i < nbits; i++){
		v_out[i] = decrypt(k[i]);
	}
	return v_out;
}




/* Ciphertexts management: copy, erase, replace */
//The following is for the SERVER side.
mkt HE::copy(mkt k){
/* 	Copies the ciphertext associated with the map key k, stores the copy 
	with a new random and unique map key and returns this key. 
	If the map key provided does not exist, an empty key is returned. */
	mkt new_map_key = "";
	if (ctxt_exists(k)){
		Ctxt ctxt = map_ctxts.at(k);
		new_map_key = storeCtxt(&ctxt);
	} else {
		cout << className() << ": " << __FUNCTION__ << ": WARNING!" << endl; 
		new_map_key = ""; //empty key is an error
	}
	return new_map_key;	
}
void HE::erase(mkt k){
/*	Checks if the map key provided and its associated ciphertext exist.
	If they don't, it displays an error message.
	Otherwise, the map key and the ciphertext are erased from the 
	unordered map.*/
	if (ctxt_exists(k)){
		map_ctxts.erase(k);
		map_keys.erase(find(map_keys.begin(),map_keys.end(),k));
	} else {
		cout << className() << ": " << __FUNCTION__ << ": WARNING!" << endl;
	}
}
void HE::replace(mkt k, mkt k_replacement){
/*	Checks if both map keys and ciphertexts exist before doing anything. 	
	It then replaces the ciphertext at map key k by a copy of the 
	ciphertext at map key k_replacement. If one of the map keys or 
	ciphertexts do not exist, an error message is displayed.*/
	if (ctxt_exists(k) && ctxt_exists(k_replacement)){
		Ctxt ctxt = map_ctxts.at(k_replacement);
		map_ctxts.at(k) = ctxt;
	} else {
		cout << className() << ": " << __FUNCTION__ << ": Warning!" << endl;
	}
}







/* Arithmetic operations in the ciphertexts map (overrides previous data) */
//SERVER SIDE - needs map_ctxts
void HE::add(mkt k1, mkt k2){
	map_ctxts.at(k1) += map_ctxts.at(k2);
}
void HE::sub(mkt k1, mkt k2){
	map_ctxts.at(k1) -= map_ctxts.at(k2);
}
void HE::mul(mkt k1, mkt k2){
	map_ctxts.at(k1).multiplyBy(map_ctxts.at(k2));
}
void HE::mul(mkt k1, mkt k2, mkt k3){
	map_ctxts.at(k1).multiplyBy2(map_ctxts.at(k2), map_ctxts.at(k3));
}
void HE::neg(mkt k1){
	map_ctxts.at(k1).negate();
}
bool HE::eq(mkt k1, mkt k2){
	bool comparePubkeys = true;
	return map_ctxts.at(k1).equalsTo(map_ctxts.at(k2), comparePubkeys);
}








//SERVER SIDE
/*LOGIC GATES only when p = 2 */
void HE::XOR(mkt k1, mkt k2){
	/* 	Purpose: Binary XOR of k1 and k2
		Inputs: Bits of k1 and bits of k2
		Outputs: k1 = k1 XOR k2
		Complexity: negligible */
	add(k1, k2);
}
void HE::AND(mkt k1, mkt k2){
	/* 	Purpose: Binary AND of k1 and k2
		Inputs: Bits of k1 and bits of k2
		Outputs: k1 = k1 AND k2
		Complexity: 1 multiplication */
	mul(k1, k2);
}
void HE::AND(mkt k1, mkt k2, mkt k3){
	/* 	Purpose: Binary AND of k1 and k2
		Inputs: Bits of k1, bits of k2 and bits of k3
		Outputs: k1 = k1 AND k2 AND k3
		Complexity: 1 multiplication */
	mul(k1, k2, k3);
}
void HE::NOT(mkt k1){
	/* 	Purpose: Binary NOT of k1
		Inputs: Bits of k1
		Outputs: k1 = NOT k1
		Complexity: negligible */
	add(k1,k_ones);
}
void HE::NAND(mkt k1, mkt k2){
	/* 	Purpose: Binary NAND of k1 and k2
		Inputs: Bits of k1 and bits of k2
		Outputs: k1 = k1 NAND k2
		Complexity: 1 multiplication */
	AND(k1,k2);
	NOT(k1);
}
void HE::OR(mkt k1, mkt k2){
	/* 	Purpose: Binary OR of k1 and k2
		Inputs: Bits of k1 and bits of k2
		Outputs: k1 = k1 OR k2
		Complexity: 1 multiplication */
	NOR(k1,k2);
	NOT(k1);
}
void HE::NOR(mkt k1, mkt k2){
	/* 	Purpose: Binary NOR of k1 and k2
		Inputs: Bits of k1 and bits of k2
		Outputs: k1 = k1 NOR k2
		Complexity: 1 multiplication */
	mkt k2c = copy(k2);
	NOT(k1);
	NOT(k2c);
	AND(k1,k2c);
	erase(k2c);
}
void HE::XNOR(mkt k1, mkt k2){
	/* 	Purpose: Binary XNOR of k1 and k2
		Inputs: Bits of k1 and bits of k2
		Outputs: k1 = k1 XNOR k2
		Complexity: negligible */
	XOR(k1, k2);
	NOT(k1);
}









/*COMBINATIONAL CIRCUITS*/
void HE::HFADDER(mkt k1, mkt k2){
	/* 	Purpose: Binary half addition of k1 and k2 (with carry out only)
		Inputs: Bit k1 and Bit k2
		Outputs: k1 = SUM and k2 = CARRY_OUT
		Complexity: 1 multiplication */
	mkt k_Cout = copy(k1);
	XOR(k1,k2); //k1 is SUM
	AND(k2,k_Cout); //k2 is CARRY OUT
	erase(k_Cout);
}
void HE::FLADDER(mkt k1, mkt k2, mkt k_Cin){
	/* 	Purpose: Binary full addition of k1 and k2 (with carry in & out)
		Inputs: Bit k1, bit k2, and carry-in bit
		Outputs: k1 = SUM and k2 = CARRY_OUT
		Complexity: 3 multiplications */
	HFADDER(k1,k2); // k1 is 1st sum, k2 is 1st Cout
	HFADDER(k1,k_Cin); // k1 is FINAL sum, k_Cin is 2nd Cout
	OR(k2, k_Cin); // k2 is FINAL Cout
}
void HE::HFSUBER(mkt k1, mkt k2){
	/* 	Purpose: Binary half substraction of k1 and k2 (with borrow out)
		Inputs: Bit k1 and bit k2
		Outputs: k1 = DIFFERENCE and k2 = BORROW_OUT
		Complexity: 1 multiplication */
	mkt k1c = copy(k1);
	XOR(k1,k2); // k1 contains difference 
	NOT(k1c);
	AND(k2,k1c); // k2 contains Bout (borrow)
	erase(k1c);
}
void HE::FLSUBER(mkt k1, mkt k2, mkt k_Bin){
	/* 	Purpose: Binary full substraction of k1 and k2 (with borrow in & out)
		Inputs: Bit k1, bit k2 and borrow-in bit
		Outputs: k1 = DIFFERENCE and k2 = BORROW_OUT
		Complexity: 3 multiplications */
	HFSUBER(k1,k2); // k1 is difference, k2 is 1st Bout
	HFSUBER(k1,k_Bin); // k1 is FINAL difference, k_Bin is 2nd B out
	OR(k2, k_Bin); // k2 is FINAL B out
}
void HE::HFEQUAL(mkt k1, mkt k2){
	/* 	Purpose: Binary comparison "k1 = k2"
		Inputs: Bit k1 and bit k2
		Outputs: k1 = (k1 == k2) and k2 untouched
		Complexity: negligible */
	XNOR(k1, k2);
}
void HE::FLEQUAL(mkt k1, mkt k2, mkt k_eq){
	/* 	Purpose: Binary comparison "k1 = k2" for N bits numbers
		Inputs: Bit k1, bit k2, and HSB or LSB k_eq bit found
		Outputs: k1 = (k1==k2)
		Complexity: 1 multiplication */
	HFEQUAL(k1,k2);
	AND(k1, k_eq); // When EQ becomes 0, it stays 0 to the end.
}
void HE::SMALLER(mkt k1, mkt k2){
	/* 	Purpose: Binary comparison "k1 < k2"
		Inputs: Bit k1 and bit k2
		Outputs: k1 = (k1<k2) and k2 = untouched
		Complexity: 1 multiplication */
	NOT(k1);
	AND(k1, k2);
}
void HE::HFCMP(mkt k1, mkt k2){
	/* 	Purpose: Binary half comparison with equal and greater than
		Inputs: Bit k1 and bit k2
		Outputs: k1 = (k1==k2) and k2 = (k1>k2)
		Complexity: 1 multiplication */
	mkt k1c = copy(k1);
	HFEQUAL(k1,k2);
	SMALLER(k2, k1c);
	erase(k1c);
}
void HE::FLCMP(mkt k1, mkt k2, mkt k_eq, mkt k_gre){
	/* 	Purpose: Binary full comparison with equal&greater in/out
		Inputs: Bit k1, bit k2, 
				bit k_eq (HSB equality) and k_gre (HSB k1 > k2)
		Outputs: k1 = (all k1 = all k2) and k2 = (all k1 > all k2)
		Complexity: 4 multiplications */
	mkt k1c = copy(k1);
	FLEQUAL(k1, k2, k_eq); //1 multiplication

	SMALLER(k2, k1c); //1 multiplication - contains k1 > k2
	//Higher significant bits must be equal for k1 > k2
	AND(k2, k_eq); //1 multiplication
	//When GRE becomes 1, it stays 1 to the end (LSB)
	OR(k2, k_gre); //1 multiplication
	erase(k1c);
}
void HE::MUX(mkt k1, mkt k2, mkt ks){
	/* 	Purpose: 2 bit multiplexer with 1 bit select signal (ks)
		Inputs: Bit k1, bit k2 and bit ks
		Outputs: k1 = k1 if ks = 1
				 k1 = k2 if ks = 0
				 k2 and ks are untouched.
		Complexity: 3 multiplications */
	mkt ksc = copy(ks);
	NAND(k1,ksc);
	NOT(ksc);
	NAND(ksc,k2);
	NAND(k1,ksc);
	erase(ksc);
}







	

/* SEQUENTIAL CIRCUITS */
/*Designed especially for N bit numbers
	Usually the inputs are nbits ciphertexts Ctxt0... where:
	Ctxt0 Ctxt1 ... Ctxtnbits
	0		0			1		-> Number at slot 0
	0		1			1		-> Number at slot 1
	0		0			0		-> Number at slot 2
	1		0			1		-> Number at slot 3
	For a vector map keys k_bit, k_bit[0] is the LSB whilst k_bit[nbits-1] 
	is the MSB
*/
void HE::RCADDER(vector<mkt> &k1_bit, vector<mkt> k2_bit){
	/* 	Purpose: Ripple Carry Adder nbits x nslots
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and 
				bit k2[0],..., bit k2[nbits - 1]
				Note that nbits can be different for k1 and k2 but these 
				would then be padded if it was the case
		Outputs: k1 = (nbits + 1) bit SUM (unsigned) and k2 = untouched
		Complexity: 3*nbits + 1 multiplications
		Level: For 1024 slots and security k=128 we have the following 
			minimum levels: 
			number of bits: 2  3  4  5  6  7  8  9  10 11 12 13 14
	*/
	unsigned nbits = PAD_BITS(k1_bit, k2_bit);
	vector<mkt> kc_bit(nbits);
	for(unsigned i = 0; i < nbits; i++){
		kc_bit[i] = copy(k2_bit[i]);
	}
	k1_bit.push_back(copy(k_zeros));
	kc_bit.push_back(copy(k_zeros));
	nbits++;
	
	HFADDER(k1_bit[0], kc_bit[0]); //1 multiplication
	for (unsigned i = 1; i < nbits; i++){ //This is nbits+1 actually
		FLADDER(k1_bit[i], kc_bit[i], kc_bit[i-1]); //3 multiplications
		erase(kc_bit[i-1]);
	}
	erase(kc_bit[nbits - 1]);
}
void HE::RBSUBER(vector<mkt> &k1_bit, vector<mkt> k2_bit){
	/* 	Purpose: Ripple Borrow Subtractor nbits x nslots
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and 
				bit k2[0],..., bit k2[nbits - 1]
				Note that nbits can be different for k1 and k2 but these 
				would then be padded if it was the case
		Outputs: 	k1 = (nbits + 1) bits DIFFERENCE (unsigned and signed) 
					and k2 = untouched
		Complexity: 1 + nbits*3 multiplications */
	unsigned nbits = PAD_BITS(k1_bit, k2_bit);
	vector<mkt> kc_bit(nbits);
	for(unsigned i = 0; i < nbits; i++){
		kc_bit[i] = copy(k2_bit[i]); 
	}
	k1_bit.push_back(copy(k_zeros)); //for sign
	kc_bit.push_back(copy(k_zeros)); //for sign
	nbits++;
	
	HFSUBER(k1_bit[0], kc_bit[0]); //1 multiplication
	for (unsigned i = 1; i < nbits; i++){ //this is nbits+1 actually
		FLSUBER(k1_bit[i], kc_bit[i], kc_bit[i-1]); //3 multiplications
		erase(kc_bit[i-1]);
	}
	erase(kc_bit[nbits - 1]);
}
void HE::RCMP(vector<mkt> &k1_bit, vector<mkt> &k2_bit){
	/* 	Purpose: Ripple Comparator
				 (equality and lower than, for unsigned numbers)
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and 
				bit k2[0],..., bit k2[nbits - 1]
				Note that nbits can be different for k1 and k2 but these 
				would then be padded if it was the case
		Outputs: k1[0] = (k1==k2) and k2[0] = (k1>k2)
				 The rest of k1 and k2 is erased
		Complexity: 4*(nbits - 1) + 1 multiplications */
	unsigned nbits = PAD_BITS(k1_bit, k2_bit);
	//starts with MSB, 1 multiplication
	HFCMP(k1_bit[nbits - 1], k2_bit[nbits - 1]); 
	for (int i = nbits - 2; i >= 0; i--){ //executed nbits - 1 times
		//Full comparator requires 4 multiplications
		FLCMP(k1_bit[i], k2_bit[i], k1_bit[i+1], k2_bit[i+1]);
		erase(k1_bit[i+1]);
		k1_bit.pop_back();
		erase(k2_bit[i+1]);
		k2_bit.pop_back();
	}
}
void HE::NMUX(vector<mkt> k1_bit, vector<mkt> k2_bit, mkt ks){
	/* 	Purpose: N bit multiplexer with a single select line (MUX N:1)
		Inputs: Bit k1[0],..., bit k1[nbits - 1],
				Bit k2[0],..., bit k2[nbits - 1],
				Bit ks (select signal)
		Outputs: k1 = k1 if ks = 1
				 k1 = k2 if ks = 0
				 k2 and ks are untouched.
		Complexity: 3*nbits multiplications */
	unsigned nbits = PAD_BITS(k1_bit, k2_bit);
	for (unsigned i = 0; i < nbits; i++){
		MUX(k1_bit[i], k2_bit[i], ks);
	}
}
void HE::REQ(vector<mkt> &k1_bit, vector<mkt> k2_bit){
	/* 	Purpose: Ripple Equality tester (for unsigned and signed numbers)
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and 
				bit k2[0],..., bit k2[nbits - 1]
				Note that nbits can be different for k1 and k2 but these 
				would then be padded if it was the case
		Outputs: k1[0] = (k1==k2) (rest of k1 is erased) and k2 = untouched
		Complexity: nbits - 1 multiplications */
	unsigned nbits = PAD_BITS(k1_bit, k2_bit);
	HFEQUAL(k1_bit[nbits - 1], k2_bit[nbits - 1]);
	for (int i = nbits - 2; i >= 0; i--){
		FLEQUAL(k1_bit[i], k2_bit[i], k1_bit[i+1]);
		erase(k1_bit[i+1]);
		k1_bit.pop_back();
	}
}
void HE::SHIFTR(vector<mkt> k1_bit, const unsigned shift){
	/* 	Purpose: Right shift of binary number and sets MSB to 0
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and the shift amount
		Outputs: None, just moves ciphertexts around
		Complexity: negligible */
	unsigned nbits = k1_bit.size();
	for (unsigned j = 0; j < shift; j++){
		for (unsigned i = 0; i < nbits - 1; i++){ //LSB to MSB
			replace(k1_bit[i],k1_bit[i+1]);
		}
		replace(k1_bit[nbits - 1], k_zeros);
	}
}
void HE::SHIFTL(vector<mkt> &k1_bit, const unsigned shift){
	/* 	Purpose: Left shift of binary number and sets LSB to 0
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and the shift amount
		Outputs: None, just replaces ciphertexts and 
				 extends the size of k1_bit
		Complexity: negligible */
	for (unsigned i = 0; i < shift; i++){
		k1_bit.push_back(copy(k_zeros));
	}
	unsigned nbits = k1_bit.size();
	for (unsigned j = 0; j < shift; j++){
		for (unsigned i = nbits - 1; i > 0; i--){ //MSB to LSB
			replace(k1_bit[i], k1_bit[i-1]);
		}
		k1_bit[0] = copy(k_zeros);
	}
}










/* ARITHMETIC CIRCUITS */
/*Designed especially for N bit numbers
	Usually the inputs are nbits ciphertexts Ctxt0... where:
	Ctxt0 Ctxt1 ... Ctxtnbits
	0		0			1		-> Number at slot 0
	0		1			1		-> Number at slot 1
	0		0			0		-> Number at slot 2
	1		0			1		-> Number at slot 3
	For a vector map keys k_bit, k_bit[0] is the LSB 
	whilst k_bit[nbits-1] is the MSB
*/
void HE::MULTIPLY(vector<mkt> &k1_bit, vector<mkt> k2_bit){
	/* 	Purpose: Multiply two binary numbers (unsigned integers)
		Inputs: Bit k1[0],..., bit k1[nbits - 1] and 
				bit k2[0],..., bit k2[nbits - 1]
				Note that nbits can be different for k1 and k2 but these 
				would then be padded if it was the case
		Outputs: k1 = k1 * k2 (twice the size of original k1 or k2)
		Complexity: 4.5*nbits*nbits - 3.5*nbits
		Time complexity: 5.5*nbits*nbits - 3.5*nbits - 1
	*/
	unsigned nbits = PAD_BITS(k1_bit, k2_bit);
	const float total_mul_time = 5.5*nbits*nbits - 3.5*nbits - 1;
	unsigned mul_done = 0;
	if(debug){
		cout << className() << ": Multiplication completed at..." << endl;
	}
	
	//Setting binary rows to k1
	vector< vector<mkt> > k(nbits, vector<mkt>(nbits));
	for (unsigned row = 0; row < nbits; row++){
		for (unsigned bit = 0; bit < nbits; bit++){
			k[row][bit] = copy(k1_bit[bit]);
		}
	}
	
	
	//ANDing and shifting binary rows
	//Requires only one multiplication for ciphertext in term of complexity
	//In terms of time, it needs nbits * nbits multiplications
	for (unsigned row = 0; row < nbits; row++){
		for (unsigned bit = 0; bit < nbits; bit++){
			AND(k[row][bit], k2_bit[row]); //1 multiplication
		}
		SHIFTL(k[row], row); //0 multiplication
	}
	if(debug){
		mul_done += nbits*nbits;
		cout << className() << ":                               ";
		cout << 100*(mul_done/total_mul_time) << "%" << endl;
	}
	
	
	
	//Setting accumulator to the first binary row
	for(unsigned bit = 0; bit < nbits; bit++){
		erase(k1_bit[bit]);
		k1_bit[bit] = copy(k[0][bit]);
	}
	k1_bit.push_back(copy(k_zeros)); //Adds a 0 as the new MSB
	//This increases n in terms of complexity for RCADDER
	
	//Accumulates all the rows in result
	//Complexity & Time: 4.5*nbits*nbits - 3.5*nbits - 1
	for (unsigned row = 1; row < nbits; row++){
		RCADDER(k1_bit, k[row]); //1 + 3n, and increases n by 1	
		if(debug){
			mul_done += 3*(k1_bit.size()-1) + 1;
			cout << className() << ":                               ";
			cout << 100*(mul_done/total_mul_time) << "%" << endl;
		}
	}
	
	//Erasing garbage
	for(unsigned i = 0; i < k.size(); i++){
		for(unsigned j = 0; j < k[i].size(); j++){
			erase(k[i][j]);
		}
	}
}
void HE::DIVIDE(vector<mkt> &A, vector<mkt> &B){
	/* 	Purpose: Euclidean division of two binary numbers (unsigned)
				 using the restoring division algorithm
				 A is the dividend, B is the divisor.
				 A should be greater than B or error could appear (unfixed)
		Inputs: Bit A[0],..., bit A[nbits - 1] and 
				bit B[0],..., bit B[nbits - 1]
				Note that nbits can be different for dividend and divisor 
				but these would then be padded if it was the case
		Outputs: dividend = remainder, divisor = quotient
		Complexity & TIME:    nbits*(1 + 3*nbits + 3*nbits)
							= 6*nbits*nbits + nbits
	*/
	const unsigned nbits = PAD_BITS(A, B);
	const float total_mul_time = 8*nbits*nbits + nbits;
	unsigned mul_done = 0;
	if(debug){
		cout << className() << ": Division completed at..." << endl;
	}
	
	//0. Place dividend A in lower half of X
	vector<mkt> X(2*nbits);
	for(unsigned i = 0; i < nbits; i++){
		X[i] = copy(A[i]);
		erase(A[i]);
		X[nbits + i] = copy(k_zeros);
	}
	
	//1. Shift X left by 1 bit
	SHIFTL(X, 1);
	erase(X[2*nbits]);
	X.pop_back(); //removes MSB added by SHIFTL
	
	mkt sign;
	vector<mkt> R(nbits), R_backup(nbits);
	for (int b = nbits - 1; b >= 0; b--){ //MSB of quotient to LSB
	
		for(unsigned i = 0; i < nbits; i++){
			R[i] = copy(X[nbits + i]);
			R_backup[i] = copy(X[nbits + i]);
		}
		
		//2. Subtract divisor B to the MSB half of X
		RBSUBER(R, B); //1 + 3n complexity
		mul_done += 1 + 5*nbits;

		sign = R[nbits]; //1 if negative
		R.pop_back(); //removes sign bit from the vector
		NOT(sign); //0 if negative
		
		//3. Conditional replacement of upper half of X
		//restores backup if negative
		NMUX(R, R_backup, sign); //3n complexity
		mul_done += 3*nbits;
		for(unsigned i = 0; i < nbits; i++){
			erase(X[nbits + i]);
			X[nbits + i] = R[i];
			erase(R_backup[i]);
		}
		SHIFTL(X, 1);
		erase(X[2*nbits]);
		X.pop_back(); //removes MSB added by SHIFTL
		X[0] = sign;
		if(debug){
			cout << className() << ":                         ";
			cout << 100*mul_done/total_mul_time << "%" << endl;
		}
	}
	for (unsigned i = 0; i < nbits; i++){
		A[i] = X[i]; //quotient
		erase(B[i]);
		B[i] = X[nbits + i]; //remainder
	}
	SHIFTR(B, 1);
}




//Designed especially for N numbers of n bits each.
void HE::AVERAGES(vector< vector<mkt> > &numbers, vector<mkt> &N){
	/*numbers is a vector of numbers, where each number is a vector of 
	encrypted bits. N is the divider which has to be provided to perform 
	the homomorphic euclidean division. The quotient and remainder of 
	the average are returned in numbers[0] and N respectively. */
	for (unsigned i = 1; i < numbers.size(); i++){
		RCADDER(numbers[0],numbers[i]); //accumulates all the numbers
		for(unsigned b = 0; b < numbers[i].size(); b++){
			erase(numbers[i][b]);
		}
	}
	PAD_BITS(numbers[0],N);
	DIVIDE(numbers[0], N);
}
void HE::FAVERAGES(vector< vector<mkt> > &numbers){
	/*numbers is a vector of numbers, where each number is a vector of 
	encrypted bits. This operation will loose the precision as no 
	remainder is calculated */
	unsigned N = numbers.size();
	if (N == 0){
		cout << className() <<": "<< __FUNCTION__ <<": N can't be 0"<< endl;
		return;
	}
	float rs = log2(N); //amount of right shift
	bool N_is_pow_of_2 = false;
	for (unsigned i = 0; i < N; i++){
		if(rs == i){
			N_is_pow_of_2 = true; //or N is 1 which is fine too
			break;
		}
	}
	if (!N_is_pow_of_2){
		cout << className() <<": "<< __FUNCTION__ <<": N not power of 2."<< endl;
		return;
	}
	
	for (unsigned i = 1; i < N; i++){
		RCADDER(numbers[0],numbers[i]); //accumulates all the numbers
		for(unsigned b = 0; b < numbers[i].size(); b++){
			erase(numbers[i][b]);
		}
	}
	for (unsigned i = 0; i < N - 1; i++){
		numbers.pop_back();
	}	
	SHIFTR(numbers[0], (unsigned)rs); //divides by a power of 2
}






unsigned HE::PAD_BITS(vector<mkt> &k1_bit, vector<mkt> &k2_bit){
	unsigned nbits_1 = k1_bit.size();
	unsigned nbits_2 = k2_bit.size();
	if (nbits_1 < nbits_2){
		for (unsigned i = nbits_1; i < nbits_2; i++){
			k1_bit.push_back(copy(k_zeros));
		}
		return nbits_2;
	} else if (nbits_1 > nbits_2){ //just resize k2 with leading zeros
		for (unsigned i = nbits_2; i < nbits_1; i++){
			k2_bit.push_back(copy(k_zeros));
		}
		return nbits_1;
	}
}