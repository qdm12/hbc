#include "FHE.h"
#include "EncryptedArray.h"
#include <NTL/lzz_pXFactoring.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>

typedef struct key_params{
	long m;
	long p; // Computations will be 'modulo p'
	long r; // Native plaintext space
	long d; // p and d determine the plaintext space F(p^d)
	long k; // Security parameter (bits)
	long L; // Number of ciphertext-primes to support (Levels, Depth) - depends on *
	long c; // Columns in key switching matrix (usually 2 or 3)
	long w; // Hamming weight of a secret key, 64 recommended	
} key_params;


key_params configure_params()
{
	key_params params;
	params.p = 257; //997
	params.r = 1;
	params.d = 0;
	params.k = 16; //128
	params.L = 8; //16
	params.c = 2; //3
	params.w = 64;	
    params.m = FindM(params.k,params.L,params.c,params.p,params.d,0,0); //cyclotomic ring
	//FindM(Security,Levels,Columns,Prime,degree,s,chosen_m)
	/*the argument s bounds from below the number of plaintext slots that we
	want to support, and chosen m gives the ability to specify a particular m 
	parameter and test if it satisfies all our constraints	*/
	return params;
}
	
int main(int argc, char **argv)
{
	bool debug = true;
	cout << "Started successfully" <<endl<<endl;
	
	/*key_ret keyPtrs;
	generate_key(keyPtrs);
	FHESecKey* secretKey2 = keyPtrs.priv;
	const FHEPubKey* publicKey2 = keyPtrs.pub;
	EncryptedArray* ea2 = keyPtrs.ea; */
	
	
	key_params params = configure_params();
	if(debug)
	{
		cout << " *** Native plaintext space"<<endl;
		cout << "     M="<<params.m<<", P="<<params.p<<", R="<<params.r<<", D="<<params.d<<endl;
		cout << " *** Security parameter, in bits: "<<params.k<<endl;
		cout << " *** Levels / Circuit depth: "<<params.L<<endl;
		cout << " *** Columns in KSM: "<<params.c<<endl;
		cout << " *** Hamming weight: "<<params.w<<endl<<endl;
		cout << "Initialising context..." << endl;
	}
	
	
    FHEcontext context(params.m,params.p,params.r);
	
	cout << "Building the modulus chain for circuit depth "<<params.L<<" and "<<params.c<<" digits in key-switching..." << endl;
    buildModChain(context,params.L,params.c);
	
	cout << "Constructing a secret key structure associated with the context..." << endl;
    FHESecKey secretKey(context);
	
    const FHEPubKey& publicKey = secretKey; //An "upcast": FHESecKey is a subclass of FHEPubKey
	
	cout << "Generating a secret key with hamming weight w="<<params.w<<"..." << endl;
	secretKey.GenSecKey(params.w);
	
	cout << "Computing key-switching matrices..." << endl;
	addSome1DMatrices(secretKey);
	
	//Monic polynomial, irreducible over Zp (see p37 he-library)
	ZZX G = context.alMod.getFactorsOverZZ()[0]; //if(0 == d)
	EncryptedArray ea(context, G);
	
	//WE NEED SECRETKEY, PUBLICKEY, EA.
	long nslots = ea.size(); //number of plaintext slots
	
	vector<long> v0(nslots), v1(nslots);
	
	v0[0] = 5;
	v0[1] = 2;
	v0[2] = 25;
	v0[4] = 99;
	
	v1[0] = 11;
	v1[1] = 70;
	v1[2] = 6;
	v1[3] = 7; 
	v1[4] = 2;
	
	cout << "Constructing ciphertexts associated with public key..." << endl;
	Ctxt ct0(publicKey), ct1(publicKey);
	cout << "Encrypting..." << endl;
	ea.encrypt(ct0, publicKey, v0);
	ea.encrypt(ct1, publicKey, v1);
   
	cout << "Homomorphic calculations..." << endl;
	Ctxt ctSum = ct0, ctProd = ct0, ctTest = ct0;
	ctSum += ct1;
	ctProd *= ct1;
	
	vector<long> const1(nslots);
	const1[0] = 8;
	const1[1] = 23;
	ZZX const1_poly;
	ea.encode(const1_poly, const1);
	ctTest.addConstant(const1_poly); //ct0 + const1 = 55
	

	cout << "Decrypting..." << endl;
    vector<long> res, res2, res3;
    ea.decrypt(ctSum, secretKey, res);
	ea.decrypt(ctProd, secretKey, res2);
	ea.decrypt(ctTest, secretKey, res3);
	
    for(int i = 0; i < res.size(); i++) { //Results are mod 257 (p)
        cout << v0[i] << " + " << v1[i] << " = " << res[i] << " || ";
		cout << v0[i] << " * " << v1[i] << " = " << res2[i] << " || ";
		cout << v0[i] << " + " << const1[i] << " = " << res3[i] << endl;
    }
    return 0;
}