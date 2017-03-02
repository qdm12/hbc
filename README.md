# Homomorphic binary circuits - _hbc_

*This project was developed as my Master final year project at Imperial College London.*

*This project is still maintained by Quentin McGaw (email: quentin . mcgaw at gmail . com)*

## 1. What is it, in *one* line?
It is an **API** of homomorphic binary operations such as binary comparison or binary Euclidian division using the library _HElib_.

## 2. What is in there?
- It is written in C++ and is cross-platform with Vagrant
- The **core API** is in *src/he.cpp*.
- The code that is ran as an **example** is in *src/main.cpp*.
- Some other classes are in *src/helper_functions.cpp*
- All the other src files are **unit tests** and timing tests for the homomorphic binary operations implemented in *src/he.cpp*.
- There is a **Vagrantfile** to setup eveything for you, cross-platform.
- There is a **makefile** to build *hbc* or setup almost everything for you (depending on your OS).
- There is this complete, detailed and updated **README.md** file.
- There are links in this readme to my **presentation** (VERY USEFUL) and my detailed report (WAY TOO LONG).

## 3. What does it run ?
- It runs the code in *main.cpp* which execute the unit tests on all the homomorphic circuits implemented.
- You can change the main.cpp with your code by inspiring from the tests.
- You can also add circuits in *he.cpp* and then add corresponding tests, and **tell me** you want to contribute !

## 4. What does it require ?
- Practically:
    - A Linux/Windows/OSX computer
    - At least 3GB of RAM and 2 CPU cores
    - An internet connection
    - CPU with Hardware virtualization tech (you probably have it don't worry)
- In terms of software (although this is automatically installed): ![hbc dependencies diagram][dependencies_jpg]

## 5. Documentation
- This readme file
- **Presentation** available on [Dropbox][dropbox_pres], on [Github][github_pres] and on [Youtube](https://www.youtube.com/watch?v=n-adgQWZYxI) where I present it.
- Report available on [Github][github_report] or on [Dropbox][dropbox_report]
- Comments in the source code, especially in _he.cpp_

## 6. Abstract ##
This project concerns the research and development of a real-use application of homomorphic encryption for cloud computing. The application takes advantage of the various possibilities and limitations of present homomorphic encryption schemes and programming libraries to remain usable in terms of time. The foundations of the application rely on the design of binary operations using homomorphic encryption. All the binary logic gates and various binary blocks were developed and adapted to provide enough functionalities to the application. The project focuses on providing features to cloud computing such as calculating averages on large amounts of encrypted numbers in a relatively short and decent time. The result is an application program interface written in C++ allowing to perform various operations on integers. It thus shows homomorphic encryption can be used today for simple operations if the security is more important than the speed of execution. 

## 7. How do I run it?
### 7.1 Using Vagrant (easiest, compatible with all, most flexible)
1. Install git on your computer
    - `apt-get install -y git` for Linux machines
    - or download it from [git-scm.com/downloads](https://git-scm.com/downloads)
2. On Windows, have an ssh client or add the **ssh.exe** of `C:\Program Files\Git\usr\bin` to your environment path
2. Install Virtual Box from [virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads)
3. Install Vagrant from [vagrantup.com/downloads.html](https://www.vagrantup.com/downloads.html)
4. Open a terminal and enter `git clone https://github.com/qdm12/hbc.git`
5. Go to the hbc directory with `cd hbc`
6. Enter `vagrant up` to launch the virtual machine which will setup and build everything for you. 
This takes about 30 minutes the first time, depending on your connection speed and CPU.
This basically launches an Ubuntu-based virtual machine with only what is necessary for this project.
**WARNING:** If you do not have hardware virtualization, you can still run it but you have to change *trusty64*
 to *trusty32* and *vb.cpus = 2* to *vb.cpus = 1*.
7. Once vagrant up has completed, enter `vagrant ssh` to log in the virtual machine.
8. The working directory *hbc* on your host machine is shared with the virtual machine at `/vagrant`.
9. In the virtual machine, enter `cd /vagrant`.
10. What's nice then:
    - You can modify the files on your host machine (like Windows etc.)
    - Changes you make are automatically reflected in the Ubuntu-based virtual machine.
    - Compile hbc again with `make hbc` in the virtual machine.
    - Run hbc with ./hbc from the virtual machine or your host machine.
    - **Note:** *You can use `make hbcNrun` to build and automatically run the main.cpp code.
11. When you are done:
    - Enter `exit` in the virtual machine, bringing you back to your host machine.
    - Enter `vagrant halt` to shutdown the machine. Or enter `vagrant destroy` to delete the machine.
12. To log back in, enter `vagrant up` and it should take about 30 seconds ! (except if you destroy the machine)

### 7.2 Using more manual ways, which don't work for all OSes
1. Make sure you have installed **make**
2. Run the makefile or your terminal as **root** or **administrator**.
3. Setup the necessary libraries
    - With the Makefile provided (should work for both Debian and Ubuntu)
        1. Note: *git, g++, m4, perl, gmp and ntl* will be installed automatically*.
        2. Enter `make HElib` in a terminal in the *hbc* directory.
    - Manually (if Vagrant and Makefile are not good for you)
        - Mac OSX
            1. Install Xcode manually or with `xcode-select --install`
            2. Install brew with `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
            3. Install libraries with `brew install wget git g++ m4 perl libboost`
            4. Download GMP with `curl https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2 > gmp-6.1.2.tar.bz2`
            5. Extract it and go to its directory with `tar -xvjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            6. Configure it with `./configure`
            7. Build it with `make`
            8. Install it with `make install`
            9. *Optionally*, check it with `make check`
            10. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            10. Download NTL with `curl http://www.shoup.net/ntl/ntl-9.9.1.tar.gz > ntl-9.9.1.tar.gz`
            11. Extract it and go to its directory with `tar -xvzf ntl-9.9.1.tar.gz && cd ntl-9.9.1/src`
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
            1. Install the libaries with (add `*sudo*` maybe) `apt-get install git g++ m4 perl libboost-all-dev`
            2. Download GMP with `wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2`
            3. Extract it and go to its directory with `tar -xvjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            4. Configure it with `./configure`
            5. Build it with `make`
            6. Install it with `make install`
            7. *Optionally*, check it with `make check`
            8. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            9. Download NTL with `wget http://www.shoup.net/ntl/ntl-9.9.1.tar.gz`
            10. Extract it and go to its directory with `tar -xvzf ntl-9.9.1.tar.gz && cd ntl-9.9.1/src`
            11. Configure it with `./configure NTL_GMP_LIP=on`
            12. Build it with `make`
            13. Install it with `make install`
            14. Go back and remove used files with `cd ../.. && rm -fr ntl-9.9.1*`
            15. Clone HElib with with `git clone https://github.com/shaih/HElib.git`
            16. Go to its src directory `cd HElib/src`
            17. Build it with `make`
            18. *Optionally*, check it with `make check` and test it with `make test`.
            19. Go back to the working directory with `cd ../..`
        - Cygwin 32 bit and 64 bit
            1. It will not work for mysterious reasons, sorry ! Use Vagrant !
4. Build hbc
    - With the Makefile provided (compatible will **all** platforms).
        1. Build it with `make hbc`
    - Manually (Works on everything, even Cygwin)
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


## 8. Remove and uninstall ##
### 8.1 With Vagrant
Just enter `vagrant destroy` from your host machine in the working directory

### 8.2 With Debian and Ubuntu
Use the makefile and run `make deepclean` which uninstalls and delete:
- hbc
- HElib, NTL, GMP
- perl, m4, git, gcc-g++ and libboost-all-dev and purge them.
Only the makefile will remain in the folder.


## 9. Acknowledgements ##
Credits to **Shai Halevi** for HElib obviously and thanks for the quick help tips in the Issues section!

Thanks to **Dr. Wei Dai** (Imperial College London) for introducing me to homomorphic encryption

Thanks to **Christian Bodt** (Coinplus) for teaching me the basics of cryptography and security, as well as useful coding skills.

Thanks to **Alexandra Rouhana** for her useful discussions that helped me figure out how to overcome some design restrictions.

Thanks to **Wei Dai** from the Vernam Group at Worcester Polytechnic Institute for keeping me updated with the status of CuHE. 

Thanks to **my dad**, **mother** and **step-mother** for their continuous support throughout my studies.


## 10. Contribution
You're welcome to contribute to this repository if you find any better circuits or other circuits and implement them !

Just send me an email (see my address at the start) and I will add you as a contributor.

Don't hesitate to contact me if you have any question as well.


## 11. Future Work (ideas crossing my mind) ####
- Use *= instead of multiplyBy when the level is very low as it is faster. multiplyBy uses relinearization which serves to reduce the size of ciphertexts. This is great for complex circuits but takes a longer time than *= for simple circuits.
- Add circuits from [here](http://www.aoki.ecei.tohoku.ac.jp/arith/mg/algorithm.html) to the core API *he.cpp*.
- Other ideas are in chapter 9: _Future work_ of my report.

[dependencies_jpg]: /docs/dependencies.jpg
[dropbox_pres]: https://www.dropbox.com/s/scrwpum0avtqxuw/Presentation.pptx?dl=1
[github_pres]: /docs/Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20presentation.pptx
[youtube_pres]: https://www.youtube.com/watch?v=n-adgQWZYxI
[dropbox_report]: https://www.dropbox.com/s/rqnrslzb1pstkq0/FYP%20report%20-%20Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20-%20Quentin%20McGaw%20qdm12%202016.pdf?dl=1
[github_report]: /docs/Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20report.pdf