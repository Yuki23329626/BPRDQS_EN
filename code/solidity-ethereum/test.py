from time import time, sleep

i = 1
while True:
    sleep(60 - time() % 60)
    print(i)
    i = i + 1