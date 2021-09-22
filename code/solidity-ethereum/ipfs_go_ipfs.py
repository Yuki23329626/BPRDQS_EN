import re
import time
import ipfshttpclient

client = ipfshttpclient.connect()

# upload 0.5KB file

time_start = time.time()  # start timing

res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_0.5KB.txt')

time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
print('\nupload 0.5KB time cost:', time_diff, 's\n')

print(res)
hash = res['Hash']
# print("hash:",hash)

# download 0.5KB file

time_start = time.time()  # start timing

client.get(res['Hash'])

time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
print('\ndownload 0.5KB time cost:', time_diff, 's\n')
# print(res)

# upload 1MB file

time_start = time.time()  # start timing

res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_1MB.txt')
# print(res['Hash'])

time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
print('\nupload 1MB time cost:', time_diff, 's\n')

print(res)
hash = res['Hash']
# print("hash:",hash)

# download 1MB file

time_start = time.time()  # start timing

client.get(res['Hash'])

time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
print('\ndownload 1MB time cost:', time_diff, 's\n')