# Homomorphic binary circuits - _hbc_ #

_This project was developed as my Master final year project at Imperial College London._

## What is it in **one** line? ##
It is an **API** of homomorphic binary operations such as binary comparisons or binary divisions using the library _HElib_.

## What is in there? ##
The core API is in the file _he.cpp_, the main code is in _main.cpp_, some other classes are in *helper_functions.cpp* and the rest are unit tests and timing tests for the homomorphic binary operations implemented. There is a _makefile_ to setup everything. My report is there to server as documentation for instance.

## What does it run ? ##
By default, it runs a set of unit tests on all the circuits implemented (initiated from _main.cpp_).
You can change the tests, or _main.cpp_ or whatever and it should still work.

## Documentation ##
For now, there is only my report on the project which should be quite complete, and my comments in the source code.

The report is contained in the repository and is also available [here](https://www.dropbox.com/s/rqnrslzb1pstkq0/FYP%20report%20-%20Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20-%20Quentin%20McGaw%20qdm12%202016.pdf?dl=0)

## How do I run it? ##
You will need several libraries.
You can either use the _Makefile_ following the instructions in the **Makefile** section or do it manually.
In both scenarios, you will need the module **make**.
The final executable file is HEapp in the root directory which can be run with `./HEapp`.


## Using the Makefile ##
_The makefile only supports Cygwin and Linux for instance, it has been untested on Mac OSX._
### Cygwin & Windows ###
**Due to permission restrictions on Windows operating systems**, the following modules have to be installed manually before launching the makefile (by using the Cygwin installer):
   * git
   * gcc-g++

When the makefile is launched, note that the following modules will be installed:
   * apt-cyg
   * curl
   * m4
   * perl

For installing the project the first time, the Makefile works without the other source codes.
Just move the Makefile in an empty directory and `cd` to it.

1. To download, compile and install HElib and other libraries: `make HElib`
2. To download, compile and run the project code: `make project`
3. To re-compile the project _source_ directory with your modifications: `make HE`
4. You can try `make help` for more information

**NOTE:** The project source code is now in the _./source_ directory, and the GitHub repository is in __./hbc_git__.

### Linux ###
When the makefile is launched, note that the following modules will be installed:
   * git
   * g++
   * curl
   * m4
   * perl

For installing the project the first time, the Makefile works without the other source codes.
Just move the Makefile in an empty directory and `cd` to it.

1. To download, compile and install HElib and other libraries: `make HElib`
2. To download, compile and run the project code: `make project`
3. To re-compile the project _source_ directory with your modifications and run HEapp: `make HE`
4. You can try `make help` for more information.

**NOTE:** The project source code is now in the _./source_ directory, and the GitHub repository is in *./hbc_git*.



## Manual Setup ##
_This could be especially useful for Mac OSX where the Makefile may not work_

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
    * Go back to main directory `cd ..`
4. Install NTL:
    * Download it [here](http://www.shoup.net/ntl/ntl-9.6.2.tar.gz)
    * Extract it `tar xf ntl-9.6.2.tar.gz`
    * Go to its source directory `cd ntl-9.6.2/src`
    * Configure it `./configure NTL_GMP_LIP=on`
    * Make it `make`
    * Install it `make install`
    * Go back to main directory `cd ..`
5. Install HElib
    * Install git `apt install git`
    * Clone it `git clone https://github.com/shaih/HElib.git`
    * **For Cygwin**, do `sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp` or there will be a compile error.
    * Go to its source directory `cd HElib/src`
    * Make it `make`
    * Check it `make check`
    * Optionally test it `make test`
    * Go back to main directory `cd ..`
6. HElib is now ready to be used.
7. Download and compile this project
    * Clone it `git https://github.com/qdm12/hbc.git ./hbc_git`
    * Create a source folder `mkdir -p source` at the root (where you should be)
    * Copy the source files `cp ./hbc_git/* ./source`
    * You will then need to compile all the files into objects (or simply run `make HE` from the Makefile provided)
       * Create an _objects_ folder `mkdir -p objects`
       * Compile the API `g++ -c source/he.cpp -I HElib/src -o objects/he.o`
       * Compile the helper functions `g++ -c source/helper_functions.cpp -o objects/helper_functions.o`
       * Compile the various tests
          * `g++ -c source/test_gates.cpp -I HElib/src -o objects/test_gates.o`
          * `g++ -c source/test_circ_comb.cpp -I HElib/src -o objects/test_circ_comb.o`
          * `g++ -c source/test_circ_seq.cpp -I HElib/src -o objects/test_circ_seq.o`
          * `g++ -c source/test_circ_arithm.cpp -I HElib/src -o objects/test_circ_arithm.o`
       * Compile the main.cpp source file `g++ -c source/main.cpp -I HElib/src -o objects/main.o`
       * Compile the objects into HEapp `g++ objects/*.o HElib/src/fhe.a -o HEapp -L/usr/local/lib -lntl -lgmp -lm`
8. Run the program with `./HEapp` & Enjoy !
9. Please refer to my long but complete report on this project, available in the repository.
10. You're welcome to contribute to this repository if you find any better circuits or other circuits and implement them !

## Abstract ##
This project concerns the research and development of a real-use application of homomorphic encryption for cloud computing. The application takes advantage of the various possibilities and limitations of present homomorphic encryption schemes and programming libraries to remain usable in terms of time. The foundations of the application rely on the design of binary operations using homomorphic encryption. All the binary logic gates and various binary blocks were developed and adapted to provide enough functionalities to the application. The project focuses on providing features to cloud computing such as calculating averages on large amounts of encrypted numbers in a relatively short and decent time. The result is an application program interface written in C++ allowing to perform various operations on integers. It thus shows homomorphic encryption can be used today for simple operations if the security is more important than the speed of execution. 


### Acknowledgements ###
Credits to **Shai Halevi** for HElib obviously and thanks for the quick help tips in the Issues section!

Thanks to **Dr. Wei Dai** (Imperial College London) for introducing me to homomorphic encryption

Thanks to **Christian Bodt** (Coinplus) for teaching the basics of cryptography and security, as well as useful coding skills.

Thanks to **Alexandra Rouhana** for her useful discussions which helped overcoming some design restrictions.

Thanks to **Wei Dai** from the Vernam Group at Worcester Polytechnic Institute for keeping me updated with the status of CuHE. 

Thanks to **my dad**, **mother** and **step-mother** for their continuous support throughout my studies.
