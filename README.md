__*WARNING : *__ This may not work on Debian based operating systems for now. This is being tested now but could actually work on a fresh installation. You can refer to https://github.com/shaih/HElib/issues/99 for more information on the issue.

# Homomorphic binary circuits - _hbc_ #

_This project was developed as my Master final year project at Imperial College London._

## What is it, in **one** line? ##
It is an **API** of homomorphic binary operations such as binary comparisons or binary divisions using the library _HElib_.

## What is in there? ##
The core API is in the file _he.cpp_, the main code is in _main.cpp_, some other classes are in *helper_functions.cpp* and the rest are unit tests and timing tests for the homomorphic binary operations implemented. There is a _makefile_ to setup everything. My report is there to server as documentation for instance.

## What does it run ? ##
By default, it runs a set of unit tests on all the circuits implemented (initiated from _main.cpp_).
You can change the tests, or _main.cpp_ or whatever and it should still work.

## Documentation ##
- Report available [here](https://www.dropbox.com/s/rqnrslzb1pstkq0/FYP%20report%20-%20Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20-%20Quentin%20McGaw%20qdm12%202016.pdf?dl=1)
- Presentation available [here](https://www.dropbox.com/s/scrwpum0avtqxuw/Presentation.pptx?dl=1)
- Comments in the source code, especially in _he.cpp_

## How do I run it? ##
In all scenarios, you will need the module **make** and to run as **root** or **administrator**.

1. You will need several libraries.
    - Use the _Makefile_ following the instructions in the **Makefile** section. **THIS is not compatible with all OS**
    - OR do it manually following the instructions in the **Manual Setup** section.
2. You have to compile the project.
    - Use the _Makefile_ with `make project`, this should work for all platforms.
    - OR do it manually following the instructions in the **Manual Setup** section.

## 1. Setup libraries with the Makefile ##
This is only compatible with _Cygwin 32bit_ and _Debian-based Linux_.

**CYGWIN note:** Due to permission restrictions on Windows, install git and gcc-g++ manually with the Cygwin installer.

When the makefile is launched, note that the following modules will be installed if not present:
* apt-cyg (_Cygwin 32bit_ only)
* git (_Linux_)
* gcc-g++ (_Linux_)
* curl
* m4
* perl

To install the project the first time:
- Move the makefile to an empty directory with `cd ..; mkdir -p new_dir; cd new_dir`
- To download, compile and install HElib and other libraries: `make HElib`
- You can try `make help` for more information

## 1. Setup libraries manually ##
0. For _Mac OSX_, to simplify the task, you can install Brew with:
    - Install Xcode manually or with `xcode-select --install`
    - Install brew with `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
1. Install the modules with `apt install curl git g++ m4 perl` or `brew install curl git g++ m4 perl`.
2. Install gmp:
    * Download it with `curl https://gmplib.org/download/gmp/gmp-6.1.0.tar.bz2 > gmp.tar.bz2`
    * Extract it `tar xjf gmp.tar.bz2`
    * Go to it directory `cd gmp-6.1.0`
    * Configure it `./configure`
    * Make it `make`
    * Install it `make install`
    * Optionally, check it `make check`
    * **For Cygwin 32bit**, `cp -f /usr/local/lib/libgmp.* /usr/XXXXX-pc-cygwin/lib/` where XXXXX is your CPU architecture.
    * Go back to the main directory `cd ..`
3. Install NTL:
    * Download it with `curl http://www.shoup.net/ntl/ntl-9.9.1.tar.gz > ntl.tar.gz`
    * Extract it `tar xf ntl.tar.gz`
    * Go to its source directory `cd ntl-9.9.1/src`
    * Configure it `./configure NTL_GMP_LIP=on`
    * Make it `make`
    * Install it `make install`
    * Go back to main directory `cd ../..`
5. Install HElib
    * Clone it with `git clone https://github.com/shaih/HElib.git`
    * **For Cygwin 32bit**, do `sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp` or there will be a compile error.
    * Go to its source directory `cd HElib/src`
    * Make it `make`
    * Check it `make check`
    * Optionally test it `make test`
    * Go back to the main directory `cd ../..`

## 2. Setup the project with the Makefile ##
This is compatible will **all** platforms.
The final executable file is HEapp in the root directory which can be run with `./HEapp`.
1. To download, compile and run the project code: `make project`
2. To re-compile the project _source_ directory with your modifications: `make HE`
3. You can try `make help` for more information
**NOTE:** The project source code is now in the _./source_ directory, and the GitHub repository is in __./hbc_git__.

## 2. Setup the project manually ##
1. Download this project with `git clone https://github.com/qdm12/hbc.git ./hbc_git`
2. Create a source folder `mkdir -p source` at the root (where you should be)
3. Copy the source files `cp ./hbc_git/* ./source`
5. Create an _objects_ folder `mkdir -p objects`
6. Compile the API `g++ -c source/he.cpp -I HElib/src -o objects/he.o`
7. Compile the helper functions `g++ -c source/helper_functions.cpp -o objects/helper_functions.o`
8. Compile the various tests
    * `g++ -c source/TEST_GATES.cpp -I HElib/src -o objects/test_gates.o`
    * `g++ -c source/TEST_CIRC_COMB.cpp -I HElib/src -o objects/test_circ_comb.o`
    * `g++ -c source/TEST_CIRC_SEQ.cpp -I HElib/src -o objects/test_circ_seq.o`
    * `g++ -c source/TEST_CIRC_ARITHM.cpp -I HElib/src -o objects/test_circ_arithm.o`
9. Compile the main.cpp source file `g++ -c source/main.cpp -I HElib/src -o objects/main.o`
10. Compile the objects into HEapp `g++ objects/*.o HElib/src/fhe.a -o HEapp -L/usr/local/lib -lntl -lgmp -lm`
11. Run the program with `./HEapp` & Enjoy !
    Please refer to my long but complete report on this project, available in the repository.
    You're welcome to contribute to this repository if you find any better circuits or other circuits and implement them !

## 3. Remove and uninstall ##
With the makefile you can run `make clean` which simply removes .o and .exe files as well as HEapp.
You can also run `make deepclean` which will uninstall from your system and delete:
- The project
- HElib
- NTL
- GMP
- Curl, perl, m4, git, gcc-g++ and libboost (libboost-devel, libboost-all-dev, libboost-dev) and purge them.
Only the makefile will remain in the folder.


## Abstract ##
This project concerns the research and development of a real-use application of homomorphic encryption for cloud computing. The application takes advantage of the various possibilities and limitations of present homomorphic encryption schemes and programming libraries to remain usable in terms of time. The foundations of the application rely on the design of binary operations using homomorphic encryption. All the binary logic gates and various binary blocks were developed and adapted to provide enough functionalities to the application. The project focuses on providing features to cloud computing such as calculating averages on large amounts of encrypted numbers in a relatively short and decent time. The result is an application program interface written in C++ allowing to perform various operations on integers. It thus shows homomorphic encryption can be used today for simple operations if the security is more important than the speed of execution. 


### Acknowledgements ###
Credits to **Shai Halevi** for HElib obviously and thanks for the quick help tips in the Issues section!

Thanks to **Dr. Wei Dai** (Imperial College London) for introducing me to homomorphic encryption

Thanks to **Christian Bodt** (Coinplus) for teaching me the basics of cryptography and security, as well as useful coding skills.

Thanks to **Alexandra Rouhana** for her useful discussions that helped me figure out how to overcome some design restrictions.

Thanks to **Wei Dai** from the Vernam Group at Worcester Polytechnic Institute for keeping me updated with the status of CuHE. 

Thanks to **my dad**, **mother** and **step-mother** for their continuous support throughout my studies.

#### New ideas which crossed my mind ####
- Use *= instead of multiplyBy when the level is very low as it is faster. multiplyBy uses relinearization which serves to reduce the size of ciphertexts. This is great for complex circuits but takes a longer time than *= for simple circuits.
- Add circuits from [here](http://www.aoki.ecei.tohoku.ac.jp/arith/mg/algorithm.html) to the core API
- Other ideas are in chapter 9: _Future work_ of my report.
