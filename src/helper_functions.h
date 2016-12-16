#ifndef HELPER_H
#define HELPER_H
#include <sstream> //string
#include <stdlib.h> //rand
#include <vector>
#include <iostream>
#include <bitset>
#include <ctime>

using namespace std;

typedef string mkt; //Map Key Type - Key to access a ciphertext in the map
string generate_string(int length);
typedef struct key_params{
	long m;
	long p; // Defines the native plaintext space. Computations are 'mod p'
	long r; // Defines the native plaintext space
	long d; // Defines the plaintext space F(p^d) for packing ciphertexts
	long k; // Security parameter (bits)
	long L; // Level parameter or Circuit depth
	long c; // Columns in key switching matrix (usually 2 or 3)
	long w; // Hamming weight of a secret key, 64 recommended
	long slb; // Slots Lower bound - to force ciphertexts to pack more slots.
} key_params;

class Timing{
	public:
		Timing();
		Timing(string t);
		void start();
		void end();
		double end(string silent);
	private:
		clock_t a,b;
		double duration;
		unsigned measure_id;
		string title;
};

class Errors{
	public:
		Errors(string t);
		void add(string name, bool error);
		void display();
	private:
		string title;
		vector<string> names;
		vector<bool> errors;
};

class Conversion{
	public:
		string long2Str(long value);
		long bitStr2Long(string bin_str);
		long signedBitStr2Long(string bin_str);
		string long2bitStr(long value);
		vector<string> matrix2bitStrVec(vector< vector<long> > v_out);
		vector<long> matrix2LongVec(vector< vector<long> > v_out);
		vector<long> matrix2SignedLongVec(vector< vector<long> > v_out);
		vector< vector<long> > longVec2Matrix(vector< long > v_in);
	private:
		bool str2Bool(string bit);
		string bool2Str(bool value);
		
		string bitStr2LongStr(string bin_str);
		string signedBitStr2LongStr(string bin_str);
};

#endif


