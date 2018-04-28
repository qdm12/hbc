FROM debian:stretch-slim
LABEL maintainer="quentin.mcgaw@gmail.com" \
      description="" \
      download="MB" \
      size="MB" \
      ram="Depends" \
      cpu_usage="Depends" \
      github="https://github.com/qdm12/hbc"
RUN GMP_V=6.1.2 && \
	NTL_V=10.5.0 && \
	apt-get update -qq > /dev/null && \
    apt-get install --no-install-recommends wget ca-certificates bzip2 make g++ m4 perl libboost-all-dev htop -qq > /dev/null && \
    wget -q https://gmplib.org/download/gmp/gmp-$GMP_V.tar.bz2 && \
	tar -xjf gmp-$GMP_V.tar.bz2 && \
	rm gmp-$GMP_V.tar.bz2 && \
	cd gmp-$GMP_V && \
    ./configure && \
    make && \
	make check && \
    make install && \
	cd / && \
	rm -r gmp-$GMP_V && \
	wget -q http://www.shoup.net/ntl/ntl-$NTL_V.tar.gz && \
	tar -xzf ntl-$NTL_V.tar.gz && \
	rm ntl-$NTL_V.tar.gz && \
	cd ntl-$NTL_V/src && \
	./configure NTL_GMP_LIP=on && \
	make && \
	make install && \
	cd / && \
	rm -r ntl-$NTL_V && \
	wget -q https://github.com/shaih/HElib/archive/master.tar.gz && \
	tar -xzf master.tar.gz && \
	rm master.tar.gz && \
	mkdir -p /hbc/HElib/src /hbc/src /hbc/temp && \
	mv HElib-master/src/* /hbc/HElib/src/ && \
	rm -r HElib-master && \
	cd /hbc/HElib/src && \
	make && \
	make check && \
	apt-get remove wget ca-certificates bzip2 -qq > /dev/null && \
	apt-get autoremove -qq > /dev/null && \ 
	rm -rf /var/lib/apt/lists/*
VOLUME /hbc/src
WORKDIR /hbc