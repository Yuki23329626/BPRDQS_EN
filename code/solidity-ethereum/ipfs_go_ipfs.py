import re
import time
import ipfshttpclient
import numpy as np
import matplotlib.pyplot as plt


client = ipfshttpclient.connect()

upload_05k = 0
upload_1m = 0
upload_5m = 0
upload_10m = 0
upload_15m = 0

download_05k = 0
download_1m = 0
download_5m = 0
download_10m = 0
download_15m = 0


# 0.5KB file

time_start = time.time()  # start timing
res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_0.5KB.txt')
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
upload_05k = time_diff
hash = res['Hash']

time_start = time.time()  # start timing
client.get(res['Hash'])
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
download_05k = time_diff

# 1MB file

time_start = time.time()  # start timing
res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_1MB.txt')
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
upload_1m = time_diff
hash = res['Hash']

time_start = time.time()  # start timing
client.get(res['Hash'])
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
download_1m = time_diff

# 5M file

time_start = time.time()  # start timing
res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_5MB.txt')
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
upload_5m = time_diff
hash = res['Hash']

time_start = time.time()  # start timing
client.get(res['Hash'])
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
download_5m = time_diff

# 10M file

time_start = time.time()  # start timing
res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_10MB.txt')
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
upload_10m = time_diff
hash = res['Hash']

time_start = time.time()  # start timing
client.get(res['Hash'])
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
download_10m = time_diff

# 15MB file

time_start = time.time()  # start timing
res = client.add(r'D:\nxshen\github\BPRDQS_EN\code\solidity-ethereum\dummyFile_15MB.txt')
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
upload_15m = time_diff
hash = res['Hash']

time_start = time.time()  # start timing
client.get(res['Hash'])
time_end = time.time()  # end timing
time_diff = time_end - time_start   # time spent
download_15m = time_diff

file_size = ['0.1KB', '1MB', '5MB', '10MB', '15MB']
upload_time = [upload_05k, upload_1m, upload_5m, upload_10m, upload_15m]
downlaod_time = [download_05k, download_1m, download_5m, download_10m, download_15m]
x = np.arange(len(file_size))
width = 0.3
plt.bar(x, upload_time, width, color='green', label='upload')
plt.bar(x + width, downlaod_time, width, color='blue', label='download')
plt.xticks(x + width / 2, file_size)
plt.ylabel('time(s)')
plt.title('Time spent of file transmission through IPFS')
plt.legend(bbox_to_anchor=(1,1), loc='upper left')
plt.show()