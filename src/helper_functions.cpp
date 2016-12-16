#include "helper_functions.h"

string generate_string(int length){
	const string alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//"0123456789!@#$%^&*ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	string s;
	for(unsigned int i = 0; i < length; ++i){
		s += alphanum[rand() % (alphanum.size() - 1)];
	}
	return s;
}



Errors::Errors(string t){
	title = t;
}
void Errors::add(string name, bool error){
	names.push_back(name);
	errors.push_back(error);
}
void Errors::display(){
	bool no_error = true;
	for (int i = 0; i < errors.size(); i++){
		if (errors[i]){
			no_error = false;
			cout << title << ": Error occured for test " << names[i] << endl;
		}
	}
	if (no_error){
		cout << title << ": ALL TESTS PASSED" << endl;
	}
}


Timing::Timing(){
	title = "";
	measure_id = 0;
}
Timing::Timing(string t){
	title = t + ": ";
	measure_id = 0;
}
void Timing::start(){
	measure_id++;
	a = clock();
}
void Timing::end(){
	b = clock();
	duration = double(b - a)/CLOCKS_PER_SEC;
	cout << title << "Duration of measurement " << measure_id << ": ";
	cout << duration << " seconds" << endl;
}
double Timing::end(string silent){
	b = clock();
	duration = double(b - a)/CLOCKS_PER_SEC;
	return duration;
}

bool Conversion::str2Bool(string bit){
	istringstream is(bit);
	bool ret;
	is >> ret;
	return ret;
}
string Conversion::bool2Str(bool value){
	ostringstream os ;
	os << value;
	return os.str();
}
string Conversion::long2Str(long value){
	ostringstream os;
	os << value;
	return os.str();
}
string Conversion::long2bitStr(long value){
	//Only for positive values (no 2's complement)
	unsigned bits = 0;
	while (value > 0) { 
		bits++;
		value = value >> 1;
	}
	bitset<32> bin(value);
	string bin_str = "";
	for (int i = bits - 1; i >= 0; i--){
		bin_str += long2Str(bin[i]);
	}
	return bin_str;
}
long Conversion::bitStr2Long(string bin_str){ //max 32 bits
	bitset<32> bin(bin_str);
	return bin.to_ulong();
}
string Conversion::bitStr2LongStr(string bin_str){ //max 32 bits
	//Only for positive values (no 2's complement)
	return long2Str(bitStr2Long(bin_str));
}
long Conversion::signedBitStr2Long(string bin_str){
	string substring = bin_str.substr(0,1);
	if (str2Bool(substring)){ //2s complement MSB
		int inv;
		string bin_inv_str = "";
		for (int i = 0; i < bin_str.length(); i++){
			substring = bin_str.substr(i,1);
			inv = !str2Bool(substring);
			bin_inv_str += bool2Str(inv);
		}
		bitset<32> binary(bin_inv_str);
		return - (binary.to_ulong() + 1);
	}
	else{
		bitset<32> binary(bin_str);
		return binary.to_ulong();
	}
}
string Conversion::signedBitStr2LongStr(string bin_str){
	return long2Str(signedBitStr2Long(bin_str));
}
vector<string> Conversion::matrix2bitStrVec(vector< vector<long> > v_out){
	unsigned nbits = v_out.size();
	unsigned nslots = v_out[0].size();
	string bin_str;
	vector<string> v_out_binstr(nslots);
	for (unsigned j = 0; j < nslots; j++){ //bit level
		bin_str = "";
		for (int i = nbits - 1; i >= 0; i--){
			//array of independent bits (ciphertexts)
			bin_str += long2Str(v_out[i][j]);
		}
		v_out_binstr[j] = bin_str;
	}
	return v_out_binstr;
}
vector<long> Conversion::matrix2LongVec(vector< vector<long> > v_out){
	unsigned nbits = v_out.size();
	unsigned nslots = v_out[0].size();
	string bin_str;
	vector<long> v_out_long(nslots);
	for (unsigned j = 0; j < nslots; j++){ //bit level
		bin_str = "";
		for (int i = nbits - 1; i >= 0; i--){
			//array of independent bits (ciphertexts)
			bin_str += long2Str(v_out[i][j]);
		}
		long x = bitStr2Long(bin_str);
		v_out_long[j] = x;
	}
	return v_out_long;
}
vector<long> Conversion::matrix2SignedLongVec(vector< vector<long> > v_out){
	unsigned nbits = v_out.size();
	unsigned nslots = v_out[0].size();
	string bin_str;
	vector<long> v_out_long(nslots);
	for (unsigned j = 0; j < nslots; j++){ //bit level
		bin_str = "";
		for (int i = nbits - 1; i >= 0; i--){
			//array of independent bits (ciphertexts)
			bin_str += long2Str(v_out[i][j]);
		}
		long x = signedBitStr2Long(bin_str);
		v_out_long[j] = x;
	}
	return v_out_long;
}
vector< vector<long> > Conversion::longVec2Matrix(vector< long > v_in){
	//Finds the number of bits necessary to represent the integers
	unsigned temp, bits = 0;
	vector< long > temp_vec = v_in;
	for(unsigned i = 0; i < temp_vec.size(); i++){
		temp = 0;
		while (temp_vec[i] > 0) { 
			temp++;
			temp_vec[i] = temp_vec[i] >> 1;
		}
		if (temp > bits){
			bits = temp;
		}
	}
	
	unsigned nslots = v_in.size();
	vector< vector<long> > v_in_bits;
	v_in_bits.resize(bits, vector<long>(nslots,0));
	
	
	for(int j = 0; j < nslots; j++){
		bitset<64> bin(v_in[j]); //max is 2^64
		for(int b = 0; b < bits; b++){
			v_in_bits[b][j] = bin[b]; //first ctxt (bits = 0) is LSB
		}
	}
	
	
	return v_in_bits;
}


