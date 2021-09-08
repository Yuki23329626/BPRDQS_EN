import csv
import sys
import shlex
import os
import glob
import numpy as np
import matplotlib.pyplot as plt
import shutil
import json
import pylab

# get each packet according to their uid to locate its source and destination
# in order to collect its info [From, To, TxTime, RxTime, Delay]
def get_packet(fin_dir):
    uid  = {}
    for filename in glob.glob(os.path.join(fin_dir, "log.*")):
        print("getting packet info in file:", filename)
        uid[filename] = {}
        data = open(filename, 'r')
        lines = data.readlines()
        for line in lines:
            string = line.split(" ")
            if string[0] == "TraceDelay" :
                if int(string[7]) not in uid[filename]:
                    uid[filename][int(string[7])] = ["", string[5], 0, 0, 0] # To Address
            if string[0] == "TraceDelay:":
                if int(string[10]) in uid[filename]:
                    uid[filename][int(string[10])][0] = string[5] # From Address
                    uid[filename][int(string[10])][2] = eval(string[12][:-2])/1000000000 # TX in sec
                    uid[filename][int(string[10])][3] = eval(string[14][:-2])/1000000000 # RX in sec
                    uid[filename][int(string[10])][4] = eval(string[16][:-3])/1000000000 # Delay in sec
        #break # uncomment this line if you want to do a quick test
    packet = {}
    for filename in uid.keys():
        for item in uid[filename]:
            #print("item:",item)
            #print(uid[filename][item])
            if uid[filename][item][0] == '':
                continue
            key = uid[filename][item][0] + " " + uid[filename][item][1]
            if key not in packet:
                packet[key] = []
                packet[key].append([uid[filename][item][2], uid[filename][item][3], uid[filename][item][4]])
            else:
                packet[key].append([uid[filename][item][2], uid[filename][item][3], uid[filename][item][4]])
    return packet

# 分析 log 檔來取得 handover 的時間點，我是取用 SDN enable 時的 handover 時間點，以此作為基準
def getHOTime(finDir):
    handover_time = {}
    list_handover_time = []
    files = glob.glob(os.path.join(finDir, "log.*"))
    print("files",files)
    with open(files[0], "r") as fin:
        lines = fin.readlines()
        for line in lines:
            string1 = line.split(" ")
            if string1[0] != "NotifyHandoverEndOkUe":
                continue
            if int(string1[5][:-1]) in handover_time:
                handover_time[int(string1[5][:-1])].append(float(string1[2][:6]))
            else:
                handover_time[int(string1[5][:-1])]=[float(string1[2][:6])]
        fin.close()
        list_handover_time = (sorted(handover_time.items(), key=lambda x:x[0]))
    #for item in list_handover_time:
        #print("IMSI:", item[0])
        #print("HO_time:", item[1])
    return list_handover_time # list_handover_time[IMSI] = handover_time

# 目前沒有使用的做法，參考用
def get_avg_delay_all(packet_dict):
    route_upload = 0
    route_download = 0

    upload_1k_delay_sum = 0
    upload_1m_delay_sum = 0

    download_1k_delay_sum = 0
    download_1m_delay_sum = 0

    for route in packet_dict:
        #print("route:", route)
        start = packet_dict[route][0][0]
        end = packet_dict[route][-1][1]
        num = len(packet_dict[route])
        #print("time spent:", (end-start))
        #print("packet received:", num)
        throughput_per_sec = num*1024/(end-start)
        #print("thoughput per sec:", throughput_per_sec)
        delay_1k = 1024/throughput_per_sec
        #print("1k byte packet delay:", delay_1k)
        delay_1m = 1024*1024/throughput_per_sec
        #print("1M byte packet delay", delay_1m)
        if route.split(" ")[0] == "1.0.0.2":
            route_download = route_download + 1
            download_1k_delay_sum = download_1k_delay_sum + delay_1k
            download_1m_delay_sum = download_1m_delay_sum + delay_1m
        else:
            route_upload = route_upload + 1
            upload_1k_delay_sum = upload_1k_delay_sum + delay_1k
            upload_1m_delay_sum = upload_1m_delay_sum + delay_1m
    
    print("average upload delay(1k byte):")
    if(route_upload>0):
        print(upload_1k_delay_sum/route_upload)
    print("average upload delay(1m byte):")
    if(route_upload>0):
        print(upload_1m_delay_sum/route_upload)
    print("average download delay(1k byte):")
    if(route_download>0):
        print(download_1k_delay_sum/route_download)
    print("average download delay(1m byte):")
    if(route_download>0):
        print(download_1m_delay_sum/route_download)

# 取得 handover 發生時的前後區間
def get_section(list_handover_time, packet):
    section = {}
    for imsi_hotimes in list_handover_time:
        for hotime in imsi_hotimes[1]:
            if (hotime < 55) and (hotime > 5):
                start_time = hotime 
                end_time = hotime + 5
                #print("hotime:", hotime)
                ue_address = "7.0.0." + str(imsi_hotimes[0]+1)
                upload_route = ue_address + " 1.0.0.2"
                download_route = "1.0.0.2 " + ue_address
                #print(upload_route)
                #print(download_route)
                #input()
                for item in packet[upload_route]:
                    if (item[0] >= start_time) and (item[0] <= end_time):
                        if upload_route not in section:
                            section[upload_route] = {}
                        if hotime not in section[upload_route]:
                            section[upload_route][hotime] = []
                        section[upload_route][hotime].append([item[0], item[1], item[2]])
                #print(section[upload_route][hotime])
                #input()
                for item in packet[download_route]:
                    if (item[0] >= start_time) and (item[0] <= end_time):
                        if download_route not in section:
                            section[download_route] = {}
                        if hotime not in section[download_route]:
                            section[download_route][hotime] = []
                        section[download_route][hotime].append([item[0], item[1], item[2]])
    return section

# 利用 throughput 算出區間內平均的 delay
def get_avg_delay_from_hotime(section):
    route_upload = 0
    route_download = 0

    upload_1k_delay_sum = 0
    upload_1m_delay_sum = 0
    upload_05k_delay_sum = 0

    download_1k_delay_sum = 0
    download_1m_delay_sum = 0
    download_05k_delay_sum = 0
    
    for route in section:
        for hotime in section[route]:
            #print(hotime)
            start = section[route][hotime][0][0]
            end = section[route][hotime][-1][0]
            num = len(section[route][hotime])

            throughput_per_sec = num*1024/(end-start)
            delay_05k = 512/throughput_per_sec
            delay_1k = 1024/throughput_per_sec
            delay_1m = 1024*1024/throughput_per_sec
            #print("route:",route)
            #print("num:",num)
            #print("delay_1k:",delay_1k)
            #print("delay_1m:",delay_1m)
            if route.split(" ")[0] == "1.0.0.2":
                route_download = route_download + 1
                download_05k_delay_sum = download_05k_delay_sum + delay_05k
                download_1k_delay_sum = download_1k_delay_sum + delay_1k
                download_1m_delay_sum = download_1m_delay_sum + delay_1m
            else:
                route_upload = route_upload + 1
                upload_05k_delay_sum = upload_05k_delay_sum + delay_05k
                upload_1k_delay_sum = upload_1k_delay_sum + delay_1k
                upload_1m_delay_sum = upload_1m_delay_sum + delay_1m
    if(route_upload==0):
        upload_05k_delay=0
    else:
        upload_05k_delay = upload_05k_delay_sum/route_upload
    if(route_upload==0):
        upload_1k_delay=0
    else:
        upload_1k_delay = upload_1k_delay_sum/route_upload
    if(route_upload==0):
        upload_1m_delay=0
    else:
        upload_1m_delay = upload_1m_delay_sum/route_upload
    
    if(route_download==0):
        download_05k_delay=0
    else:
        download_05k_delay = download_05k_delay_sum/route_download
    if(route_download==0):
        download_1k_delay=0
    else:
        download_1k_delay = download_1k_delay_sum/route_download
    if(route_download==0):
        download_1m_delay=0
    else:
        download_1m_delay = download_1m_delay_sum/route_download
    print("average delay of upload 0.5KB(sec):")
    print(upload_05k_delay)
    print("average delay of upload 1KB(sec):")
    print(upload_1k_delay)
    print("average delay of upload 1MB(sec):")
    print(upload_1m_delay)

    print("average delay of download 0.5KB(sec):")
    print(download_05k_delay)
    print("average delay of download 1KB(sec):")
    print(download_1k_delay)
    print("average delay of download 1MB(sec):")
    print(download_1m_delay)
    return upload_1k_delay, upload_1m_delay, upload_05k_delay, download_1k_delay, download_1m_delay, download_05k_delay

# 另一種做法(目前應該沒有使用)
def get_avg_delay_from_hotime_2(section):
    
    upload_count = 0
    download_count = 0

    upload_delay_sum = 0
    download_delay_sum = 0

    upload_1m_count = 0
    download_1m_count = 0
    upload_1m_delay_sum = 0
    download_1m_delay_sum = 0

    for route in section:
        for hotime in section[route]:
            for packet in section[route][hotime]:
                if route.split(" ")[0] == "1.0.0.2":
                    download_count = download_count + 1
                    if download_count == 1024:
                        download_1m_count = download_1m_count + 1
                        download_1m_delay_sum = download_1m_delay_sum + (packet[1]-section[route][hotime][0][0])
                    download_delay_sum = download_delay_sum + packet[2]
                else:
                    upload_count = upload_count + 1
                    if upload_count == 1024:
                        upload_1m_count = upload_1m_count + 1
                        upload_1m_delay_sum = upload_1m_delay_sum + (packet[1]-section[route][hotime][0][0])
                    upload_delay_sum = upload_delay_sum + packet[2]
    
    print("single packet upload delay:")
    print(upload_delay_sum/upload_count)
    print("single packet downlaod delay:")
    print(download_delay_sum/download_count)
    print("1m packet upload delay:")
    print(upload_1m_delay_sum/upload_1m_count)
    print("1m packet downlaod delay:")
    print(download_1m_delay_sum/download_1m_count)

# 命令列後面需要加一個參數，參數名稱為兩個 output log 共同的前綴字
# 比方說，如果兩個資料夾名稱分別是 out_sdn, out_lte，則共同的前綴字為 "out_"
# 使用本 python 檔的方式為 "python diff_time.py out_"
prefix_name = " ".join(map(shlex.quote, sys.argv[1:]))
print("prefix_name: ", prefix_name)

dir_udp_sdn = prefix_name + "sdn"

dir_udp_lte = prefix_name + "lte"

dir_output = prefix_name

# 確定兩個 log 資料夾在不在
if(dir_udp_sdn == "" or not os.path.exists(dir_udp_sdn)):
    os.mkdir( dir_udp_sdn )
    print("please check if the dir_udp_sdn is exist")
    exit()

if(dir_udp_lte == "" or not os.path.exists(dir_udp_lte)):
    os.mkdir( dir_udp_lte )
    print("please check if the dir_udp_lte is exist")
    exit()

# 確定 cache 資料夾在不在
if not os.path.exists(dir_output):
    os.mkdir( dir_output )
    print("\nmaking dir for output files:", dir_output)
    #os.makedirs(dir_output)
print("\ndir_output:", dir_output, "has exist!")

# 讀取 cache 檔案，如果沒有現有的 cache 檔案的話，會生成 cache 檔
# 生成 cache 檔的過程會分析全部 log 資料夾中，每一條 route 的資訊
# 由於 log 檔資料龐大，分析完我會儲存成一份 json 檔當作 cache 使用，使用同資料集時方便再次執行本程式
# 若是 log 檔有重新跑過的話，請刪除本 cache 檔資料夾來重跑分析程式
path_packet_sdn = dir_output + "/packet_sdn_.json"
path_packet_lte = dir_output + "/packet_lte_.json"

if(not os.path.exists(path_packet_sdn)):
    packet_sdn = get_packet(dir_udp_sdn)
    json.dump(packet_sdn, open(path_packet_sdn,'w'))
else:
    print("get packet info from file:", path_packet_sdn)
    packet_sdn = json.load(open(path_packet_sdn,'r'))

if(not os.path.exists(path_packet_lte)):
    packet_lte = get_packet(dir_udp_lte)
    json.dump(packet_lte, open(path_packet_lte,'w'))
else:
    print("get packet info from file:", path_packet_lte)
    packet_lte = json.load(open(path_packet_lte,'r'))

# 取得 handover 時間點
print("dir_udp_sdn:", dir_udp_sdn)
list_handover_time = getHOTime(dir_udp_sdn)
print("list_handover_time",list_handover_time)

# 取得 handover 時間點發生前後的一小段時間
section_sdn = get_section(list_handover_time, packet_sdn)
section_lte = get_section(list_handover_time, packet_lte)

# 算出時間區間內的平均 delay
print("\nSDN after handover:\n")
upload_1k_sdn, upload_1m_sdn, upload_05k_sdn, download_1k_sdn, download_1m_sdn, download_05k_sdn = get_avg_delay_from_hotime(section_sdn)
print("\n3GPP-X2 after handover:\n")
upload_1k_lte, upload_1m_lte, upload_05k_lte, download_1k_lte, download_1m_lte, download_05k_lte = get_avg_delay_from_hotime(section_lte)

# 畫圖表

# 1MB
fig2 = plt.figure(2, figsize=(8,6))
condition = ['Upload', 'Downlaod']
lte_delay = [upload_1m_lte, download_1m_lte]
sdn_delay = [upload_1m_sdn, download_1m_sdn]
x = np.arange(len(condition))
width = 0.4
plt.bar(x, lte_delay, width, color='blue', label='3GPP-X2')
plt.bar(x+width, sdn_delay, width, color='green', label='SDN')
plt.xticks( x + width / 2, condition)
plt.ylabel('Average Delay(sec)')
plt.title('1MB file transmission delay between 3GPP-X2 and SDN')
plt.legend( loc='upper right')
fig2.show()

# 1KB
fig1 = plt.figure(1, figsize=(8,6))
axes = plt.gca()
axes.set_ylim([0,0.006])
condition = ['Upload', 'Downlaod']
lte_delay = [upload_1k_lte, download_1k_lte]
sdn_delay = [upload_1k_sdn, download_1k_sdn]
x = np.arange(len(condition))
width = 0.4
plt.bar(x, lte_delay, width, color='blue', label='3GPP-X2')
plt.bar(x+width, sdn_delay, width, color='green', label='SDN')
plt.xticks( x + width / 2, condition)
plt.ylabel('Average Delay(sec)')
plt.title('1KB file transmission delay between 3GPP-X2 and SDN')
plt.legend( loc='upper right')
fig1.show()

# 0.5KB
fig3 = plt.figure(3, figsize=(8,6))
axes = plt.gca()
axes.set_ylim([0,0.003])
condition = ['Upload', 'Downlaod']
lte_delay = [upload_05k_lte, download_05k_lte]
sdn_delay = [upload_05k_sdn, download_05k_sdn]
x = np.arange(len(condition))
width = 0.4
plt.bar(x, lte_delay, width, color='blue', label='3GPP-X2')
plt.bar(x+width, sdn_delay, width, color='green', label='SDN')
plt.xticks( x + width / 2, condition)
plt.ylabel('Average Delay(sec)')
plt.title('0.5KB file transmission delay between 3GPP-X2 and SDN')
plt.legend( loc='upper right')
fig3.show()

input()


