from web3 import Web3
from web3.middleware import geth_poa_middleware

web3 = Web3(Web3.HTTPProvider('https://rinkeby.infura.io/v3/3c8c82c3951c4cf3b61586b0202981c1'))

# necessary for Rinkey
web3.middleware_onion.inject(geth_poa_middleware, layer=0)

print(web3.clientVersion)

print(web3.eth.get_block('latest'))
