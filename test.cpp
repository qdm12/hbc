#include <fstream>
#include <sstream>
#include <iostream>
#include <bitset>
#include "helper_functions.h"

using namespace std;

int main(int argc, char **argv)
{
	Conversion conv;
	
	bool x = true;
	string y = conv.bool2Str(x);
	cout << y << endl;
    return 0;		
}


