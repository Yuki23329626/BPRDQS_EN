import time
import requests
import os
import json

project_id = '1yJWtiH93lpPRSy9YsrkB8YLvmj'
project_secret = '9ee3953481ca0c61c8c5fd3865816420'

isUpload = True

if(isUpload):
    time_start = time.time()  # start timing

    filePath = r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_0.5KB.txt'
    if not os.path.exists(filePath):
        print('Wrong path')
        exit(1)

    with open(filePath, 'r') as f:
        print(f.read())

    files = {
        'file': filePath
    }
    response = requests.post('https://ipfs.infura.io:5001/api/v0/add', files=files, auth=(project_id, project_secret))
    print(response.text)

    time_end = time.time()  # end timing

    time_c = time_end - time_start   # time spent

    print('upload 0.5KB time cost:', time_c, 's')

    print('\n==== Getting context of json ====\n')

    response_json = json.loads(response.text)

    params = (
        ('arg', response_json['Hash']),
    )
    response = requests.post('https://ipfs.infura.io:5001/api/v0/cat',
                            params=params, auth=(project_id, project_secret))
    print(response.text)
