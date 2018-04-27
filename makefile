GMP_V = 6.1.2
NTL_V = 10.5.0
COMPILER = g++

hbc : createdirs objects/he.o objects/helper_functions.o \
		objects/test_gates.o objects/test_circ_comb.o objects/test_circ_seq.o \
		objects/test_circ_arithm.o \
		objects/main.o
	$(info )
	$(info Compiling hbc...)
	$(COMPILER) -std=c++11 objects/*.o HElib/src/fhe.a -o hbc -L/usr/local/lib -lntl -lgmp -lm

createdirs :
	mkdir -p objects
	
objects/he.o : src/he.cpp src/he.h
	$(COMPILER) -std=c++11 -c src/he.cpp -I HElib/src -o objects/he.o

objects/helper_functions.o : src/helper_functions.cpp src/helper_functions.h
	$(COMPILER) -std=c++11 -c src/helper_functions.cpp -o objects/helper_functions.o
	
objects/test_gates.o : src/TEST_GATES.cpp src/TEST_GATES.h
	$(COMPILER) -std=c++11 -c src/TEST_GATES.cpp -I HElib/src -o objects/test_gates.o
	
objects/test_circ_comb.o : src/TEST_CIRC_COMB.cpp src/TEST_CIRC_COMB.h
	$(COMPILER) -std=c++11 -c src/TEST_CIRC_COMB.cpp -I HElib/src -o objects/test_circ_comb.o
	
objects/test_circ_seq.o : src/TEST_CIRC_SEQ.cpp src/TEST_CIRC_SEQ.h
	$(COMPILER) -std=c++11 -c src/TEST_CIRC_SEQ.cpp -I HElib/src -o objects/test_circ_seq.o
	
objects/test_circ_arithm.o : src/TEST_CIRC_ARITHM.cpp src/TEST_CIRC_ARITHM.h
	$(COMPILER) -std=c++11 -c src/TEST_CIRC_ARITHM.cpp -I HElib/src -o objects/test_circ_arithm.o
	
objects/main.o: src/main.cpp
	$(COMPILER) -std=c++11 -c src/main.cpp -I HElib/src -o objects/main.o
	
hbcNrun : hbc
	./hbc
	
ini :
	sudo apt-get install -y git $(COMPILER) libboost-all-dev
	apt-get install -y git $(COMPILER) libboost-all-dev

gmp : ini
	$(info Installing GMP...)
	sudo apt-get install -y m4 perl
	apt-get install -y m4 perl
	wget https://gmplib.org/download/gmp/gmp-$(GMP_V).tar.bz2
	tar -xvjf gmp-$(GMP_V).tar.bz2
	#cd gmp-$(GMP_V) && ./configure ABI=64
	cd gmp-$(GMP_V) && ./configure
	cd gmp-$(GMP_V) && make
	cd gmp-$(GMP_V) && sudo make install
	cd gmp-$(GMP_V) && make check
	rm -fr gmp-$(GMP_V)*	

ntl : ini gmp
	$(info Installing NTL...)
	wget http://www.shoup.net/ntl/ntl-$(NTL_V).tar.gz
	tar -xvf ntl-$(NTL_V).tar.gz
	#cd ntl-$(NTL_V)/src && ./configure NTL_GMP_LIP=on CFLAGS="-O2 -m64"
	cd ntl-$(NTL_V)/src && ./configure NTL_GMP_LIP=on
	cd ntl-$(NTL_V)/src && make
	cd ntl-$(NTL_V)/src && sudo make install
	rm -fr ntl-$(NTL_V)*
	
HElib : gmp ntl
	$(info Installing HELib...)
	if [ ! -d "HElib" ]; then git clone https://github.com/shaih/HElib.git; fi
	cd HElib/src && make
	#cd HElib/src && make check
	#cd HElib/src && make test
	
clean :
	rm -fr objects *.exe *.o
	
deepclean :
	$(info Cleaning up everything !)
	rm -fr /usr/local/include/NTL
	rm -f /usr/local/include/gmp.h
	rm -f /usr/local/lib/libgmp.*
	rm -f /usr/local/lib/libntl.*
	rm -fr HElib
	rm -fr src

help : 
	@echo make hbc - Compiles the project source code into an executable "hbc"
	@echo make hbcNrun - Compiles the project source code and runs it.
	@echo make HElib - Downloads HElib and other libraries and installs them (alreaduy done in Vagrant)
	@echo make clean - Removes all executables .exe and objects .o
	@echo make deepclean - Removes all the libraries and packages installed. BE CAUTIOUS!
