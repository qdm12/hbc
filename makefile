#Requires make for cygwin and an internet connection
GMP_V = 6.1.0
NTL_V = 9.6.2

ini :
ifeq ($(shell uname -o),Cygwin)
	lynx -source rawgit.com/transcode-open/apt-cyg/master/apt-cyg > apt-cyg
	install apt-cyg /bin
	rm -f apt-cyg
	apt-cyg install wget
	apt-cyg install curl
endif

gmp : ini
	apt-cyg install m4 perl
	curl https://gmplib.org/download/gmp/gmp-$(GMP_V).tar.bz2 > gmp-$(GMP_V).tar.bz2
	tar xjf gmp-$(GMP_V).tar.bz2
	rm -f gmp-$(GMP_V).tar.bz2
	cd gmp-$(GMP_V) && ./configure
	cd gmp-$(GMP_V) && make
	cd gmp-$(GMP_V) && make install
	cd gmp-$(GMP_V) && make check

ntl : ini
	curl http://www.shoup.net/ntl/ntl-$(NTL_V).tar.gz > ntl-$(NTL_V).tar.gz
	tar xf ntl-$(NTL_V).tar.gz
	rm -f ntl-$(NTL_V).tar.gz
	cd ntl-$(NTL_V)/src && ./configure NTL_GMP_LIP=on
	cd ntl-$(NTL_V)/src && make
	cd ntl-$(NTL_V)/src && make install
	
HElib : ntl gmp
	git clone https://github.com/shaih/HElib.git
	sed -i -- 's/_B/_B_/g' HElib/src/Test_Replicate.cpp
	cd HElib\src && make
	cd HElib\src && make check
	
clean : 
	rm -fr gmp-$(GMP_V)
	rm -fr ntl-$(NTL_V)
	rm -fr HElib
	apt-cyg remove wget curl perl m4
	
	
all : HElib