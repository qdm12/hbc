#Requires make for cygwin and an internet connection
GMP_V = 6.1.0
NTL_V = 9.6.2

ini :
	$(info Checking pre-requisites...)
ifeq ($(shell uname -o),Cygwin)
	$(info Cygwin detected, installing modules if necessary.)
	lynx -source rawgit.com/transcode-open/apt-cyg/master/apt-cyg > apt-cyg
	install apt-cyg /bin
	rm -f apt-cyg
	apt-cyg install wget
	apt-cyg install curl
	apt-cyg install git
else
	$(error Only Cygwin is supported for now.)
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
	$(error Only Cygwin is supported for now.)
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
	cp -f /usr/local/lib/libgmp.* /usr/x86_64-pc-cygwin/lib/
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
	sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp
	cd HElib/src && make
	cd HElib/src && make check
	cd HElib/src && make test
	
HEapp :
	$(info Building custom code from main.cpp...)
	$(info )
ifeq ($(shell uname -o),Cygwin)
	$(info Cygwin detected, installing modules if necessary.)
	apt-cyg install git gcc-g++
endif
	rm -fr myHE
	git clone https://github.com/qdm12/myHE.git
	cd myHE && g++ main.cpp ../HELib/src/fhe.a -I ../HELib/src -o ../HEapp -L/usr/local/lib -lntl -lgmp -lm
	./HEapp.exe

test : test.cpp
	g++ test.cpp -o test
	
clean :
	rm -f *.exe
	
deepclean :
	$(info Cleaning up everything !)
	cd gmp-$(GMP_V) && make uninstall
	$(info ...Uninstalled GMP)
	cd ntl-$(NTL_V)/src && make uninstall
	$(info ...Uninstalled NTL)
	rm -f /usr/x86_64-pc-cygwin/lib/libgmp.*
	$(info ...Uninstalled static GMP and NTL)
	rm -fr gmp-$(GMP_V) ntl-$(NTL_V) HElib
	$(info ...Removed GMP, NTL, HELib folder)
ifeq ($(shell uname -o),Cygwin)
	apt-cyg remove wget curl perl m4 git
	$(info ...Removed wget curl perl m4 git from CYGWIN)
endif

help : 
	@echo Available commands are:
	@echo make HElib - Downloads HElib and other libraries and installs those
	@echo make HEapp - Downloads draft c++ file from Github, compiles it and runs it.
	@echo make test - Simply builds test from the file test.cpp, without any inclusion.
	@echo make clean - Removes all executables .exe
	@echo make deepclean - Removes all the libraries and packages installed. BE CAUTIOUS!