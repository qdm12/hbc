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
  
  config.vm.provision :shell, :run => 'always', inline: <<-SHELL
    echo " ===== HBC ===== Updating packages..."
    sudo apt-get update
    sudo apt-get install -y git g++ m4 perl libboost-all-dev htop 
    sudo apt-get -y autoremove

    #Installing GMP
    if [ ! -f "/usr/local/lib/libgmp.a" ]; then
        GMP_V=6.1.2
        echo " ===== HBC ===== Installing GMP-$GMP_V..."
        wget https://gmplib.org/download/gmp/gmp-$GMP_V.tar.bz2
        tar -xvjf gmp-$GMP_V.tar.bz2
        cd gmp-$GMP_V
        ./configure
        make
        sudo make install
        make check
        cd ..
        rm -fr gmp-$GMP_V*
        unset GMP_V
    else
        echo " ===== HBC ===== GMP seems already installed. Skipping..."
    fi
    
    #Installing NTL
    if [ ! -f "/usr/local/lib/libntl.a" ]; then
        NTL_V=10.5.0
        echo " ===== HBC ===== Installing NTL-$NTL_V..."
        wget http://www.shoup.net/ntl/ntl-$NTL_V.tar.gz
        tar -xvzf ntl-$NTL_V.tar.gz
        cd ntl-$NTL_V/src
        ./configure NTL_GMP_LIP=on
        make
        sudo make install
        cd ../..
        rm -fr ntl-$NTL_V*
        unset NTL_V
    else
        echo " ===== HBC ===== NTL seems already installed. Skipping..."
    fi
    
    #Installing HElib
    cd /vagrant
    if [ ! -f "HElib/src/fhe.a" ]; then
        if [ ! -d "HElib" ]; then
            echo " ===== HBC ===== HElib not found; Downloading and building it..."
            #rm -fr HElib
            git clone https://github.com/shaih/HElib.git
        else
            echo " ===== HBC ===== HElib found but no compiled fhe.a found. Compiling it !"            
        fi
        cd HElib/src
        make
        #make check
        #1 of the check of HElib fails for some reason
        #make test
        cd ../..
    else
        echo " ===== HBC ===== HElib is present and is assumed to be compiled."
    fi
   
    #Building hbc
    make hbc
    echo " ===== HBC ===== hbc was successfully built ! Run it with: cd /vagrant && ./hbc"
  SHELL
end

