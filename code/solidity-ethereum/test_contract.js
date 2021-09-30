fs = require('fs');
const Web3 = require('web3');
const HDWalletProvider = require("truffle-hdwallet-provider");

// account information
const mnemonic = "method banner ordinary buyer series either initial drink exchange rifle little lunch";
defaultAccount = '0x382e347f834E1C897ec965eB1Af2b673900257d4';
private_keys = '2c22045da1809eb6bb6cb0d3130854f90712b7d3ffe6e9d59a22964cf422bc15';

const web3 = new Web3(new HDWalletProvider(mnemonic, 'https://rinkeby.infura.io/v3/3c8c82c3951c4cf3b61586b0202981c1'));

var samplecontractContract = new web3.eth.Contract([{ "constant": true, "inputs": [], "name": "testFunc", "outputs": [{ "name": "", "type": "int256" }], "payable": false, "stateMutability": "pure", "type": "function" }]);

web3.eth.accounts.privateKeyToAccount(private_keys);

web3.eth.getAccounts().then(e => {
  firstAccount = e[0];
  console.log("firstAccount: " + firstAccount);
  var samplecontract = samplecontractContract.deploy(
    {
      from: firstAccount,
      data: '0x6080604052348015600f57600080fd5b50607e8061001e6000396000f3fe6080604052348015600f57600080fd5b506004361060285760003560e01c8063037a417c14602d575b600080fd5b60336049565b6040518082815260200191505060405180910390f35b6000600190509056fea165627a7a72305820e710d7394e9965c17ead6bb53757a23caee28d75a0a02b483638015a49dac6070029',
      gas: '4700000'
    }, function (e, contract) {
      console.log(e, contract);
      if (typeof contract.address !== 'undefined') {
        console.log('Contract mined! address: ' + contract.address + ' transactionHash: ' + contract.transactionHash);
      }
    });
});

