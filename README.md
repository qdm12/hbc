# Homomorphic binary circuits - _hbc_

_This project was developed as my Master final year project at Imperial College London._
_This project is still maintained by Quentin McGaw quentin . mcgaw at gmail . com

## What is it, in **one** line?
It is an **API** of homomorphic binary operations such as binary comparisons or binary divisions using the library _HElib_.

## What is in there?
The core API is in the file _he.cpp_, the main code is in _main.cpp_, some other classes are in *helper_functions.cpp* and the rest are unit tests and timing tests for the homomorphic binary operations implemented. There is a _makefile_ to setup everything. My report is there to server as documentation for instance.

## What does it run ?
By default, it runs a set of unit tests on all the circuits implemented (initiated from _main.cpp_).
You can change the tests, or _main.cpp_ or whatever and it should still work.

## Documentation
- *Presentation* available [here](https://www.dropbox.com/s/scrwpum0avtqxuw/Presentation.pptx?dl=1)
- Report available [here](https://www.dropbox.com/s/rqnrslzb1pstkq0/FYP%20report%20-%20Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20-%20Quentin%20McGaw%20qdm12%202016.pdf?dl=1)
- Comments in the source code, especially in _he.cpp_

## How do I run it?
### Using Vagrant (easiest, compatible with all, most flexible)
1. Install git on your computer
    - `apt-get install -y git` for Linux machines
    - or download it from [git-scm.com/downloads](https://git-scm.com/downloads)
2. Install Virtual Box from [virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads)
3. Install Vagrant from [vagrantup.com/downloads.html](https://www.vagrantup.com/downloads.html)
4. Open a terminal and enter `git clone https://github.com/qdm12/hbc.git`
5. Go to the hbc directory with `cd hbc`
6. Enter `vagrant up` to launch the virtual machine which will setup and build everything for you. 
This takes about 30 minutes the first time, depending on your connection speed and CPU.
This basically launches an Ubuntu-based virtual machine with only what is necessary for this project.
7. Once vagrant up has completed, enter `vagrant ssh` to log in the virtual machine.
8. The working directory *hbc* on your host machine is shared with the virtual machine at `/vagrant`.
9. In the virtual machine, enter `cd /vagrant`.
10. What's nice then:
    - You can modify the files on your host machine (like Windows
    - Changes you make are automatically reflected in the virtual machine.
    - Compile hbc again with `make hbc` in the virtual machine.
    - Run hbc with ./hbc from the virtual machine or your host machine.
    - **Note:** *You can use `make hbcNrun` to build and automatically run the main.cpp code.
11. When you are done:
    - Enter `exit` in the virtual machine, bringing you back to your host machine.
    - Enter `vagrant halt` to shutdown the machine. Or enter `vagrant destroy` to delete the machine.
12. To log back in, enter `vagrant up` and it should take about 30 seconds ! (except if you destroy the machine)


### Using more manual ways, which don't work for all OSes
1. Make sure you have installed **make**
2. Run the makefile or your terminal as **root** or **administrator**.
3. Setup the necessary libraries
    - With the Makefile provided (*only works for **Cygwin 32 bit** and **Debian** *)
        - Cygwin 32 bit
            1. Manually install git and gcc-g++ (Windows restrictions)
            2. Note: *apt-cyg, m4, perl, gmp and ntl* will be installed automatically*.
            3. Enter `make HElib` in a terminal in the *hbc* directory.
        - Debian
            1. Note: *git, g++, m4, perl, gmp and ntl* will be installed automatically*.
            2. Enter `make HElib` in a terminal in the *hbc* directory.
    - Manually (if Vagrant and Makefile are not good for you)
        - Mac OSX
            1. Install Xcode manually or with `xcode-select --install`
            2. Install brew with `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
            3. Install libraries with `brew install wget git g++ m4 perl lzip XXX libboost`
            4. Download GMP with `curl https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2 > gmp-6.1.2.tar.bz2`
            5. Extract it and go to its directory with `tar xjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            6. Configure it with `./configure`
            7. Build it with `make`
            8. Install it with `make install`
            9. *Optionally*, check it with `make check`
            10. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            10. Download NTL with `curl http://www.shoup.net/ntl/ntl-9.9.1.tar.gz > ntl-9.9.1.tar.gz`
            11. Extract it and go to its directory with `tar xvzf ntl-9.9.1.tar.gz && cd ntl-9.9.1/src`
            12. Configure it with `./configure NTL_GMP_LIP=on`
            13. Build it with `make`
            14. Install it with `make install`
            15. Go back and remove used files with `cd ../.. && rm -fr ntl-9.9.1*`
            16. Clone HElib with with `git clone https://github.com/shaih/HElib.git`
            17. Go to its src directory `cd HElib/src`
            18. Build it with `make`
            19. *Optionally*, check it with `make check` and test it with `make test`.
            20. Go back to the working directory with `cd ../..`
        - Other Linux OSes
            1. Install the libaries with (add `*sudo*` maybe) `apt-get install git g++ m4 perl lzip libboost-all-dev`
            2. Download GMP with `wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2`
            3. Extract it and go to its directory with `tar xjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            4. Configure it with `./configure`
            5. Build it with `make`
            6. Install it with `make install`
            7. *Optionally*, check it with `make check`
            8. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            9. Download NTL with `wget http://www.shoup.net/ntl/ntl-9.9.1.tar.gz`
            10. Extract it and go to its directory with `tar xvzf ntl-9.9.1.tar.gz && cd ntl-9.9.1/src`
            11. Configure it with `./configure NTL_GMP_LIP=on`
            12. Build it with `make`
            13. Install it with `make install`
            14. Go back and remove used files with `cd ../.. && rm -fr ntl-9.9.1*`
            15. Clone HElib with with `git clone https://github.com/shaih/HElib.git`
            16. Go to its src directory `cd HElib/src`
            17. Build it with `make`
            18. *Optionally*, check it with `make check` and test it with `make test`.
            19. Go back to the working directory with `cd ../..`
        - Cygwin 32 bit
            1. Install apt-cyg with 
            2. Manually install git and gcc-g++ (and make) with the Cygwin installer.
            3. Install the libaries with `apt-cyg install git g++ m4 perl lzip libboost-devel`
            4. Download GMP with `wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2`
            5. Extract it and go to its directory with `tar xjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            6. Configure it with `./configure`
            7. Build it with `make`
            6. Install it with `make install`
            7. *Optionally*, check it with `make check`
            8. Enter `cp -f /usr/local/lib/libgmp.* /usr/**XXXXX**-pc-cygwin/lib/` where **XXXXX** is your CPU architecture.
            9. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            10. Download NTL with `wget http://www.shoup.net/ntl/ntl-9.9.1.tar.gz`
            11. Extract it and go to its src directory with `tar xvzf ntl-9.9.1.tar.gz && cd ntl-9.9.1/src`
            12. Configure it with `./configure NTL_GMP_LIP=on`
            13. Build it with `make`
            14. Install it with `make install`
            15. Go back and remove used files with `cd ../.. && rm -fr ntl-9.9.1*`
            16. Clone HElib with with `git clone https://github.com/shaih/HElib.git`
            17. Enter `sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp` or there will be a compile error.
            18. Go to its src directory `cd HElib/src`
            19. Build it with `make`
            20. *Optionally*, check it with `make check` and test it with `make test`.
            21. Go back to the working directory with `cd ../..`
4. Build hbc
    - With the Makefile provided (compatible will **all** platforms).
        1. Build it with `make hbc`
    - Manually
        1. Create the directory objects `mkdir -p objects`
        2. Compile the API `g++ -c src/he.cpp -I HElib/src -o objects/he.o`
        3. Compile the helper functions `g++ -c src/helper_functions.cpp -o objects/helper_functions.o`
        4. Compile the various tests
            * `g++ -c src/TEST_GATES.cpp -I HElib/src -o objects/test_gates.o`
            * `g++ -c src/TEST_CIRC_COMB.cpp -I HElib/src -o objects/test_circ_comb.o`
            * `g++ -c src/TEST_CIRC_SEQ.cpp -I HElib/src -o objects/test_circ_seq.o`
            * `g++ -c src/TEST_CIRC_ARITHM.cpp -I HElib/src -o objects/test_circ_arithm.o`
        5. Compile the main.cpp file `g++ -c src/main.cpp -I HElib/src -o objects/main.o`
        6. Compile the objects into hbc `g++ objects/*.o HElib/src/fhe.a -o hbc -L/usr/local/lib -lntl -lgmp -lm`
5. Run hbc
    - If on Vagrant, `cd /vagrant` first
    - Run it with `./hbc` && Enjoy ! (Careful about having enough **RAM**)
    - You can also build it and run the new build with `make hbcNrun`
6. RAM considerations **IMPORTANT**
    - To run the default hbc program, you need at least 3GB of ram.
    - This is because the average operation (arithmetic tests) uses about 2GB of ram for 2 bits.
    - Note that you can comment it out in the main.cpp or TEST_ARITHMETIC.cpp 
      and stick to tests of simpler circuits such as the multiplication which 
      only require about 0.7 - 1GB of ram. You can modify the amount of RAM of 
      Vagrant in the vb.memory field, which is set to 2600MB by default.
    - To monitor the RAM on Vagrant, open a new host terminal, go to the working
      directory and use `vagrant ssh -c htop`.
7. CPU considerations
    - By default, the Vagrant VM uses 2 cores of your CPU (vb.cpus = 2) so that
      you can run hbc and also monitor the RAM with another `vagrant ssh`.
    - You can also run more instances of hbc if you have more than two cores available.
      With Vagrant, just set vb.cpus to 3 for example, log in with `vagrant ssh` on different
      host terminals and run hbc (provided you have enough ram to run both obviously).


## Remove and uninstall ##
### With Vagrant
Just enter `vagrant destroy` from your host machine in the working directory

### With Cygwin 32 bit
Use the makefile and run `make deepclean` which uninstalls and delete:
- hbc
- HElib, NTL, GMP
- perl, m4, git, gcc-g++ and libboost-devel and purge them.
Only the makefile will remain in the folder.

### With Debian
Use the makefile and run `make deepclean` which uninstalls and delete:
- hbc
- HElib, NTL, GMP
- git, g++, perl, m4, git, gcc-g++, libboost-all-dev and libboost-dev and purge them.


## Contribution
You're welcome to contribute to this repository if you find any better circuits or other circuits and implement them !
Just send me an email (see my address at the start) and I will add you as a contributor.
Don't hesitate to contact me if you have any question as well.

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
