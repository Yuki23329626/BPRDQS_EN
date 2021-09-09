#!/bin/bash
set -e
datadir="~/.ethereum/devchain"
#ip=$(echo `awk 'END{print $1}' /etc/hosts`)
ip=$(ping -c1 go-ethereum-signer1 | sed -nE 's/^PING[^(]+\(([^)]+)\).*/\1/p')

echo "======== geth ========"
echo "======== init ========"
geth --datadir=$datadir init "/root/genesis/poa_for_dev.json"

sleep 2
echo "======== bootnode ========"
#bootkey="fcc406a7344690f66c757cc2c9987e3e78bb01e33229f9877d48a7fecc2d6657"
#bootnode -nodekeyhex $bootkey -verbosity 4 &
#bootnodeid=$(bootnode --nodekeyhex=$bootkey -writeaddress) #node address: 256330933851d6d3c5f7326b01021553415a33cd5485e23bfbe35f6321e6e2f8373bb1c94933fdb3283a1a8b2b737587dd99c555029e65e173a3094daa39277c

#bootnode --genkey=boot.key
#bootnode --nodekey=boot.key -verbosity 4 &
#bootnode --nodekey=boot.key  -writeaddress > bootnodeid.txt
#bootnodeid=$(bootnode --nodekey=boot.key  -writeaddress)

sleep 2
echo "======== account ========"
privatekey="138cbbfb21686ddc3b5ffeb2cfc83491175af68319977acb81d0ae93392c626c"
address="e79d33e93bd888b35e055f1a12d876354729037b"

echo "======== check account exist ========"
isInFile=$(echo `geth account list --datadir=$datadir | grep -c $address`)
if [ $isInFile -eq 0 ]; then
    echo "======== acoount no exist! Starting import! ========"
    echo "" > ~/.accountpassword
    echo $privatekey > ~/.privatekey
    geth account import --datadir=$datadir --password ~/.accountpassword  ~/.privatekey
else
   echo "======== account exist ========"
fi

sleep 2
echo "======== mine ========"
nodeaddress="256330933851d6d3c5f7326b01021553415a33cd5485e23bfbe35f6321e6e2f8373bb1c94933fdb3283a1a8b2b737587dd99c555029e65e173a3094daa39277c"

#no use bootnode, fix nodekeyhex
#geth --datadir=$datadir --nodekeyhex "fcc406a7344690f66c757cc2c9987e3e78bb01e33229f9877d48a7fecc2d6657" --networkid 53809 --port 30303 --rpcapi "db,admin,debug,miner,personal,txpool,eth,net,web3" --rpc --rpcaddr "0.0.0.0" --rpccorsdomain "*" --rpcvhosts=* --password ~/.accountpassword --unlock $address --mine --minerthreads=1  2> /root/geth.log

#use bootnode
bootnodeId="enode://"$nodeaddress"@"$ip":30303"
#bootnodeId="enode://"$bootnodeid"@"$ip":30303"
until echo | nc -z -v go-ethereum-signer1 30303; do
    echo "Waiting go-ethereum-signer1 to start..."
    sleep 2
done
geth --datadir=$datadir --bootnodes $bootnodeId --networkid 11559 --port 30303 --rpcapi "admin,debug,miner,personal,txpool,eth,net,web3" --rpc --rpcaddr "0.0.0.0" --rpccorsdomain "*" --rpcvhosts=* --password ~/.accountpassword --etherbase $address  2> /root/geth.log
 