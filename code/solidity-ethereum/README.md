# About the writing and execution of smart contract

Editing date: 2021/06/27
Editor: Shen Nongxiang
Mailbox: g08410117@ccu.edu.tw

## 1. the environment

1. Environment

Ethereum Remix IDE: [URL](https://remix.ethereum.org/)

## 2. execution

1. Place "BPREET.sol", "BPRDQS.sol", "BN256G2.sol", "BN256G1.sol" under the folder on the Remix IDE
-Among them, "BN256G2.sol" and "BN256G1.sol" are the G1 and G2 solidity libraries implemented on the elliptic curve BN256
-"BPRDQS.sol" is the smart contract under the scheme implemented by us, which includes equality test
-"BPREET.sol" is the smart contract under the scheme we want to compare, which includes operations such as equality test and re-encryption

2. Open the smart contract you want to subordinate on the Remix IDE, and compile it first, as shown in the figure below

![](image/solidity_compile.png)

3. After the compilation is complete, if there is no problem, you can perform subordinate actions

![](image/solidity_deploy.png)

Remark:
-If the amount of calculation is too large, the browser page will crash directly
-If the GAS LIMIT is insufficient, you can try to increase it

4. After the subordinates are completed, as shown in the figure below, click the "testOurs" button in the figure to send the transaction

![](image/solidity_test.png)

This button corresponds to the "testOURS" function in the smart contract
The parameters used by this function come from the uint256[12] data_ours variable in solidity
> The variable content is a pre-filled value. For the value generation method, please refer to [README.md](../golang-ethereum/README.md) under the "golang-ethereum/" folder
> Why do you need to pre-fill? The reason is that the Remix IDE can’t eat too long input. It is a compromise to use pre-filling for testing.

5. Confirm the transaction result, as shown below

![](image/solidity_result.png)

The red box in the figure is the result of equality test
In addition, the consumption caused by calculation (execution cost) is 164557 gas

## 3. reference materials

1. Elliptic curve BN256 library: [https://github.com/witnet/bls-solidity](https://github.com/witnet/bls-solidity)
2. Pre-compiled smart contract related information: [https://eips.ethereum.org/](https://eips.ethereum.org/)

## 4. Practice commands

1. Create file with specific size on Windows  

- [Download DummyFileCreator](https://www.mynikko.com/dummy/)

2. get file context on IPFS

```cmd
curl -X POST -Uri "1yJWtiH93lpPRSy9YsrkB8YLvmj:9ee3953481ca0c61c8c5fd3865816420" "https://ipfs.infura.io:5001/api/v0/cat?arg=QmYrF9yXAXPUKkTEcpqcoZHNc7vM96KJeRMh82AXqDxbaA"
```

3. specific version of go-ipfs and ipfshttpclient

there have some issues about version compatible between ipfshttpclient(ipfsapi) and go-ipfs
now I both use the v0.8.0 version

- git:  
```bash
git clone --depth 1 --branch v0.8.0 https://github.com/ipfs/go-ipfs.git
```
- pip:  
```bash
pip install ipfshttpclient==0.8.0
```
- go-ipfs:  
[download go-ipfs-zip](https://dist.ipfs.io/go-ipfs/v0.8.0)