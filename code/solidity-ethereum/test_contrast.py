from web3 import Web3
from web3.middleware import geth_poa_middleware
from solcx import install_solc
# from solcx import compile_source
import solcx
import json

w3 = Web3(Web3.HTTPProvider(
    'https://rinkeby.infura.io/v3/3c8c82c3951c4cf3b61586b0202981c1'))

# necessary for Rinkey(PoA chain)
w3.middleware_onion.inject(geth_poa_middleware, layer=0)

# print(web3.clientVersion)
# print(web3.eth.get_block('latest'))

install_solc(version='v0.6.8')

# compile source code

compiled_sol = solcx.compile_files(
    [r"D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\BPRDQS.sol"]
)

bn256test_abi = compiled_sol["D:/nxshen/github/BPRDQS_EN/code/solidity-ethereum/BPRDQS.sol:bn256test"]['abi']

print(bn256test_abi)

with open('D:/nxshen/github/BPRDQS_EN/code/solidity-ethereum/compiled_sol.json', 'w') as f:
    json.dump(compiled_sol, f)

# contract_id, contract_interface = compiled_sol.popitem()

# print(contract_interface['abi'])

# def deploy_contract(w3, contract_interface):
#     tx_hash = w3.eth.contract(
#         abi=contract_interface['abi'],
#         bytecode=contract_interface['bin']).constructor().transact()

#     address = w3.eth.get_transaction_receipt(tx_hash)['contractAddress']
#     return address

# address = deploy_contract(w3, contract_interface)
# print(f'Deployed {contract_id} to: {address}\n')