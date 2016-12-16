Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"
  #config.vm.network "forwarded_port", guest: 5000, host: 5000
  config.vm.provider "virtualbox" do |vb|
    #vb.gui = true
    vb.memory = "2600"
    #The average operation requires 6476MB of RAM for 3 bits!
    #Note that other operations require way less RAM, such as multiplication.
    vb.cpus = 2
    #You can set it to 3 or more to run multiple instances of hbc If you have enough RAM.
    vb.name = "hbc-virtual-machine"
  end
  config.vm.hostname = "hbc-hostname"
  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y git g++ m4 perl htop libboost-all-dev lzip
    sudo apt-get -y autoremove
      
    #Installing GMP
    GMP_V=6.1.2
    wget https://gmplib.org/download/gmp/gmp-$GMP_V.tar.lz
    tar --lzip -xvf gmp-$GMP_V.tar.lz
	cd gmp-$GMP_V
    ./configure
	make
	make install
    #make check
    rm -fr gmp-$GMP_V*
    unset GMP_V
    
    #Installing NTL
    NTL_V=9.9.1
    wget http://www.shoup.net/ntl/ntl-$NTL_V.tar.gz
	tar -xvzf ntl-$NTL_V.tar.gz
	cd ntl-$NTL_V/src
    ./configure NTL_GMP_LIP=on
	make
	make install
    cd ..
    rm -fr ntl-$NTL_V*
    unset NTL_V
    
    #Installing HElib
    cd /vagrant
    if [ ! -d "HElib/src" ]; then
        echo "HElib/src not found; Building it again..."
        #rm -fr HElib
        git clone https://github.com/shaih/HElib.git
        cd HElib/src
        make
        make check
        make test
        cd ../..
    fi
   
    #Building hbc
    make hbc
    echo "hbc was successfully built ! Run it with: cd /vagrant && ./hbc"
    echo "colorscheme desert" > ~/.vimrc
  SHELL
  config.vm.provision "file", source: "~/.gitconfig", destination: "~/.gitconfig"
end

