require('dotenv').config();
const Web3 = require('web3');
var HDWalletProvider = require("truffle-hdwallet-provider");
var mnemonic = process.env["NEMONIC"];
var tokenKey = process.env["ENDPOINT_KEY"];
var accountAddress = '0x382e347f834E1C897ec965eB1Af2b673900257d4';

const web3 = new Web3(new HDWalletProvider(mnemonic, 'https://rinkeby.infura.io/v3/' + tokenKey));

async function testOURS(deployedContractAbi, deployedContractAddress) {
  const contractInstance = new web3.eth.Contract(deployedContractAbi, deployedContractAddress);
  await contractInstance.methods.testOURS().call({
    from: accountAddress,
    gas: '2697681'
  }).then(console.log)
  await contractInstance.methods.testOURS().send({
    from: accountAddress
    // }).on('transactionHash', async function (txHash) {
    //   console.log('txHash:' + txHash);
    //   await web3.eth.getTransaction(txHash).then(console.log);
  }).on('receipt', function (receipt) {
    console.log('BPRDOS.testOURS:');
    console.log(receipt)
  })
};

async function testBPREET1(deployedContractAbi, deployedContractAddress) {
  const contractInstance = new web3.eth.Contract(deployedContractAbi, deployedContractAddress);
  await contractInstance.methods.testBPREET1().call({
    from: accountAddress,
    gas: '2697681'
  }).then(console.log)
  await contractInstance.methods.testBPREET1().send({
    from: accountAddress
    // }).on('transactionHash', async function (txHash) {
    //   console.log('txHash:' + txHash);
    //   await web3.eth.getTransaction(txHash).then(console.log);
  }).on('receipt', function (receipt) {
    console.log('BPREET.testBPREET1:');
    console.log(receipt)
  })
};

BPRDOS_ADDRESS = '0x4410c9529e9a8a9343436Fd5d4ce0c72131538e7';
BPRDOS_ABI = [
  {
    "inputs": [],
    "name": "testOURS",
    "outputs": [
      {
        "internalType": "bool",
        "name": "",
        "type": "bool"
      }
    ],
    "stateMutability": "nonpayable",
    "type": "function"
  }
];

BPREET_ADDRESS = '0x9e457520A893e10282513313556d7Cbc90F63aCC';
BPREET_ABI = [
  {
    "inputs": [],
    "name": "testBPREET1",
    "outputs": [
      {
        "internalType": "bool",
        "name": "",
        "type": "bool"
      }
    ],
    "stateMutability": "nonpayable",
    "type": "function"
  },
  {
    "inputs": [
      {
        "internalType": "uint8[1024]",
        "name": "a",
        "type": "uint8[1024]"
      },
      {
        "internalType": "uint8[1024]",
        "name": "b",
        "type": "uint8[1024]"
      }
    ],
    "name": "xor",
    "outputs": [
      {
        "internalType": "uint8[1024]",
        "name": "r",
        "type": "uint8[1024]"
      }
    ],
    "stateMutability": "nonpayable",
    "type": "function"
  }
];

console.log('BPRDOS:');
testOURS(BPRDOS_ABI, BPRDOS_ADDRESS);

console.log('BPREET:');
testBPREET1(BPREET_ABI, BPREET_ADDRESS);

