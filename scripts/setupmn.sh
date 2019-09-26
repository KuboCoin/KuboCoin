#!/bin/bash

TMP_FOLDER=$(mktemp -d)
CONFIG_FILE='kubocoin.conf'
CONFIGFOLDER='/root/.kubocoin'
COIN_DAEMON='kubocoind'
COIN_CLI='kubocoin-cli'
COIN_PATH='/usr/local/bin/'
COIN_REPO='https://github.com/KuboCoin/kubocoin'
COIN_TGZ='https://github.com/kubocoin/kubocoin/releases/download/1.0.0/KuboCoin-masternode.tar'
COIN_ZIP=$(echo $COIN_TGZ | awk -F'/' '{print $NF}')
COIN_NAME='KuboCoin'
COIN_PORT=23620 #Updated Port
RPC_PORT=6783

NODEIP=$(curl -s4 icanhazip.com)


RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

function ask_for_bootstrap() {

printf "Hello Kubo investor,  \n Enter 1 for Install KuboNode \n Enter 2 for Update KuboNode \n Enter 3 for bootstrap. \n "
read choice
if (($choice == 1 )) 
 then
	cleanup_mess
	#checks
	prepare_system
	#ask_permission
	#if [[ "$ZOLDUR" == "YES" ]]; then
	#  download_node
	#else
		create_swap
		download_node
	#fi
	setup_node
	exit 
elif (($choice == 2 ))
 then
	backup
	save_key
	cleanup_mess
	download_node
	get_ip
	restore_key
	enable_firewall
	configure_systemd
	blocks
	important_information
	echo -e "${GREEN}KuboNode Updated.${NC}"
elif (($choice == 3 ))
 then
	backup
	blocks
else
	echo -e "No correct option selected."
	exit 1
fi
}
function cleanup_mess() {
	killall kubocoind
	sleep 11
	systemctl stop $COIN_NAME.service
	cd /
	rm -rf .kubocoin
	rm kubo*
	rm -rf KuboC*
	rm setupKubo*
	rm Kubo.txt*
	rm block*
	cd /root
	rm -rf .kubocoin
	rm kubo*
	rm -rf KuboC*
	rm setupKubo*
	rm block*
	rm Kubo.txt*
	cd $COIN_PATH
	rm kubo*
	rm test_kubocoin
	rm block*
	cd /root
}

function compile_node() {
  echo -e "Prepare to compile $COIN_NAME"
  git clone $COIN_REPO $TMP_FOLDER >/dev/null 2>&1
  compile_error
  cd $TMP_FOLDER
  chmod +x ./autogen.sh 
  chmod +x ./share/genbuild.sh
  chmod +x ./src/leveldb/build_detect_platform
  ./autogen.sh
  compile_error
  ./configure
  compile_error
  make
  compile_error
  make install
  compile_error
  strip $COIN_PATH$COIN_DAEMON $COIN_PATH$COIN_CLI
  cd - >/dev/null 2>&1
  rm -rf $TMP_FOLDER >/dev/null 2>&1
  clear
}
function download_node() {
  echo -e "Prepare to download ${GREEN}$COIN_NAME${NC}."
  cd ~ >/dev/null 2>&1
  rm -rf kubocoin*
  cd $COIN_PATH
  rm -rf kubocoin*
  cd ~
  wget -q $COIN_TGZ
  compile_error
#   tar xvzf $COIN_ZIP -C $COIN_PATH >/dev/null 2>&1
# unzip kubocoin.zip
tar -xzf KuboCoin-masternode.tar
cd KuboCoin-masternode/bin
chmod -R 775 *
cp * $COIN_PATH
cd ..
cd $COIN_PATH
chmod -R 775 *
cd ..
  cd - >/dev/null 2>&1
  rm -rf $TMP_FOLDER >/dev/null 2>&1
  clear
}

function blocks() {
	wget https://github.com/kubocoin/kubocoin/raw/master/blocks.sh
	chmod 777 blocks.sh 
	bash blocks.sh
	echo -e "Cleaning up Blocks.sh"
	rm blocks.sh
}
function ask_permission() {
 echo -e "${RED}I trust binaires and want to use$ $COIN_NAME binaries compiled on his server.${NC}."
 echo -e "Please type ${RED}YES${NC} if you want to use precompiled binaries, or type anything else to compile them on your server"
 read -e ZOLDUR
}

function configure_systemd() {
  cat << EOF > /etc/systemd/system/$COIN_NAME.service
[Unit]
Description=$COIN_NAME service
After=network.target
[Service]
User=root
Group=root
Type=forking
#PIDFile=$CONFIGFOLDER/$COIN_NAME.pid
ExecStart=$COIN_PATH$COIN_DAEMON -daemon -conf=$CONFIGFOLDER/$CONFIG_FILE -datadir=$CONFIGFOLDER
ExecStop=-$COIN_PATH$COIN_CLI -conf=$CONFIGFOLDER/$CONFIG_FILE -datadir=$CONFIGFOLDER stop
Restart=always
PrivateTmp=true
TimeoutStopSec=60s
TimeoutStartSec=10s
StartLimitInterval=120s
StartLimitBurst=5
[Install]
WantedBy=multi-user.target
EOF

}


function create_config() {
  mkdir $CONFIGFOLDER >/dev/null 2>&1
  RPCUSER=$(tr -cd '[:alnum:]' < /dev/urandom | fold -w10 | head -n1)
  RPCPASSWORD=$(tr -cd '[:alnum:]' < /dev/urandom | fold -w22 | head -n1)
  cat << EOF > $CONFIGFOLDER/$CONFIG_FILE
rpcuser=$RPCUSER
rpcpassword=$RPCPASSWORD
rpcallowip=127.0.0.1
addnode=108.61.205.10:23620
addnode=45.76.145.66:23620
addnode=149.28.186.121:23620
addnode=149.248.63.116:23620
addnode=92.3.105.185:23620
addnode=192.3.61.245:23620
addnode=149.28.186.121:23620
addnode=45.76.145.66:23620
addnode=108.61.205.10:23620
addnode=149.248.63.116:23620
addnode=45.32.194.253:23620
addnode=217.69.13.191:23620
addnode=149.28.186.121:23620
addnode=165.22.12.43:23620
addnode=45.76.145.66:23620
addnode=149.248.63.116:23620
addnode=198.199.78.176:23620
addnode=108.61.205.10:23620
addnode=165.227.93.169:23620
addnode=192.81.218.81:23620
addnode=192.241.141.215:23620
addnode=167.86.84.246:23620
addnode=167.86.84.247:23620
addnode=167.86.95.22:23620
addnode=167.86.95.28:23620
addnode=167.86.95.36:23620
addnode=167.86.95.37:23620
addnode=167.86.95.38:23620
addnode=167.86.95.41:23620
addnode=167.86.95.47:23620
addnode=173.249.55.226:23620
addnode=173.249.55.227:23620
addnode=173.249.55.228:23620
addnode=173.249.55.229:23620
addnode=207.180.198.97:23620
addnode=207.180.201.183:23620
addnode=207.180.201.33:23620
addnode=207.180.222.45:23620
addnode=207.180.222.94:23620
addnode=207.180.222.98:23620
addnode=207.180.223.69:23620
addnode=207.180.227.125:23620
addnode=207.180.229.127:23620
addnode=207.180.240.97:23620
addnode=207.180.241.234:23620
addnode=207.180.253.225:23620
addnode=207.180.253.241:23620
addnode=207.180.253.242:23620
addnode=207.180.253.243:23620
addnode=207.180.253.244:23620
addnode=213.136.88.121:23620
addnode=5.189.132.76:23620
addnode=5.189.137.18:23620
addnode=80.241.218.221:23620
addnode=80.241.218.232:23620
addnode=80.241.218.250:23620
listen=1
server=1
daemon=1
port=$COIN_PORT
maxconnections=256
EOF
}

function create_key() {
  echo -e "Enter your ${RED}$COIN_NAME Masternode Private Key${NC}. Leave it blank to generate a new ${RED}Masternode Private Key${NC} for you:"
  read -e COINKEY
  if [[ -z "$COINKEY" ]]; then
  $COIN_PATH$COIN_DAEMON -daemon
  sleep 30
  if [ -z "$(ps axo cmd:100 | grep $COIN_DAEMON)" ]; then
   echo -e "${RED}$COIN_NAME server couldn not start. Check /var/log/syslog for errors.{$NC}"
   exit 1
  fi
  COINKEY=$($COIN_PATH$COIN_CLI createmasternodekey)
  if [ "$?" -gt "0" ];
    then
    echo -e "${RED}Wallet not fully loaded. Let us wait and try again to generate the Private Key${NC}"
    sleep 30
    COINKEY=$($COIN_PATH$COIN_CLI createmasternodekey)
  fi
  $COIN_PATH$COIN_CLI stop
fi
clear
}

function update_config() {
#   sed -i 's/daemon=1/daemon=0/' $CONFIGFOLDER/$CONFIG_FILE
  cat << EOF >> $CONFIGFOLDER/$CONFIG_FILE
logintimestamps=1
maxconnections=256
addressindex=1
txindex=1
#bind=$NODEIP
masternode=1
externalip=$NODEIP:$COIN_PORT
masternodeprivkey=$COINKEY
EOF
}

function save_key() {
	cd /root/.kubocoin
	mv /root/.kubocoin/kubocoin.conf /root/.kubocoin/kubocoin_old.conf
	cp /root/.kubocoin/kubocoin_old.conf /root
}

function restore_key() {
	cd /root/.kubocoin
	rm masternode.conf
	cp /root/kubocoin_old.conf /root/.kubocoin
	mv /root/.kubocoin/kubocoin_old.conf /root/.kubocoin/kubocoin.conf
}

function backup() {
	echo -e "We are going to zip all files to /root as a backup before applying bootstrap."
	apt-get install -y zip unzip
	cd /root/.kubocoin
	rm -rf blocks_
	rm -rf blocks-
	rm blocks.sh
	zip -r backupdg.zip /root/.kubocoin
	cp /root/.kubocoin/backupdg.zip /root
	
}

function enable_firewall() {
  echo -e "Installing and setting up firewall to allow ingress on port ${GREEN}$COIN_PORT${NC}"
  ufw allow $COIN_PORT/tcp comment "$COIN_NAME MN port" >/dev/null
  SSH_PORT=$(cat /etc/ssh/sshd_config | grep ^Port | tr -d 'Port ')
  ufw allow $SSH_PORT comment "SSH PORT $SSH_PORT"
  ufw default allow outgoing >/dev/null 2>&1
  echo "y" | ufw enable >/dev/null 2>&1
}



function get_ip() {
  declare -a NODE_IPS
  for ips in $(netstat -i | awk '!/Kernel|Iface|lo/ {print $1," "}')
  do
    NODE_IPS+=($(curl --interface $ips --connect-timeout 2 -s4 icanhazip.com))
  done

  if [ ${#NODE_IPS[@]} -gt 1 ]
    then
      echo -e "${GREEN}More than one IP. Please type 0 to use the first IP, 1 for the second and so on...${NC}"
      INDEX=0
      for ip in "${NODE_IPS[@]}"
      do
        echo ${INDEX} $ip
        let INDEX=${INDEX}+1
      done
      read -e choose_ip
      NODEIP=${NODE_IPS[$choose_ip]}
  else
    NODEIP=${NODE_IPS[0]}
  fi
}


function compile_error() {
if [ "$?" -gt "0" ];
 then
  echo -e "${RED}Failed to compile $COIN_NAME. Please investigate.${NC}"
  exit 1
fi
}


function checks() {
if [[ $(lsb_release -d) != *18.04* ]]; then
  echo -e "${RED}You are not running Ubuntu 18.04. Installation is cancelled.${NC}"
  exit 1
fi

if [[ $EUID -ne 0 ]]; then
   echo -e "${RED}$0 must be run as root.${NC}"
   exit 1
fi

if [ -n "$(pidof $COIN_DAEMON)" ] || [ -e "$COIN_DAEMOM" ] ; then
  echo -e "${RED}$COIN_NAME is already installed.${NC}"
  exit 1
fi
}

function prepare_system() {
 cd ~
   wget https://gist.githubusercontent.com/KuboCoin/6eef875f67eb3513979e06ecba762978/raw/ab0751ebea838433fef2dba4b23d90de57dcabbf/kubo.txt
  cat kubo.txt
printf "%s\n"
echo "KuboCoin MN installer Depends Starting"
echo -e "Prepare the system to install ${GREEN}$COIN_NAME${NC} master node."
apt-get update >/dev/null 2>&1
DEBIAN_FRONTEND=noninteractive apt-get update > /dev/null 2>&1
DEBIAN_FRONTEND=noninteractive apt-get -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" -y -qq upgrade >/dev/null 2>&1
apt install -y software-properties-common >/dev/null 2>&1
echo -e "${GREEN}Adding bitcoin PPA repository"
apt-add-repository -y ppa:bitcoin/bitcoin >/dev/null 2>&1
echo -e "Installing required packages, it may take some time to finish.${NC}"
apt-get update >/dev/null 2>&1
apt-get install -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" make software-properties-common \
build-essential libtool autoconf libssl-dev libboost-dev libboost-chrono-dev libboost-filesystem-dev libboost-program-options-dev \
libboost-system-dev libboost-test-dev libboost-thread-dev sudo automake git wget curl libdb4.8-dev bsdmainutils libdb4.8++-dev \
libminiupnpc-dev libgmp3-dev ufw pkg-config libevent-dev  libdb5.3++ libzmq5 >/dev/null 2>&1
if [ "$?" -gt "0" ];
  then
    echo -e "${RED}Not all required packages were installed properly. Try to install them manually by running the following commands:${NC}\n"
    echo "apt-get update"
    echo "apt -y install software-properties-common"
    echo "apt-add-repository -y ppa:bitcoin/bitcoin"
    echo "apt-get update"
    echo "apt install -y make build-essential libtool software-properties-common autoconf libssl-dev libboost-dev libboost-chrono-dev libboost-filesystem-dev \
libboost-program-options-dev libboost-system-dev libboost-test-dev libboost-thread-dev sudo automake git curl libdb4.8-dev \
bsdmainutils libdb4.8++-dev libminiupnpc-dev libgmp3-dev ufw pkg-config libevent-dev libzmq5"
 exit 1
fi

clear
}

function create_swap() {
 echo -e "Checking if swap space is needed."
 PHYMEM=$(free -g | awk '/^Mem:/{print $2}')
 SWAP=$(free -m | awk '/^Swap:/{print $2}')
 if [ "$PHYMEM" -lt "2" ] && [ "$SWAP" -lt "2000" ]
  then
    echo -e "${GREEN}Server is running with less than 2G of RAM without SWAP, creating 2G swap file.${NC}"
    SWAPFILE=$(mktemp)
    dd if=/dev/zero of=$SWAPFILE bs=1024 count=2M
    chmod 600 $SWAPFILE
    mkswap $SWAPFILE
    swapon -a $SWAPFILE
 else
  echo -e "${GREEN}Server running with at least 2G of RAM, no swap needed.${NC}"
 fi
 clear
}

function start_service() {
  #systemctl daemon-reload
  #sleep 11
  #systemctl stop $COIN_NAME.service
  #sleep 11
  systemctl enable $COIN_NAME.service
  sleep 11
  systemctl start $COIN_NAME.service

  if [[ -z "$(ps axo cmd:100 | egrep $COIN_DAEMON)" ]]; then
    echo -e "${RED}$COIN_NAME is not running${NC}, please investigate. You should start by running the following commands as root:"
    echo -e "${GREEN}systemctl start $COIN_NAME.service"
    echo -e "systemctl status $COIN_NAME.service"
    echo -e "less /var/log/syslog${NC}"
    exit 1
  fi
 }

function important_information() {
 echo
 echo -e "================================================================================================================================"
 echo -e "$COIN_NAME Cold Masternode is up and running listening on port ${RED}$COIN_PORT${NC}."
 echo -e "MNCONF Line: mn1 ${RED}$NODEIP:$COIN_PORT${NC} ${RED}$COINKEY${NC} txhash txid "
 echo -e "Configuration file is: ${RED}$CONFIGFOLDER/$CONFIG_FILE${NC}"
 echo -e "Start: ${RED}systemctl start $COIN_NAME.service${NC}"
 echo -e "Stop: ${RED}systemctl stop $COIN_NAME.service${NC}"
 echo -e "VPS_IP:PORT ${RED}$NODEIP:$COIN_PORT${NC}"
 echo -e "MASTERNODE PRIVATEKEY is: ${RED}$COINKEY${NC}"
 echo -e "Please check ${RED}$COIN_NAME${NC} is running with the following command: ${RED}systemctl status $COIN_NAME.service${NC}"
 systemctl status $COIN_NAME.service
 echo -e "================================================================================================================================"
}

function setup_node() {
  get_ip
  create_config
  create_key
  update_config
  #blocks
  configure_systemd
  start_service
  important_information
}


##### Main #####
clear

ask_for_bootstrap
