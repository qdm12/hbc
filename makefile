#Requires make for cygwin and an internet connection
GMP_V = 6.1.0
NTL_V = 9.6.2

error:
	$(error make has to be followed by something)

ini :
	$(info Checking pre-requisites...)
ifeq ($(shell uname -o),Cygwin)
	$(info Cygwin detected, installing modules if necessary.)
	lynx -source rawgit.com/transcode-open/apt-cyg/master/apt-cyg > apt-cyg
	install apt-cyg /bin
	rm -f apt-cyg
	apt-cyg install curl
	#for helib, requires to manually install git on cygwin
	#for ntl, requires to manually install gcc-g++ on cygwin
else
	apt install -y curl git g++
endif
	$(info = = = = = = = = = = = = = = = =)
	$(info All pre-requisites modules are installed.)
	$(info = = = = = = = = = = = = = = = =)

gmp : ini
	$(info Installing GMP...)
	$(info Installing pre-requisites M4 and perl)
ifeq ($(shell uname -o),Cygwin)
	$(info Cygwin detected, installing modules if necessary.)
	apt-cyg install m4 perl
else
	$(info Linux detected, installing modules if necessary.)
	apt install -y m4 perl
endif
	$(info M4 and Perl are installed.)
	curl https://gmplib.org/download/gmp/gmp-6.1.0.tar.bz2 > gmp.tar.bz2
	tar xjf gmp.tar.bz2
	rm -f gmp.tar.bz2
	cd gmp-$(GMP_V) && ./configure
	cd gmp-$(GMP_V) && make
	cd gmp-$(GMP_V) && make install
	#cd gmp-$(GMP_V) && make check
ifeq ($(shell uname -o),Cygwin)
	if [ -d "/usr/x86_64-pc-cygwin/lib/" ]; then cp -f /usr/local/lib/libgmp.* /usr/x86_64-pc-cygwin/lib/; fi
	if [ -d "/usr/i686-pc-cygwin/lib/" ]; then cp -f /usr/local/lib/libgmp.* /usr/i686-pc-cygwin/lib/; fi
endif
	

ntl : ini gmp
	$(info Installing NTL...)
	curl http://www.shoup.net/ntl/ntl-9.6.2.tar.gz > ntl.tar.gz
	tar xf ntl.tar.gz
	rm -f ntl.tar.gz
	cd ntl-$(NTL_V)/src && ./configure NTL_GMP_LIP=on
	cd ntl-$(NTL_V)/src && make
	cd ntl-$(NTL_V)/src && make install
	
HElib : ntl gmp
	$(info Installing HELib...)
	git clone https://github.com/shaih/HElib.git
ifeq ($(shell uname -o),Cygwin)
	sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp
endif
	cd HElib/src && make
	cd HElib/src && make check
	cd HElib/src && make test

setup_gcc :
ifeq ($(shell uname -o),Cygwin)
	$(info Cygwin detected: Be sure to have gcc-g++ and git installed.)
else
	$(info Linux detected, installing modules if necessary.)
	apt install -y git g++
endif

objects/helper_functions.o : source/helper_functions.cpp source/helper_functions.h
	$(info )
	$(info Building helper_functions.o...)
	mkdir -p objects
	g++ -c source/helper_functions.cpp -o objects/helper_functions.o
	
objects/test_gates.o : source/test_gates.cpp source/test_gates.h
	$(info )
	$(info Building test_gates.o...)
	mkdir -p objects
	g++ -c source/test_gates.cpp -I HELib/src -o objects/test_gates.o
	
objects/test_circ_comb.o : source/test_circ_comb.cpp source/test_circ_comb.h
	$(info )
	$(info Building test_circ_comb.o...)
	mkdir -p objects
	g++ -c source/test_circ_comb.cpp -I HELib/src -o objects/test_circ_comb.o
	
objects/test_circ_seq.o : source/test_circ_seq.cpp source/test_circ_seq.h
	$(info )
	$(info Building test_circ_seq.o...)
	mkdir -p objects
	g++ -c source/test_circ_seq.cpp -I HELib/src -o objects/test_circ_seq.o
	
objects/test_circ_arithm.o : source/test_circ_arithm.cpp source/test_circ_arithm.h
	$(info )
	$(info Building test_circ_arithm.o...)
	mkdir -p objects
	g++ -c source/test_circ_arithm.cpp -I HELib/src -o objects/test_circ_arithm.o
	
objects/he.o : source/he.cpp source/he.h
	$(info )
	$(info Building he.o...)
	mkdir -p objects
	g++ -c source/he.cpp -I HELib/src -o objects/he.o
	
objects/main.o: source/main.cpp
	$(info )
	$(info Building main.o...)
	mkdir -p objects
	g++ -c source/main.cpp -I HELib/src -o objects/main.o
	
HEapp : objects/he.o objects/helper_functions.o \
		objects/test_gates.o objects/test_circ_comb.o objects/test_circ_seq.o \
		objects/test_circ_arithm.o \
		objects/main.o
	$(info )
	$(info Building HEapp...)
	g++ objects/*.o HELib/src/fhe.a -o HEapp -L/usr/local/lib -lntl -lgmp -lm
	
HE : setup_gcc HEapp
	./HEapp.exe
	
download : 
	git clone https://github.com/qdm12/hbc.git ./hbc_git
	mkdir -p source
	cp ./hbc_git/source/* ./source
	
project : download HE

	

	
clean :
	rm -fr *.exe *.o
	
deepclean :
	$(info Cleaning up everything !)
	if [ -d "/gmp-$(GMP_V)" ]; then cd gmp-$(GMP_V)/src && make uninstall; fi
	$(info ...Uninstalled GMP)
	if [ -d "/ntl-$(NTL_V)" ]; then cd ntl-$(NTL_V)/src && make uninstall; fi
	$(info ...Uninstalled NTL)
ifeq ($(shell uname -o),Cygwin)
	$(info Cygwin detected, uninstalling static GMP and NTL.)
	rm -f /usr/x86_64-pc-cygwin/lib/libgmp.*
	rm -f /usr/i686-pc-cygwin/lib/libgmp.*
	$(info ...Uninstalled static GMP and NTL)
endif
	rm -fr gmp-$(GMP_V) ntl-$(NTL_V) HElib
	$(info ...Removed GMP, NTL, HELib folder)
ifeq ($(shell uname -o),Cygwin)
	lynx -source rawgit.com/transcode-open/apt-cyg/master/apt-cyg > apt-cyg
	install apt-cyg /bin
	rm -f apt-cyg
	apt-cyg remove curl perl m4 git gcc-g++
	$(info ...Removed curl perl m4 git gcc-g++ from CYGWIN)
else
	apt remove -y perl git g++
endif

help : 
	@echo Available commands are:
	@echo make HElib - Downloads HElib and other libraries and installs them
	@echo make project - Downloads, compiles the project source code and runs it.
	@echo make HEapp - Compiles the project source code into an executable "HEApp"
	@echo make HE - Compiles the project source code and runs it.
	@echo make clean - Removes all executables .exe and objects .o
	@echo make deepclean - Removes all the libraries and packages installed. BE CAUTIOUS!