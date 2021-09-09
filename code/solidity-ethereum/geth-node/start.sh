#!./bin/bash

echo "======== account ========"
privatekey="1fb08249ee755bc4ba0c30529431fe3caf4da2fc9e9abcf2574e76248c8a9f82"
address="f9BCe4e8708445f6113C620836925f8aEaFFC0B9"

echo "======== check account exist ========"
isInFile=$(echo `geth --rinkeby account list --datadir=$datadir | grep -c $address`)
if [ $isInFile -eq 0 ]; then
    echo "======== acoount no exist! Starting import! ========"
    echo "" > ~/.accountpassword
    echo $privatekey > ~/.privatekey
    geth --rinkeby account import --datadir=$datadir --password ~/.accountpassword  ~/.privatekey
else
   echo "======== account exist ========"
fi

# geth --rinkeby --unlock 0xf9BCe4e8708445f6113C620836925f8aEaFFC0B9 --rpc --rpcaddr "0.0.0.0" --rpccorsdomain "*" --rpcvhosts=* --password ~/.accountpassword --etherbase $address 2> /root/geth.log
geth --rinkeby --http --http.api personal,eth,net,web3 --password ~/.accountpassword --etherbase $address  2> /root/geth.log