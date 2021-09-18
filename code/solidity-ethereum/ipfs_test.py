from web3 import Web3
w3 = Web3(Web3.HTTPProvider('https://rinkeby.infura.io/v3/3c8c82c3951c4cf3b61586b0202981c1'))

print(w3.isConnected())

w3.eth.get_block('latest')

latest_block = web3.eth.get_block('latest')

print(latest_block)