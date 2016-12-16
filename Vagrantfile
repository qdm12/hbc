Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"
  #config.vm.network "forwarded_port", guest: 5000, host: 5000
  config.vm.provider "virtualbox" do |vb|
    #vb.gui = true
    vb.memory = "2048"
    vb.cpus = 1
  end
  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y git g++ m4 perl libboost-all-dev
    sudo apt-get -y autoremove
    #build-essential
      
    #Installing GMP
    mkdir temp
    cd temp
    GMP_V=6.1.0
    wget https://gmplib.org/download/gmp/gmp-$GMP_V.tar.bz2
	tar -xjf gmp-$GMP_V.tar.bz2
	cd gmp-$GMP_V
    ./configure
	make
	make install
    cd ..
    rm -fr temp
    unset GMP_V
    
    #Installing NTL
    mkdir temp
    cd temp
    NTL_V=9.9.1
    wget http://www.shoup.net/ntl/ntl-$NTL_V.tar.gz
	tar -xvzf ntl-$NTL_V.tar.gz
	cd ntl-$NTL_V/src
    ./configure NTL_GMP_LIP=on
	make
	make install
    cd ../..
    rm -fr temp
    unset NTL_V
    
    #Installing HElib
    cd /vagrant
    if [ ! -d "HElib/src" ]; then
        rm -fr HElib
        git clone https://github.com/shaih/HElib.git
        cd HElib/src
        make
        make check
        make test
        cd ../..
    fi
   
    #Building hbc
    make HEapp
    
    echo "colorscheme desert" > ~/.vimrc
  SHELL
  config.vm.provision "file", source: "~/.gitconfig", destination: "~/.gitconfig"
end

