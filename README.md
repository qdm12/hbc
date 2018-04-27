# Homomorphic binary circuits - *hbc*

**API** of homomorphic binary operations such as binary comparison or binary Euclidian division using the library [*HElib*](https://github.com/shaih/HElib)

*Still maintained (April 2018) by Quentin McGaw (email: quentin.mcgaw@gmail.com)*

[![Build Status](https://travis-ci.org/qdm12/hbc.svg?branch=master)](https://travis-ci.org/qdm12/hbc)

[![GitHub last commit](https://img.shields.io/github/last-commit/qdm12/hbc.svg)](https://github.com/qdm12/hbc/issues)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/y/qdm12/hbc.svg)](https://github.com/qdm12/hbc/issues)
[![GitHub issues](https://img.shields.io/github/issues/qdm12/hbc.svg)](https://github.com/qdm12/hbc/issues)

[![Docker Build Status](https://img.shields.io/docker/build/qmcgaw/homomorphic-binary-circuits.svg)](https://hub.docker.com/r/qmcgaw/homomorphic-binary-circuits)
[![Docker Pulls](https://img.shields.io/docker/pulls/qmcgaw/homomorphic-binary-circuits.svg)](https://hub.docker.com/r/qmcgaw/homomorphic-binary-circuits)
[![Docker Stars](https://img.shields.io/docker/stars/qmcgaw/homomorphic-binary-circuits.svg)](https://hub.docker.com/r/qmcgaw/homomorphic-binary-circuits)
[![Docker Automated](https://img.shields.io/docker/automated/qmcgaw/homomorphic-binary-circuits.svg)](https://hub.docker.com/r/qmcgaw/homomorphic-binary-circuits)
[![](https://images.microbadger.com/badges/image/qmcgaw/homomorphic-binary-circuits.svg)](https://microbadger.com/images/qmcgaw/homomorphic-binary-circuits)
[![](https://images.microbadger.com/badges/version/qmcgaw/homomorphic-binary-circuits.svg)](https://microbadger.com/images/qmcgaw/homomorphic-binary-circuits)

This project concerns the research and development of a real-use application of homomorphic encryption for cloud computing. The application takes advantage of the various possibilities and limitations of present homomorphic encryption schemes and programming libraries to remain usable in terms of time. The foundations of the application rely on the design of binary operations using homomorphic encryption. All the binary logic gates and various binary blocks were developed and adapted to provide enough functionalities to the application. The project focuses on providing features to cloud computing such as calculating averages on large amounts of encrypted numbers in a relatively short and decent time. The result is an application program interface written in C++ allowing to perform various operations on integers. It thus shows homomorphic encryption can be used today for simple operations if the security is more important than the speed of execution. 

- Written in C++
- Cross-platform using either:
	- Docker (see [Dockerfile](https://github.com/qdm12/hbc/blob/master/Dockerfile))
	- Vagrant (see [Vagrantfile](https://github.com/qdm12/hbc/blob/master/Vagrantfile))
- API is in [*src/he.cpp*](https://github.com/qdm12/hbc/blob/master/src/he.cpp)
- [*src/TEST_*](https://github.com/qdm12/hbc/tree/master/src) files are **unit tests** and timing tests for the homomorphic binary operations implemented in *src/he.cpp*. You should inspire for them to develop your own code.
- **makefile** to build *hbc* and/or setup almost everything for you (depending on your OS).
- Powerpoint Presentation available on [Dropbox][dropbox_pres], on [Github][github_pres] and on [Youtube](https://www.youtube.com/watch?v=n-adgQWZYxI) where I present it.
- Detailed report available on [Github][github_report] or on [Dropbox][dropbox_report]

*This project was developed as my Master thesis at Imperial College London.*

## 1. Your own code

- By default, [*src/main.cpp*](https://github.com/qdm12/hbc/blob/master/src/main.cpp) runs all the unit tests of the project
- Change main.cpp with your code by inspiring from the tests [*src/TEST_*](https://github.com/qdm12/hbc/tree/master/src)
- You can also add circuits in [*src/he.cpp*](https://github.com/qdm12/hbc/blob/master/src/he.cpp) and then add corresponding tests, and **tell me** if you want to contribute !
- Refer to the [Running it section](#running-it)

## 2. Running it

### 2.1. Docker (easiest)

[![Docker](https://github.com/qdm12/hbc/raw/master/readme/docker.png)](https://hub.docker.com/r/qmcgaw/homomorphic-binary-circuits/)

Requirements:
- A Linux based machine, MacOS or Windows Enterprise
- At least 3GB of RAM
- An internet connection

1. You need a Linux based machine, MacOS or Windows Enterprise for Docker
1. Install Docker (from [here](https://www.docker.com/community-edition))
1. Place the content of the `src/` directory from the hbc repository in `/yourpath` in example
1. Enter the following command in your terminal:

	```bash
	docker run -it --rm -v /yourpath:/hbc/src qmcgaw/homomorphic-binary-circuits
	```
	
	This will download the Docker image, mount the source files in the Docker container and make the hbc binary from 
	the source files. You can then edit the source files and run `make` in the Docker container to re-build it.
	
	You should also run the executable in the Docker container with `/hbc/hbc`
	
### 2.2. Vagrant

![Docker](https://github.com/qdm12/hbc/raw/master/readme/vagrant.png)

Requirements:
- At least 3GB of RAM
- An internet connection

1. Install [Git](https://git-scm.com/downloads)
1. **On Windows**, have an ssh client or add the **ssh.exe** of `C:\Program Files\Git\usr\bin` to your environment path
1. Install [Virtual Box](https://www.virtualbox.org/wiki/Downloads)
1. Install [Vagrant](https://www.vagrantup.com/downloads.html)
1. In a terminal, enter

	```bash
	git clone https://github.com/qdm12/hbc.git
	```

1. Go to the hbc directory

	```bash
	cd hbc
	```
	
1. Launch the virtual machine which will setup and build everything for you with

	```bash
	vagrant up
	```
	
	This takes about 30 minutes the first time, depending on your connection speed and CPU.
	
	It launches an Ubuntu virtual machine with only what is necessary for this project.
	
	**WARNING:** If you do not have hardware virtualization, you can still run it but you have to change *trusty64*
	to *trusty32* and *vb.cpus = 2* to *vb.cpus = 1*.

1. Log in the virtual machine with

	```bash
	vagrant ssh
	```

	The working directory *hbc* on your host machine is shared with the virtual machine at `/vagrant`

1. In the virtual machine, enter

	```bash
	cd /vagrant
	```

- You can modify the files on your host machine (Windows, MacOS etc.)
- Changes you make are automatically reflected in the virtual machine
- Compile hbc again with `make` in the virtual machine
- Run hbc with ./hbc from the virtual machine or your host machine.
- *You can use `make hbcNrun` to build and automatically run the main.cpp code*

When you are done:
- Enter `exit` in the virtual machine, bringing you back to your host machine.
- Enter `vagrant halt` to shutdown the machine. Or enter `vagrant destroy` to delete the machine.

To log back in, enter `vagrant up` and it should take about 30 seconds (except if you destroy the machine)

### 2.3. Manually

Requirements:
- At least 3GB of RAM
- An internet connection

For your information, software dependencies:

| Program or Library | Requirement 1 | Requirement 2 | Requirement 3 | Requirement 4 | Requirement 5 | Requirement 6 |
| ------------------ | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| hbc                | g++           | make          | libboost      | c++11         | HElib         |
| HElib              | g++           | make          | git           | NTL 10.5.0    | GMP 6.1.2     |
| NTL 10.5.0         | g++           | make          | GMP 6.1.2     |
| GMP 6.1.2          | g++           | make          | m4            | perl          |


1. Make sure you have installed **make**
1. Open a terminal as **root** or **administrator** ideally
1. Setup the necessary libraries
    - With the Makefile provided (only works for **Debian** and **Ubuntu**)
        1. Note: *git, g++, m4, perl, gmp and ntl* will be installed automatically
        1. Enter `make HElib` in a terminal in the *hbc* directory.
    - Manually (if Docker, Vagrant and the Makefile are not good for you)
        - Mac OSX
            1. Install Xcode manually or with `xcode-select --install`
            1. Install brew with `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
            1. Install libraries with `brew install wget git g++ m4 perl libboost`
            1. Download GMP with `curl https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2 > gmp-6.1.2.tar.bz2`
            1. Extract it and go to its directory with `tar -xvjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            1. Configure it with `./configure`
            1. Build it with `make`
            1. Install it with `make install`
            1. *Optionally*, check it with `make check`
            1. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            1. Download NTL with `curl http://www.shoup.net/ntl/ntl-10.5.0.tar.gz > ntl-10.5.0.tar.gz`
            1. Extract it and go to its directory with `tar -xvzf ntl-10.5.0.tar.gz && cd ntl-10.5.0/src`
            1. Configure it with `./configure NTL_GMP_LIP=on`
            1. Build it with `make`
            1. Install it with `make install`
            1. Go back and remove used files with `cd ../.. && rm -fr ntl-10.5.0*`
            1. Clone HElib with with `git clone https://github.com/shaih/HElib.git`
            1. Go to its src directory `cd HElib/src`
            1. Build it with `make`
            1. *Optionally*, check it with `make check`
            1. Go back to the working directory with `cd ../..`
        - Other Linux OSes
            1. Install the libaries with (add `sudo` maybe) `apt-get install git g++ m4 perl libboost-all-dev`
            1. Download GMP with `wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2`
            1. Extract it and go to its directory with `tar -xvjf gmp-6.1.2.tar.bz2 && cd gmp-6.1.2`
            1. Configure it with `./configure`
            1. Build it with `make`
            1. Install it with `make install`
            1. *Optionally*, check it with `make check`
            1. Go back and remove used files with `cd .. && rm -fr gmp-6.1.2*`
            1. Download NTL with `wget http://www.shoup.net/ntl/ntl-10.5.0.tar.gz`
            1. Extract it and go to its directory with `tar -xvzf ntl-10.5.0.tar.gz && cd ntl-10.5.0/src`
            1. Configure it with `./configure NTL_GMP_LIP=on`
            1. Build it with `make`
            1. Install it with `make install`
            1. Go back and remove used files with `cd ../.. && rm -fr ntl-10.5.0*`
            1. Clone HElib with with `git clone https://github.com/shaih/HElib.git`
            1. Go to its src directory `cd HElib/src`
            1. Build it with `make`
            1. *Optionally*, check it with `make check` and test it with `make test`.
            1. Go back to the working directory with `cd ../..`
4. Build hbc
    - With the Makefile provided (compatible will **all** platforms).
        1. Build it with `make hbc`
    - Manually
        1. Create the directory objects `mkdir -p objects`
        1. Compile the API `g++ -c src/he.cpp -I HElib/src -o objects/he.o`
        1. Compile the helper functions `g++ -c src/helper_functions.cpp -o objects/helper_functions.o`
        1. Compile the various tests
            - `g++ -c src/TEST_GATES.cpp -I HElib/src -o objects/test_gates.o`
            - `g++ -c src/TEST_CIRC_COMB.cpp -I HElib/src -o objects/test_circ_comb.o`
            - `g++ -c src/TEST_CIRC_SEQ.cpp -I HElib/src -o objects/test_circ_seq.o`
            - `g++ -c src/TEST_CIRC_ARITHM.cpp -I HElib/src -o objects/test_circ_arithm.o`
        1. Compile the main.cpp file `g++ -c src/main.cpp -I HElib/src -o objects/main.o`
        1. Compile the objects into *hbc* `g++ objects/*.o HElib/src/fhe.a -o hbc -L/usr/local/lib -lntl -lgmp -lm`
5. Run hbc
    - Run it with `./hbc` (Careful about having enough **RAM**)
    - You can also build it and run the new build with `make hbcNrun`


## 3. RAM considerations IMPORTANT
- To run the default hbc program, you need at least 3GB of RAM.
- This is because the average operation (arithmetic tests) uses about 2GB of RAM for 2 bits.
- Note that you can comment it out in the main.cpp or TEST_ARITHMETIC.cpp 
  and stick to tests of simpler circuits such as the multiplication which 
  only require about 0.7 - 1GB of RAM. 
- For **Vagrant**, you can modify the amount of RAM in the **vb.memory** field, 
  which is set to **2600MB** by default. To monitor the RAM usage, open a new 
  host terminal, go to the working directory and use `vagrant ssh -c htop`.

	  
## 4. CPU considerations for Vagrant
- By default, the Vagrant VM uses 2 cores of your CPU (vb.cpus = 2) so that
  you can run hbc and also monitor the RAM with another `vagrant ssh`.
- You can also run more instances of hbc if you have more than two cores available.
  With Vagrant, just set vb.cpus to 3 for example, log in with `vagrant ssh` on different
  host terminals and run hbc (provided you have enough RAM to run both obviously).

## 5. Remove and uninstall

### 5.1 Docker

In a terminal on your host machine, enter:

```bash
docker image rm qmcgaw/homomorphic-binary-circuits
```

Then delete the hbc/src directory on your host machine

### 5.1 With Vagrant

In a terminal on your host machine, enter:

```bash
vagrant destroy
```

Then delete the hbc/src directory on your host machine

### 5.2 Otherwise

Use the makefile and run `make deepclean` which uninstalls and delete:
- hbc
- src directory
- HElib, NTL, GMP
Only the makefile will remain in the folder.


## 6. Acknowledgements

Credits to **Shai Halevi** for HElib obviously and thanks for the quick help tips in the Issues section!

Thanks to **Dr. Wei Dai** (Imperial College London) for introducing me to homomorphic encryption

Thanks to **Christian Bodt** (Coinplus) for teaching me the basics of cryptography and security, as well as useful coding skills.

Thanks to **Alexandra Rouhana** for her useful discussions that helped me figure out how to overcome some design restrictions.

Thanks to **Wei Dai** from the Vernam Group at Worcester Polytechnic Institute for keeping me updated with the status of CuHE. 

Thanks to **my dad**, **mother** and **step-mother** for their continuous support throughout my studies.

## 7. Contribution

You're welcome to contribute to this repository if you find any better circuits or other circuits and implement them.

Just send me an email (see my address at the start) and I will add you as a contributor.

Please create an issue on the repository if you have an issue or question. Thanks !

## 8. Future Work (ideas crossing my mind)

- Use *= instead of multiplyBy when the level is very low as it is faster. multiplyBy uses relinearization which serves to reduce the size of ciphertexts. This is great for complex circuits but takes a longer time than *= for simple circuits.
- Add circuits from [here](http://www.aoki.ecei.tohoku.ac.jp/arith/mg/algorithm.html) to the core API *he.cpp*.
- Other ideas are in chapter 9: _Future work_ of my report.

[dependencies_jpg]: /docs/dependencies.jpg
[dropbox_pres]: https://www.dropbox.com/s/scrwpum0avtqxuw/Presentation.pptx?dl=1
[github_pres]: /docs/Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20presentation.pptx
[youtube_pres]: https://www.youtube.com/watch?v=n-adgQWZYxI
[dropbox_report]: https://www.dropbox.com/s/rqnrslzb1pstkq0/FYP%20report%20-%20Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20-%20Quentin%20McGaw%20qdm12%202016.pdf?dl=1
[github_report]: /docs/Homomorphic%20encryption%20Cryptography%20for%20cloud%20computing%20report.pdf