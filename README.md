# hbc

_This project was developed as my Master final year project at Imperial College London._

## What is it in **one** line? ##
### It is an API of binary operations such as binary comparisons or binary divisions using the library HElib.

## What is in there? ##
The core API is in the file _he.cpp_, the main code is in _main.cpp_, some other classes are in _helper_functions.cpp_ and the rest are unit tests and timing tests for the homomorphic binary operations implemented. 

## How do I run it? ##

You will need several libraries.
You can either use the _Makefile_ following the instructions in the **Makefile** section or do it manually.
In both scenarios, you will need the module **make**.
To do it manually:

0. You should already have gcc-g++ or g++ installed.
1. Install m4 `apt install m4`
2. Install perl `apt install perl`
3. Install gmp:
    * Download it [here](https://gmplib.org/download/gmp/gmp-6.1.0.tar.bz2)
    * Extract it `tar xjf gmp-6.1.0.tar.bz2`
    * Go to it directory `cd gmp-6.1.0`
    * Configure it `./configure`
    * Make it `make`
    * Install it `make install`
    * Optionally, check it `make check`
    * **For Cygwin**, `cp -f /usr/local/lib/libgmp.* /usr/XXXXX-pc-cygwin/lib/` where XXXXX is your CPU architecture.
4. Install NTL:
    * Download it [here](http://www.shoup.net/ntl/ntl-9.6.2.tar.gz)
    * Extract it `tar xf ntl-9.6.2.tar.gz`
    * Go to its source directory `cd ntl-9.6.2/src`
    * Configure it `./configure NTL_GMP_LIP=on`
    * Make it `make`
    * Install it `make install`
5. Install HElib
    * Install git `apt install git`
    * Clone it `git clone https://github.com/shaih/HElib.git`
    * **For Cygwin**, do `sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp` or there will be a compile error.
    * Go to its source directory `cd HElib/src`
    * Make it `make`
    * Check it `make check`
    * Optionally test it `make test`
6. HElib is now ready to be used.
7. Download and compile this project
    * Clone it `git https://github.com/qdm12/hbc.git ./hbc_git`
    * Create a source folder `mkdir -p source` at the root (where you should be)
    * Copy the source files `cp ./hbc_git/source/* ./source`
    * You will then need to compile all the files into objects (or simply run `make HE` from the Makefile provided)
       * Create an _objects_ folder `mkdir -p objects`
       * Compile the API `g++ -c source/he.cpp -I HELib/src -o objects/he.o`
       * Compile the helper functions `g++ -c source/helper_functions.cpp -o objects/helper_functions.o`
       * Compile the various tests
          * `g++ -c source/test_gates.cpp -I HELib/src -o objects/test_gates.o`
          * `g++ -c source/test_circ_comb.cpp -I HELib/src -o objects/test_circ_comb.o`
          * `g++ -c source/test_circ_seq.cpp -I HELib/src -o objects/test_circ_seq.o`
          * `g++ -c source/test_circ_arithm.cpp -I HELib/src -o objects/test_circ_arithm.o`
       * Compile the main.cpp source file `g++ -c source/main.cpp -I HELib/src -o objects/main.o`
       * Compile the objects into HEapp `g++ objects/*.o HELib/src/fhe.a -o HEapp -L/usr/local/lib -lntl -lgmp -lm`
8. Run the program with ./HEapp & Enjoy !
9. Please refer to my long but complete report on this project, available [here](www.google.com)
10. You're welcome to contribute to this repository if you find any better circuits or other circuits and implement them !

## Using the Makefile ##
XXX to do


## Abstract ##
This project concerns the research and development of a real-use application of homomorphic encryption for cloud computing. The application takes advantage of the various possibilities and limitations of present homomorphic encryption schemes and programming libraries to remain usable in terms of time. The foundations of the application rely on the design of binary operations using homomorphic encryption. All the binary logic gates and various binary blocks were developed and adapted to provide enough functionalities to the application. The project focuses on providing features to cloud computing such as calculating averages on large amounts of encrypted numbers in a relatively short and decent time. The result is an application program interface written in C++ allowing to perform various operations on integers. It thus shows homomorphic encryption can be used today for simple operations if the security is more important than the speed of execution. 
